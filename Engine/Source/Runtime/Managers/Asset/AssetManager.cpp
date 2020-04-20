#include "dmkafx.h"
#include "AssetManager.h"

#include "Platform/Windows.h"
#include "Objects/InternalFormat/InteralFormat.h"

namespace Dynamik {
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

	UI32 AssetManager::addAsset(AssetContainer container, UI32 sceneIndex, UI32 levelIndex)
	{
		assets[levelIndex][sceneIndex].pushBack(container);
		return assets[levelIndex][sceneIndex].size() - 1;
	}

	void AssetManager::updateAsset(AssetContainer container, UI32 assetIndex, UI32 sceneIndex, UI32 levelIndex)
	{
		assets[levelIndex][sceneIndex][assetIndex] = container;
	}

	AssetContainer AssetManager::getAsset(UI32 assetIndex, UI32 sceneIndex, UI32 levelIndex)
	{
		return assets[levelIndex][sceneIndex][assetIndex];
	}

	void AssetManager::loadScene(UI32 sceneIndex, UI32 levelIndex)
	{
		ARRAY<AssetContainer> _scene = _initializeSceneData(assets[levelIndex][sceneIndex]);

		{
			ARRAY<std::future<void>, DMKArrayDestructorCallMode::DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_ALL> threads;
			for (UI32 index = 0; index < _scene.size(); index++)
			{
				InternalFormat* _format = (InternalFormat*)_scene[index].address;

				DMKModelLoadInfo info;
				info.path = _format->descriptor.assetDescription.dynamikResouceFilePath;
				info.vertexOffset = {
					_format->descriptor.transformDescriptor.location[0],
					_format->descriptor.transformDescriptor.location[1],
					_format->descriptor.transformDescriptor.location[2],
				};
				info.meshes = &_format->meshDatas;
				threads.pushBack(std::async(std::launch::async, loadModel, info));
			}
		}

		updateScene(_scene, sceneIndex, levelIndex);
	}

	ARRAY<AssetContainer> AssetManager::_initializeSceneData(ARRAY<AssetContainer> scene)
	{
		for (UI32 index = 0; index < scene.size(); index++)
		{
			/* Update addresses from DMKGameObject* to InternalFormat* */
			scene[index].address = (InternalFormat*)scene[index].address;
			InternalFormat* _format = (InternalFormat*)scene[index].address;

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
			_format->objectPath = daiManager.getData(utils::DMKDaiFileDataType::DMK_DAI_FILE_DATA_TYPE_MODEL)[0].c_str();

			/* Get texture paths */
			for (auto texturePath : daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_TEXTURE))
				_format->texturePaths.pushBack(texturePath.c_str());

			/* Get shader paths */
			/* Vertex shader path */
			if (daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_VERTEX).size())
				_format->shaderPaths.vertexShader = (_basePath + daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_VERTEX)[0]).c_str();

			/* Tessellation shader path */
			if (daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_TESSELLATION).size())
				_format->shaderPaths.tessellationShader = (_basePath + daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_TESSELLATION)[0]).c_str();
		
			/* Geometry shader path */
			if (daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_GEOMETRY).size())
				_format->shaderPaths.grometryShader = (_basePath + daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_GEOMETRY)[0]).c_str();
		
			/* Fragment shader path */
			if (daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_FRAGMENT).size())
				_format->shaderPaths.fragmentShader = (_basePath + daiManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_FRAGMENT)[0]).c_str();
		}

		return scene;
	}
}