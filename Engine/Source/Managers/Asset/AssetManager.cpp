#include "dmkafx.h"
#include "AssetManager.h"

#include "Platform/Windows.h"
#include "Objects/InternalFormat/InternalFormat.h"
#include "StaticObject.h"
#include "SkyboxObject.h"

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
				_store.textureCoordinate = { mesh->mTextureCoords[0][index].x, 1.0f - mesh->mTextureCoords[0][index].y, mesh->mTextureCoords[0][index].z };
			if (mesh->HasNormals())
				_store.normal = { mesh->mNormals[index].x, mesh->mNormals[index].y, mesh->mNormals[index].z };
			_store.integrity = 1.0f;

			_mesh.vertexDataStore.push_back(_store);
		}

		aiFace face;
		for (UI32 index = 0; index < mesh->mNumFaces; index++)
		{
			face = mesh->mFaces[index];
			for (UI32 itr = 0; itr < face.mNumIndices; itr++)
				_mesh.indexes.push_back(face.mIndices[itr]);
		}

		return _mesh;
	}

	Mesh loadAnimData(aiMesh* mesh, const aiScene* scene, std::vector<VertexBoneData8> boneData)
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
				_store.textureCoordinate = { mesh->mTextureCoords[0][index].x, 1.0f - mesh->mTextureCoords[0][index].y, mesh->mTextureCoords[0][index].z };
			if (mesh->HasNormals())
				_store.normal = { mesh->mNormals[index].x, mesh->mNormals[index].y, mesh->mNormals[index].z };
			_store.integrity = 1.0f;

			VertexBoneData8* _data = StaticAllocator<VertexBoneData8>::allocate();
			for (UI32 i = 0; i < 8; i++)
			{
				_data->boneIDs[i] = boneData[index].boneIDs[i];
				_data->boneWeights[i] = boneData[index].boneWeights[i];
			}
			_store.boneData = _data;

			_mesh.vertexDataStore.push_back(_store);
		}

		aiFace face;
		for (UI32 index = 0; index < mesh->mNumFaces; index++)
		{
			face = mesh->mFaces[index];
			for (UI32 itr = 0; itr < face.mNumIndices; itr++)
				_mesh.indexes.push_back(face.mIndices[itr]);
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
					_mesh.textureDatas.push_back(_texture);
				}
			}
			else
			{
				_texture.loadCubemap(format->texturePaths, format->descriptor.assetDescription.textureInputType);
				_mesh.textureDatas.push_back(_texture);
			}

			format->meshDatas.push_back(_mesh);
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene, format);
		}
	}

	void processNode(aiNode* node, const aiScene* scene, POINTER<InternalFormat> format, AnimatedMesh animMesh)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			auto _mesh = loadAnimData(mesh, scene, animMesh.bones);;

			Texture _texture;
			if (format->descriptor.assetDescription.textureType == DMKTextureType::DMK_TEXTURE_TYPE_2D || format->descriptor.assetDescription.textureType == DMKTextureType::DMK_TEXTURE_TYPE_3D)
			{
				for (auto _path : format->texturePaths)
				{
					_texture.loadTexture(_path, format->descriptor.assetDescription.textureType, format->descriptor.assetDescription.textureInputType);
					_mesh.textureDatas.push_back(_texture);
				}
			}
			else
			{
				_texture.loadCubemap(format->texturePaths, format->descriptor.assetDescription.textureInputType);
				_mesh.textureDatas.push_back(_texture);
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

	UI32 AssetManager::addLevel(std::vector<std::vector<AssetContainer>> containers)
	{
		assets.push_back(containers);
		return assets.size() - 1;
	}

	void AssetManager::updateLevel(std::vector<std::vector<AssetContainer>> containers, UI32 index)
	{
		assets[index] = containers;
	}

	std::vector<std::vector<AssetContainer>> AssetManager::getLevel(UI32 index)
	{
		return assets[index];
	}

	UI32 AssetManager::addScene(std::vector<AssetContainer> containers, UI32 levelIndex)
	{
		assets[levelIndex].push_back(containers);
		return assets[levelIndex].size() - 1;
	}

	void AssetManager::updateScene(std::vector<AssetContainer> containers, UI32 sceneIndex, UI32 levelIndex)
	{
		assets[levelIndex][sceneIndex] = containers;
	}

	std::vector<AssetContainer> AssetManager::getScene(UI32 sceneIndex, UI32 levelIndex)
	{
		return assets[levelIndex][sceneIndex];
	}

	UI32 AssetManager::addAsset(POINTER<DMKGameObject> object, UI32 sceneIndex, UI32 levelIndex)
	{
		assets[levelIndex][sceneIndex].push_back(createAssetContainer(object));
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
		currentLevel = levelIndex;
		currentScene = sceneIndex;

		/* Initialize the scene data to get the required data from .dai file */
		STORE _scene = _initializeSceneData(assets[levelIndex][sceneIndex]);

		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Run each object loading function in a separate thread to make things faster */
			std::vector<std::future<void>> threads;
			for (UI32 index = 0; index < _scene.size(); index++)
			{
				DMK_BEGIN_PROFILE_TIMER();

				POINTER<InternalFormat> _format = _scene[index].address;
				if (_format->meshDatas.size())
					continue;

				if (_format->type == DMKObjectType::DMK_OBJECT_TYPE_SKELETAL_ANIMATION)
					threads.push_back(std::async(std::launch::async, LoadAnimation, _format));
				else
					threads.push_back(std::async(std::launch::async, LoadAsset, _format));
			}
		}

		/* Update the scene data */
		updateScene(_scene, sceneIndex, levelIndex);
	}

	std::vector<AssetContainer> AssetManager::getRenderableAssets(UI32 sceneIndex, UI32 levelIndex)
	{
		DMK_BEGIN_PROFILE_TIMER();

		std::vector<AssetContainer>_assets;

		for (AssetContainer _asset : assets[levelIndex][sceneIndex])
			if (_asset.type <= DMKObjectType::DMK_OBJECT_TYPE_CAMERA)
				_assets.push_back(_asset);

		return _assets;
	}

	std::vector<POINTER<InternalFormat>> AssetManager::getRenderablesAsInternalFormats(UI32 sceneIndex, UI32 levelIndex)
	{
		DMK_BEGIN_PROFILE_TIMER();

		std::vector<POINTER<InternalFormat>> _formats;

		/* Go through all the assets and return the renderable objects in the scene */
		for (AssetContainer _asset : assets[levelIndex][sceneIndex])
		{
			if (!_asset.address.isValid() || (I32)_asset.type < 0)
				continue;

			if (_asset.type <= DMKObjectType::DMK_OBJECT_TYPE_CAMERA)
				_formats.push_back(_asset.address);
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
		Assimp::Importer myImporter;
		AnimatedMesh _animatedMesh;
		_animatedMesh.scene = myImporter.ReadFile(format->objectPath,
			aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
		_animatedMesh.setAnimation(0);

		if (!_animatedMesh.scene)
			DMK_CORE_FATAL("Unable to load the specified file!");

		UI32 vertexCount = 0;
		for (UI32 m = 0; m < _animatedMesh.scene->mNumMeshes; m++) {
			vertexCount += _animatedMesh.scene->mMeshes[m]->mNumVertices;
		};
		_animatedMesh.bones.resize(vertexCount);
		_animatedMesh.globalInverseTransform = _animatedMesh.scene->mRootNode->mTransformation;
		_animatedMesh.globalInverseTransform.Inverse();

		UI32 vertexBase(0);
		for (UI32 m = 0; m < _animatedMesh.scene->mNumMeshes; m++) {
			aiMesh* paiMesh = _animatedMesh.scene->mMeshes[m];
			if (paiMesh->mNumBones > 0)
				_animatedMesh.loadBones(paiMesh, vertexBase, _animatedMesh.bones);
			vertexBase += _animatedMesh.scene->mMeshes[m]->mNumVertices;
		}

		Mesh _mesh;
		MeshPointStore _store;
		for (UI32 m = 0; m < _animatedMesh.scene->mNumMeshes; m++) {
			for (UI32 index = 0; index < _animatedMesh.scene->mMeshes[m]->mNumVertices; index++) {
				if (_animatedMesh.scene->mMeshes[m]->HasPositions())
					_store.position =
				{
					_animatedMesh.scene->mMeshes[m]->mVertices[index].x,
					_animatedMesh.scene->mMeshes[m]->mVertices[index].y,
					_animatedMesh.scene->mMeshes[m]->mVertices[index].z
				};
				if (_animatedMesh.scene->mMeshes[m]->mColors[0])
					_store.color =
				{
					_animatedMesh.scene->mMeshes[m]->mColors[0][index].r,
					_animatedMesh.scene->mMeshes[m]->mColors[0][index].g,
					_animatedMesh.scene->mMeshes[m]->mColors[0][index].b
				};
				if (_animatedMesh.scene->mMeshes[m]->mTextureCoords[0])
					_store.textureCoordinate =
				{
					_animatedMesh.scene->mMeshes[m]->mTextureCoords[0][index].x,
					1.0f - _animatedMesh.scene->mMeshes[m]->mTextureCoords[0][index].y,
					_animatedMesh.scene->mMeshes[m]->mTextureCoords[0][index].z
				};
				if (_animatedMesh.scene->mMeshes[m]->HasNormals())
					_store.normal =
				{
					_animatedMesh.scene->mMeshes[m]->mNormals[index].x,
					_animatedMesh.scene->mMeshes[m]->mNormals[index].y,
					_animatedMesh.scene->mMeshes[m]->mNormals[index].z
				};
				_store.integrity = 1.0f;

				VertexBoneData8* _data = StaticAllocator<VertexBoneData8>::allocate();
				for (UI32 i = 0; i < 8; i++)
				{
					_data->boneIDs[i] = _animatedMesh.bones[index].boneIDs[i];
					_data->boneWeights[i] = _animatedMesh.bones[index].boneWeights[i];
				}
				_store.boneData = _data;

				_mesh.vertexDataStore.push_back(_store);
			}

			aiFace face;
			for (UI32 index = 0; index < _animatedMesh.scene->mMeshes[m]->mNumFaces; index++)
			{
				face = _animatedMesh.scene->mMeshes[m]->mFaces[index];
				for (UI32 itr = 0; itr < face.mNumIndices; itr++)
					_mesh.indexes.push_back(face.mIndices[itr]);
			}

			Texture _texture;
			if (format->descriptor.assetDescription.textureType == DMKTextureType::DMK_TEXTURE_TYPE_2D || format->descriptor.assetDescription.textureType == DMKTextureType::DMK_TEXTURE_TYPE_3D)
			{
				for (auto _path : format->texturePaths)
				{
					_texture.loadTexture(_path, format->descriptor.assetDescription.textureType, format->descriptor.assetDescription.textureInputType);
					_mesh.textureDatas.push_back(_texture);
				}
			}
			else
			{
				_texture.loadCubemap(format->texturePaths, format->descriptor.assetDescription.textureInputType);
				_mesh.textureDatas.push_back(_texture);
			}

			format->meshDatas.push_back(_mesh);
		}

		processNode(_animatedMesh.scene->mRootNode, _animatedMesh.scene, format, _animatedMesh);
		format->animation.push_back(_animatedMesh);
	}

	AssetManager::STORE AssetManager::_initializeSceneData(std::vector<AssetContainer> scene)
	{
		DMK_BEGIN_PROFILE_TIMER();

		for (UI32 index = 0; index < scene.size(); index++)
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Update addresses from DMKGameObject* to POINTER<InternalFormat> */
			POINTER<InternalFormat> _format = scene[index].address;

			if (_format->objectPath.size())
				continue;

			std::string _basePath = _format->descriptor.assetDescription.dynamikResouceFilePath;

			/* Check if the file is successfully opened */
			if (
				(_basePath.find(".obj") != std::string::npos) ||
				(_basePath.find(".fbx") != std::string::npos) ||
				(_basePath.find(".dae") != std::string::npos) ||
				(_basePath.find(".fmt") != std::string::npos))
			{
				DMK_CORE_ERROR("Unable to open the DAI file. Attempting to open the file as a default object.");

				_format->objectPath = _basePath;
				_format->texturePaths.push_back("Defaults/default.png");
				_format->shaderPaths.vertexShader = "Defaults/vert.spv";
				_format->shaderPaths.fragmentShader = "Defaults/frag.spv";
			}
			else
			{
				/* Instantiate a dai file manager */
				utils::daiManager daiManager;

				if (_basePath.find(".dai") != std::string::npos)
				{
					daiManager.open(_basePath);
					daiManager.initDataStore();
					_basePath = _basePath.substr(0, _basePath.size() - 13);

					_format = createAssetContainer(_resolveObject(daiManager.getObjectType())).address;
				}
				else
				{
					/* Create the basepath for the resources */
					_basePath += ((_basePath[_basePath.size() - 1] == '/' || _basePath[_basePath.size() - 1] == '\\') ? "" : "/");
					std::string _fullPath = _basePath + "modelData.dai";

					/* Open the DAI file */
					daiManager.open(_fullPath);
				}

				/* Get object path */
				_format->objectPath = _basePath + daiManager.getData(utils::DMKDaiFileDataType::DMK_DAI_FILE_DATA_TYPE_MODEL)[0];

				/* Get texture paths */
				for (auto texturePath : daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_TEXTURE))
					_format->texturePaths.push_back(_basePath + texturePath);

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
			}

			copyToAssetContainer(&scene[index], _format);
		}

		return scene;
	}
	
	POINTER<DMKGameObject> AssetManager::_resolveObject(DMKObjectType type)
	{
		POINTER<DMKGameObject> _object;

		switch (type)
		{
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_IMAGE_2D:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_BOUNDING_BOX:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_MESH:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC:
			_object = StaticAllocator<DMKStaticObject>::allocate();
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PLAYER:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_NPC:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXTURE_UI:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKYBOX:
			_object = StaticAllocator<DMKSkyboxObject>::allocate();
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SPRITES:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_FONT:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXT_OVERLAY:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PARTICLE:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTION:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTOR:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKELETAL_ANIMATION:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_LIGHT:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_CAMERA:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_AUDIO:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_VIDEO:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_WIND:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_AI:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SCRIPT:
			break;
		case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_KEY_BINDINGS:
			break;
		default:
			break;
		}

		_object->initialize();
		return _object;
	}
}