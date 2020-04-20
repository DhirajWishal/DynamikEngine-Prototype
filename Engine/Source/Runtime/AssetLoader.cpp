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

	void AssetLoader::loadAssets(ARRAY<DMKModelLoadInfo> loadInfos)
	{
		ARRAY<std::future<void>, DMKArrayDestructorCallMode::DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_ALL> threads = {};
		for (I32 i = 0; i < loadInfos.size(); i++)
			threads.push_back(std::async(std::launch::async, loadModel, loadInfos[i]));
	}
}