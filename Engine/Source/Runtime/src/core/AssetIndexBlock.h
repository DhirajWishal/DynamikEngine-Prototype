#pragma once
#ifndef _DYNAMIK_ASSET_INDEX_BLOCK_H
#define _DYNAMIK_ASSET_INDEX_BLOCK_H

#include <DataTypesLib/Public/Array.h>

namespace Dynamik {
	struct DMKAssetIndexContainer {
		POINTER<UI32> address;
		UI32 byteSize = 0;
		DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT;
	};

	struct DMKSceneDataContainer {
		ARRAY<DMKAssetIndexContainer> assets;
	};

	struct DMKLevelDataContainer {
		ARRAY<DMKSceneDataContainer> scenes;
	};

	enum DMKIndexBlock {
		DMK_INDEX_BLOCK_LEVEL_DATA,
		DMK_INDEX_BLOCK_ENGINE_DATA,
		DMK_INDEX_BLOCK_USER_DATA,

		DMK_INDEX_BLOCK_MAX
	};

	class AssetIndexBlock {
	public:
		AssetIndexBlock();
		~AssetIndexBlock();

		// return level index
		UI32 addLevel(DMKLevelDataContainer container);
		void updateLevel(DMKLevelDataContainer container, UI32 index);
		DMKLevelDataContainer getLevel(UI32 index);

		// return scene index
		UI32 addScene(DMKSceneDataContainer container, UI32 levelIndex);
		void updateScene(DMKSceneDataContainer container, UI32 sceneIndex = 1, UI32 levelIndex = 1);
		DMKSceneDataContainer getScene(UI32 sceneIndex, UI32 levelIndex = 1);

		// return object index
		UI32 addAsset(DMKAssetIndexContainer container, UI32 sceneIndex = 1, UI32 levelIndex = 1);
		void updateAsset(DMKAssetIndexContainer container, UI32 assetIndex, UI32 sceneIndex = 1, UI32 levelIndex = 1);
		DMKAssetIndexContainer getAsset(UI32 assetIndex, UI32 sceneIndex = 1, UI32 levelIndex = 1);

	private:
		ARRAY<DMKLevelDataContainer> levelDataContainers;
	};
}

#endif // !_DYNAMIK_ASSET_INDEX_BLOCK_H
