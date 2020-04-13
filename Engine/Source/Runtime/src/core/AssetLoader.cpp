#include "dmkafx.h"
#include "AssetLoader.h"

namespace Dynamik {
	void AssetLoader::loadAsset(FSTR path)
	{
		if (path.find(".obj") != DMK_STRING_MATCH_NOT_FOUND)
			loadAsset(path, DMKAssetExtension::DMK_ASSET_EXTENSION_OBJ);
	}
	
	void AssetLoader::loadAsset(FSTR path, DMKAssetExtension ext)
	{
	}
}