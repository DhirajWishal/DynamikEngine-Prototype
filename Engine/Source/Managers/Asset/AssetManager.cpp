#include "dmkafx.h"
#include "AssetManager.h"

#include "Platform/Windows.h"
#include "Objects/InternalFormat/InternalFormat.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace Dynamik {
	/* Helper function */
	Mesh loadData(aiMesh* mesh, const aiScene* scene)
	{
		Mesh _mesh;
		MeshPointStore _store;

		for (UI32 index = 0; index < mesh->mNumVertices; index++)
		{
			if (mesh->HasPositions())
				_store.position = { mesh->mVertices[index].x, mesh->mVertices[index].y, mesh->mVertices[index].z };
			if (mesh->mColors[0])
				_store.color = { mesh->mColors[0][index].r, mesh->mColors[0][index].g, mesh->mColors[0][index].b };
			if (mesh->mTextureCoords[0])
				_store.textureCoordinate = { mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y, mesh->mTextureCoords[0][index].z };
			if (mesh->HasNormals())
				_store.normal = { mesh->mNormals[index].x, mesh->mNormals[index].y, mesh->mNormals[index].z };
			_store.integrity = 1.0f;

			_mesh.vertexDataStore.pushBack(_store);
		}

		aiFace face;
		for (UI32 index = 0; index < mesh->mNumFaces; index++)
		{
			face = mesh->mFaces[index];
			for (UI32 itr = 0; itr < face.mNumIndices; itr++)
				_mesh.indexes.pushBack(face.mIndices[itr]);
		}

		return _mesh;
	}

	void processNode(aiNode* node, const aiScene* scene, POINTER<InternalFormat> format)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			auto _mesh = loadData(mesh, scene);

			Texture _texture;
			if (format->descriptor.assetDescription.textureType == DMKTextureType::DMK_TEXTURE_TYPE_2D || format->descriptor.assetDescription.textureType == DMKTextureType::DMK_TEXTURE_TYPE_3D)
			{
				for (auto _path : format->texturePaths)
				{
					_texture.loadTexture(_path, format->descriptor.assetDescription.textureType, format->descriptor.assetDescription.textureInputType);
					_mesh.textureDatas.pushBack(_texture);
				}
			}
			else
			{
				_texture.loadCubemap(format->texturePaths, format->descriptor.assetDescription.textureInputType);
				_mesh.textureDatas.pushBack(_texture);
			}

			format->meshDatas.push_back(_mesh);
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene, format);
		}
	}

	/* Asset Manager class definitions */
	AssetManager::~AssetManager()
	{
		for (auto level : assets)
			for (auto scene : level)
				for (auto asset : scene)
					StaticAllocator<InternalFormat>::deAllocate(asset.address);
	}

	UI32 AssetManager::addLevel(ARRAY<ARRAY<AssetContainer>> containers)
	{
		assets.pushBack(containers);
		return assets.size() - 1;
	}

	void AssetManager::updateLevel(ARRAY<ARRAY<AssetContainer>> containers, UI32 index)
	{
		assets[index] = containers;
	}

	ARRAY<ARRAY<AssetContainer>> AssetManager::getLevel(UI32 index)
	{
		return assets[index];
	}

	UI32 AssetManager::addScene(ARRAY<AssetContainer> containers, UI32 levelIndex)
	{
		assets[levelIndex].pushBack(containers);
		return assets[levelIndex].size() - 1;
	}

	void AssetManager::updateScene(ARRAY<AssetContainer> containers, UI32 sceneIndex, UI32 levelIndex)
	{
		assets[levelIndex][sceneIndex] = containers;
	}

	ARRAY<AssetContainer> AssetManager::getScene(UI32 sceneIndex, UI32 levelIndex)
	{
		return assets[levelIndex][sceneIndex];
	}

	UI32 AssetManager::addAsset(POINTER<DMKGameObject> object, UI32 sceneIndex, UI32 levelIndex)
	{

		assets[levelIndex][sceneIndex].pushBack(createAssetContainer(object));
		return assets[levelIndex][sceneIndex].size() - 1;
	}

	void AssetManager::updateAsset(POINTER<DMKGameObject> object, UI32 assetIndex, UI32 sceneIndex, UI32 levelIndex)
	{
		assets[levelIndex][sceneIndex][assetIndex] = createAssetContainer(object);
	}

	AssetContainer AssetManager::getAsset(UI32 assetIndex, UI32 sceneIndex, UI32 levelIndex)
	{
		return assets[levelIndex][sceneIndex][assetIndex];
	}

	void AssetManager::loadScene(UI32 sceneIndex, UI32 levelIndex)
	{
		/* Initialize the scene data to get the required data from .dai file */
		STORE _scene = _initializeSceneData(assets[levelIndex][sceneIndex]);

		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Run each object loading function in a separate thread to make things faster */
			ARRAY<std::future<void>, DMKArrayDestructorCallMode::DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_ALL> threads;
			for (UI32 index = 0; index < _scene.size(); index++)
			{
				DMK_BEGIN_PROFILE_TIMER();

				POINTER<InternalFormat> _format = _scene[index].address;
				threads.pushBack(std::async(std::launch::async, LoadAsset, _format));
			}
		}

		/* Update the scene data */
		updateScene(_scene, sceneIndex, levelIndex);
	}

	ARRAY<AssetContainer> AssetManager::getRenderableAssets(UI32 sceneIndex, UI32 levelIndex)
	{
		DMK_BEGIN_PROFILE_TIMER();

		ARRAY<AssetContainer>_assets;

		for (AssetContainer _asset : assets[levelIndex][sceneIndex])
			if (_asset.type <= DMKObjectType::DMK_OBJECT_TYPE_CAMERA)
				_assets.pushBack(_asset);

		return _assets;
	}

	ARRAY<POINTER<InternalFormat>> AssetManager::getRenderablesAsInternalFormats(UI32 sceneIndex, UI32 levelIndex)
	{
		DMK_BEGIN_PROFILE_TIMER();

		ARRAY<POINTER<InternalFormat>> _formats;

		/* Go through all the assets and return the renderable objects in the scene */
		for (AssetContainer _asset : assets[levelIndex][sceneIndex])
		{
			if (!_asset.address.isValid() || (I32)_asset.type < 0)
				continue;

			if (_asset.type <= DMKObjectType::DMK_OBJECT_TYPE_CAMERA)
				_formats.pushBack(_asset.address);
		}

		return _formats;
	}

	AssetContainer AssetManager::createAssetContainer(POINTER<DMKGameObject> object)
	{
		DMK_BEGIN_PROFILE_TIMER();

		/* Create a new Asset Container */
		AssetContainer _container;

		/* Heap allocate a memory block and move the data in the Dynamik Game Object to it */
		_container.address = StaticAllocator<InternalFormat>::allocate(sizeof(InternalFormat));
		StaticAllocator<InternalFormat>::set(_container.address, InternalFormat());
		memcpy(_container.address.get(), object.get(), object.getTypeSize());

		_container.type = object->type;

		return _container;
	}

	void AssetManager::copyToAssetContainer(POINTER<AssetContainer> container, POINTER<InternalFormat> format)
	{
		container->type = format->type;
		memcpy(container->address.get(), format.get(), format.getTypeSize());
	}

	void AssetManager::LoadAsset(POINTER<InternalFormat> format)
	{
		Assimp::Importer myImporter;
		auto scene = myImporter.ReadFile(format->objectPath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene)
			DMK_CORE_FATAL("Unable to load the specified file!");

		processNode(scene->mRootNode, scene, format);
	}

	void AssetManager::LoadAnimation(POINTER<InternalFormat> format)
	{
	}

	AssetManager::STORE AssetManager::_initializeSceneData(ARRAY<AssetContainer> scene)
	{
		DMK_BEGIN_PROFILE_TIMER();

		for (UI32 index = 0; index < scene.size(); index++)
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Update addresses from DMKGameObject* to POINTER<InternalFormat> */
			POINTER<InternalFormat> _format = scene[index].address;

			/* Instantiate a dai file manager */
			utils::daiManager daiManager;

			/* Create the basepath for the resources */
			std::string _basePath = _format->descriptor.assetDescription.dynamikResouceFilePath;
			_basePath += ((_basePath[_basePath.size() - 1] == '/' || _basePath[_basePath.size() - 1] == '\\') ? "" : "/");
			std::string _fullPath = _basePath + "modelData.dai";

			/* Open the DAI file */
			daiManager.open(_fullPath);

			/* Check if the file is successfully opened */
			if (!daiManager.isOpen())
				DMK_CORE_FATAL("Unable to open the DAI file @ " + _basePath);

			/* Get object path */
			_format->objectPath = _basePath + daiManager.getData(utils::DMKDaiFileDataType::DMK_DAI_FILE_DATA_TYPE_MODEL)[0];

			/* Get texture paths */
			for (auto texturePath : daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_TEXTURE))
				_format->texturePaths.pushBack(_basePath + texturePath);

			/* Get shader paths */
			/* Vertex shader path */
			if (daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_VERTEX).size())
				_format->shaderPaths.vertexShader = _basePath + daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_VERTEX)[0];

			/* Tessellation shader path */
			if (daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_TESSELLATION).size())
				_format->shaderPaths.tessellationShader = _basePath + daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_TESSELLATION)[0];

			/* Geometry shader path */
			if (daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_GEOMETRY).size())
				_format->shaderPaths.geometryShader = _basePath + daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_GEOMETRY)[0];

			/* Fragment shader path */
			if (daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_FRAGMENT).size())
				_format->shaderPaths.fragmentShader = _basePath + daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_FRAGMENT)[0];

			copyToAssetContainer(&scene[index], _format);
		}

		return scene;
	}
}