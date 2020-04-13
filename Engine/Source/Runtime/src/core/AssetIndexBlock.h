#pragma once
#ifndef _DYNAMIK_ASSET_INDEX_BLOCK_H
#define _DYNAMIK_ASSET_INDEX_BLOCK_H

#include <DataTypesLib/Public/Array.h>

namespace Dynamik {
	struct DMKAssetIndexContainer {
		POINTER<UI32> address;
		UI32 byteSize = 0;
	};

	class AssetIndexBlock {
	public:
		AssetIndexBlock() {}
		~AssetIndexBlock() {}

		// Returns ID to the added container
		UI32 addAssetIndexContainer(DMKAssetIndexContainer container);
		DMKAssetIndexContainer getAssetIndexContainer(UI32 ID);

		B1 isIDValid(UI32 ID);
		B1 isIndexContainerUsed(UI32 ID);

	private:
		struct _AssetIndexContainer_wrapper {
			DMKAssetIndexContainer container;
			B1 isInUse = false;
		};

		ARRAY<_AssetIndexContainer_wrapper> containers;
		UI32 currentContainerIndex = 0;
	};
}

#endif // !_DYNAMIK_ASSET_INDEX_BLOCK_H
