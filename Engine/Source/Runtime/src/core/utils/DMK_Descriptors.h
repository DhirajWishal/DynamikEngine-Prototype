#pragma once

#ifndef _DYNAMIK_DESCRIPTORS_H
#define _DYNAMIK_DESCRIPTORS_H

#include <string>
#include <vector>

namespace Dynamik {
	namespace initInfo {
		struct ADGR {
			std::wstring basePath = L"";

			float baseMipLevel = 1.0f;

			std::vector<std::wstring>& modelPaths;
			std::vector<std::vector<std::wstring>>& texturePaths;

			uint32_t* progress = nullptr;
		};
	}
}

#endif	// !_DYNAMIK_DESCRIPTORS_H