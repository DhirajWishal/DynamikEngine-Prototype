#pragma once
#ifndef _DYNAMIK_ASSET_LOADER_H
#define _DYNAMIK_ASSET_LOADER_H

#include "AssetIndexBlock.h"

namespace Dynamik {
	enum class DMKAssetExtension {
		DMK_ASSET_EXTENSION_OBJ,
	};

	class AssetLoader {
	public:
		AssetLoader() {}
		~AssetLoader() {}

		void loadAsset(FSTR path);
		void loadAsset(FSTR path, DMKAssetExtension ext);

		static void loadAssets(ARRAY<DMKModelLoadInfo> loadInfos);
	};
}

#endif // !_DYNAMIK_ASSET_LOADER_H
