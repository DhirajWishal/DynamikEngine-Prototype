#include "dmkafx.h"
#include "AssetIndexBlock.h"

namespace Dynamik {
	UI32 AssetIndexBlock::addAssetIndexContainer(DMKAssetIndexContainer container)
	{
		_AssetIndexContainer_wrapper _wrapper;
		_wrapper.container = container;
		_wrapper.isInUse = false;
		containers.pushBack(_wrapper);

		return currentContainerIndex++;
	}
	
	DMKAssetIndexContainer AssetIndexBlock::getAssetIndexContainer(UI32 ID)
	{
		_AssetIndexContainer_wrapper _wrapper = containers[ID];
		if (!_wrapper.isInUse)
		{
			containers[ID].isInUse = true;
			return _wrapper.container;
		}

		return DMKAssetIndexContainer();
	}
	
	B1 AssetIndexBlock::isIDValid(UI32 ID)
	{
		return ID <= currentContainerIndex;
	}
	
	B1 AssetIndexBlock::isIndexContainerUsed(UI32 ID)
	{
		return containers[ID].isInUse;
	}
}