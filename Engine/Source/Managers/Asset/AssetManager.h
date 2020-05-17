#pragma once
#ifndef _DYNAMIK_ASSET_MANAGER_H
#define _DYNAMIK_ASSET_MANAGER_H

#include "Objects/DMKObject/DMKObject.h"
#include <vector>
#include "GameObject.h"

#include "Objects/InternalFormat/InternalFormat.h"

namespace Dynamik {
	/* Scene data descriptor */
	struct  DMKSceneDescriptor {
		CCPTR sceneID = "Scene_1";
		UI32 sceneIndex = 0;
		std::vector<DMKGameObject*> assets;
	};

	/* Level data descriptor */
	struct  DMKLevelDescriptor {
		CCPTR levelID = "Level_1";
		UI32 levelIndex = 0;
		std::vector<DMKSceneDescriptor> scenes;
		F32 levelCompletion = 0.0f;
	};

	struct AssetContainer {
		POINTER<InternalFormat> address;
		DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC;
	};

	/* Asset manager for the Dynamik Engine */
	class AssetManager : public DMKObject {
		using STORE = std::vector<AssetContainer>;

	public:
		AssetManager() {}
		~AssetManager();

		/* Returns the level index */
		UI32 addLevel(std::vector<std::vector<AssetContainer>> containers);
		void updateLevel(std::vector<std::vector<AssetContainer>> containers, UI32 index = 0);
		std::vector<std::vector<AssetContainer>> getLevel(UI32 index);

		/* Returns the scene index */
		UI32 addScene(std::vector<AssetContainer> containers, UI32 levelIndex);
		void updateScene(std::vector<AssetContainer> containers, UI32 sceneIndex, UI32 levelIndex);
		std::vector<AssetContainer> getScene(UI32 sceneIndex, UI32 levelIndex);

		/* Returns the object index */
		UI32 addAsset(POINTER<DMKGameObject> object, UI32 sceneIndex, UI32 levelIndex);
		void updateAsset(POINTER<DMKGameObject> object, UI32 assetIndex, UI32 sceneIndex, UI32 levelIndex);
		AssetContainer getAsset(UI32 assetIndex, UI32 sceneIndex, UI32 levelIndex);

		/* Loads a scene data to the memory */
		void loadScene(UI32 sceneIndex, UI32 levelIndex);

		/* Filter renderable assets and return them */
		std::vector<AssetContainer> getRenderableAssets(UI32 sceneIndex, UI32 levelIndex);
		std::vector<POINTER<InternalFormat>> getRenderablesAsInternalFormats(UI32 sceneIndex, UI32 levelIndex);

		/* Create an Asset Container statically */
		static AssetContainer createAssetContainer(POINTER<DMKGameObject> object);
		static void copyToAssetContainer(POINTER<AssetContainer> container, POINTER<InternalFormat> format);

		/* Load FBX files */
		/* Loads a full scene to multiple meshes and rendered using one shader */
		static void LoadAsset(POINTER<InternalFormat> format);
		static void LoadAnimation(POINTER<InternalFormat> format);

	private:
		std::vector<std::vector<std::vector<AssetContainer>>> assets;
		std::vector<std::vector<std::vector<AssetContainer>>> renderableObjects;

		STORE _initializeSceneData(std::vector<AssetContainer> scene);

		UI32 currentLevel = 0;
		UI32 currentScene = 0;
	};
}

#endif // !_DYNAMIK_ASSET_MANAGER_H
