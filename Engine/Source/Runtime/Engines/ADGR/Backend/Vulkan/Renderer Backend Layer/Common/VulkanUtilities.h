#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_UTILITIES_H
#define _DYNAMIK_ADGR_VULKAN_UTILITIES_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanUtilities {
				VulkanUtilities(const VulkanUtilities&) = delete;
				VulkanUtilities(VulkanUtilities&&) = delete;
			public:
				~VulkanUtilities() {}

				static void displayVertexBufferContent(VkDevice logicalDevice, VkBuffer buffer, VkDeviceMemory bufferMemory);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_UTILITIES_H
