#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_DESCRIPTOR_H
#define _DYNAMIK_ADGR_VULKAN_DESCRIPTOR_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanDescriptorSetLayoutInitInfo {
				ARRAY<VkDescriptorSetLayoutBinding> bindings;
			};

			struct ADGRVulkanDescriptorPoolInitInfo {
				ARRAY<VkDescriptorPoolSize> poolSizes;
				UI32 maxDescriptorSets = 1;
			};

			struct ADGRVulkanDescriptorSetsInitInfo {
				ARRAY<VkWriteDescriptorSet> descriptorWrites;
			};

			class VulkanDescriptor {
			public:
				VulkanDescriptor() {}
				virtual ~VulkanDescriptor() {}

				virtual void initializeLayout(VkDevice logicalDevice, ADGRVulkanDescriptorSetLayoutInitInfo info);
				virtual void initializePool(VkDevice logicalDevice, ADGRVulkanDescriptorPoolInitInfo info);
				virtual void initializeSets(VkDevice logicalDevice, ADGRVulkanDescriptorSetsInitInfo info);

				virtual void terminate(VkDevice logicalDevice);

				VkDescriptorSetLayout layout = VK_NULL_HANDLE;
				VkDescriptorPool pool = VK_NULL_HANDLE;
				VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_DESCRIPTOR_H
