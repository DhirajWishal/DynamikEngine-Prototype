#pragma once

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct VulkanData {
				VkInstance instance;
				VkDevice device;
				VkPhysicalDevice physicalDevice;

				VkExtent2D swapChainExtent;

				VulkanData(
					VkInstance ins = VK_NULL_HANDLE,
					VkDevice dev = VK_NULL_HANDLE,
					VkPhysicalDevice pd = VK_NULL_HANDLE,

					VkExtent2D ext = { VK_NULL_HANDLE, VK_NULL_HANDLE }
				) : instance(ins), device(dev), physicalDevice(pd),
					swapChainExtent(ext)
				{}
			};
		}
	}
}