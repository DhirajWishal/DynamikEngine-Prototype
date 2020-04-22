#pragma once
#ifndef _DYNAMIK_ASSET_MANAGER_H
#define _DYNAMIK_ASSET_MANAGER_H

#include "Objects/DMKObject/DMKObject.h"
#include "Public/Array.h"
#include "GameObject.h"
#include "core.h"
#include "Objects/InternalFormat/InteralFormat.h"

namespace Dynamik {
	/* Scene data descriptor */
	struct DMK_API DMKSceneDescriptor {
		CCPTR sceneID = "Scene_1";
		UI32 sceneIndex = 0;
		ARRAY<DMKGameObject*> assets;
	};

	/* Level data descriptor */
	struct DMK_API DMKLevelDescriptor {
		CCPTR levelID = "Level_1";
		UI32 levelIndex = 0;
		ARRAY<DMKSceneDescriptor> scenes;
		F32 levelCompletion = 0.0f;
	};

	struct AssetContainer {
		VPTR address;
		UI32 byteSize = 0;
		DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC;
	};

	/* Asset manager for the Dynamik Engine */
	class AssetManager : public DMKObject {
		using STORE = ARRAY<AssetContainer>;

	public:
		AssetManager() {}
		~AssetManager() {}

		/* Returns the level index */
		UI32 addLevel(ARRAY<ARRAY<AssetContainer>> containers);
		void updateLevel(ARRAY<ARRAY<AssetContainer>> containers, UI32 index = 0);
		ARRAY<ARRAY<AssetContainer>> getLevel(UI32 index);

		/* Returns the scene index */
		UI32 addScene(ARRAY<AssetContainer> containers, UI32 levelIndex);
		void updateScene(ARRAY<AssetContainer> containers, UI32 sceneIndex, UI32 levelIndex);
		ARRAY<AssetContainer> getScene(UI32 sceneIndex, UI32 levelIndex);

		/* Returns the object index */
		UI32 addAsset(AssetContainer container, UI32 sceneIndex, UI32 levelIndex);
		void updateAsset(AssetContainer container, UI32 assetIndex, UI32 sceneIndex, UI32 levelIndex);
		AssetContainer getAsset(UI32 assetIndex, UI32 sceneIndex, UI32 levelIndex);

		/* Loads a scene data to the memory */
		void loadScene(UI32 sceneIndex, UI32 levelIndex);

		/* Filter renderable assets and return them */
		ARRAY<AssetContainer> getRenderableAssets(UI32 sceneIndex, UI32 levelIndex);
		ARRAY<POINTER<InternalFormat>> getRenderablesAsInternalFormats(UI32 sceneIndex, UI32 levelIndex);

	private:
		ARRAY<ARRAY<ARRAY<AssetContainer>>> assets;
		ARRAY<ARRAY<ARRAY<AssetContainer>>> renderableObjects;

		STORE _initializeSceneData(ARRAY<AssetContainer> scene);
	};
}

#endif // !_DYNAMIK_ASSET_MANAGER_H
