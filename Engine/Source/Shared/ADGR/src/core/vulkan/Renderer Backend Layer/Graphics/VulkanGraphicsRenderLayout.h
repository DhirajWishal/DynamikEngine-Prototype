#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_RENDER_LAYOUT_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_RENDER_LAYOUT_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanDescriptorSetLayoutInitInfo {
				ARRAY<VkDescriptorSetLayoutBinding> additionalBindings;
				B1 overrideBindings = false;
			};

			class VulkanGraphicsRenderLayout {
				VulkanGraphicsRenderLayout(const VulkanGraphicsRenderLayout&) = delete;
				VulkanGraphicsRenderLayout(VulkanGraphicsRenderLayout&&) = delete;

			public:
				static VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device, ADGRVulkanDescriptorSetLayoutInitInfo info);
				static void terminateDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout layout);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDER_LAYOUT_H
