#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_EXTENSION_MANAGER_H
#define _DYNAMIK_RENDERER_VULKAN_EXTENSION_MANAGER_H

#include "Public/Array.h"

namespace Dynamik {
	namespace Renderer {
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

#endif // !_DYNAMIK_RENDERER_VULKAN_EXTENSION_H
