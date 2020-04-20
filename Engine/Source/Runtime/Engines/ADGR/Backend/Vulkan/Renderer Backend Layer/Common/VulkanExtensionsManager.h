#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_EXTENSION_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_EXTENSION_MANAGER_H

#include "Public/Array.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanExtensionsManager {
			public:
				VulkanExtensionsManager() {}
				~VulkanExtensionsManager() {}

				static ARRAY<CCPTR> getRequiredExtentions(B1);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_EXTENSION_H
