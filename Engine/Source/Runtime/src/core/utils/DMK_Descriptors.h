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

			ARRAY<std::string>& modelPaths;
			ARRAY<ARRAY<std::string>>& texturePaths;

			UI32* progress = nullptr;
		};
	}
}

#endif	// !_DYNAMIK_DESCRIPTORS_H