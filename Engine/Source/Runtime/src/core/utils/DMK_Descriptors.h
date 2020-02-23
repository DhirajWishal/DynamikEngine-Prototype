#pragma once

#ifndef _DYNAMIK_DESCRIPTORS_H
#define _DYNAMIK_DESCRIPTORS_H

#include <string>
#include <vector>

namespace Dynamik {
	namespace initInfo {
		struct ADGR {
			std::string basePath = "";

			float baseMipLevel = 1.0f;

			std::vector<std::string>& modelPaths;
			std::vector<std::vector<std::string>>& texturePaths;

			uint32_t* progress = nullptr;
		};
	}
}

#endif	// !_DYNAMIK_DESCRIPTORS_H