#include "dmkafx.h"
#include "AssetIndexBlock.h"

namespace Dynamik {
	AssetIndexBlock::AssetIndexBlock()
	{
	}

	AssetIndexBlock::~AssetIndexBlock()
	{
	}

	UI32 AssetIndexBlock::addLevel(DMKLevelDataContainer container)
	{
		levelDataContainers.pushBack(container);
		return levelDataContainers.size() - 1;
	}

	void AssetIndexBlock::updateLevel(DMKLevelDataContainer container, UI32 index)
	{
		levelDataContainers[index] = container;
	}
	
	DMKLevelDataContainer AssetIndexBlock::getLevel(UI32 index)
	{
		return levelDataContainers[index];
	}
	
	UI32 AssetIndexBlock::addScene(DMKSceneDataContainer container, UI32 levelIndex)
	{
		levelDataContainers[levelIndex].scenes.pushBack(container);
		return levelDataContainers[levelIndex].scenes.size() - 1;
	}
	
	void AssetIndexBlock::updateScene(DMKSceneDataContainer container, UI32 sceneIndex, UI32 levelIndex)
	{
		levelDataContainers[levelIndex].scenes[sceneIndex] = container;
	}
	
	DMKSceneDataContainer AssetIndexBlock::getScene(UI32 sceneIndex, UI32 levelIndex)
	{
		return levelDataContainers[levelIndex].scenes[sceneIndex];
	}
	
	UI32 AssetIndexBlock::addAsset(DMKAssetIndexContainer container, UI32 sceneIndex, UI32 levelIndex)
	{
		levelDataContainers[levelIndex].scenes[sceneIndex].assets.pushBack(container);
		return levelDataContainers[levelIndex].scenes[sceneIndex].assets.size() - 1;
	}
	
	void AssetIndexBlock::updateAsset(DMKAssetIndexContainer container, UI32 assetIndex, UI32 sceneIndex, UI32 levelIndex)
	{
		levelDataContainers[levelIndex].scenes[sceneIndex].assets[assetIndex] = container;
	}
	
	DMKAssetIndexContainer AssetIndexBlock::getAsset(UI32 assetIndex, UI32 sceneIndex, UI32 levelIndex)
	{
		return levelDataContainers[levelIndex].scenes[sceneIndex].assets[assetIndex];
	}
}