#pragma once

#ifndef _DYNAMIK_DESCRIPTORS_
#define _DYNAMIK_DESCRIPTORS_

#include <string>
#include <vector>

namespace Dynamik {
	namespace initInfo {
		struct ADGR {
			std::string basePath = "";

			float baseMipLevel = 1.0f;

			std::vector<std::string>& texturePaths;
			std::vector<std::string>& modelPaths;

			uint32_t* progress = nullptr;
		};
	}
}

#endif