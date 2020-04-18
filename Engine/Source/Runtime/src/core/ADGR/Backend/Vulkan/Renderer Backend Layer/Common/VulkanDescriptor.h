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
				UI32 setCount = 1;
			};

			class VulkanDescriptor {
			public:
				VulkanDescriptor() {}
				virtual ~VulkanDescriptor() {}

				virtual void initializeLayout(VkDevice logicalDevice, ADGRVulkanDescriptorSetLayoutInitInfo info);
				virtual void initializePool(VkDevice logicalDevice, ADGRVulkanDescriptorPoolInitInfo info);
				virtual void allocateSets(VkDevice logicalDevice, UI32 setCount = 1);
				virtual void initializeSets(VkDevice logicalDevice, ADGRVulkanDescriptorSetsInitInfo info);
				virtual void updateSet(VkDevice logicalDevice, ARRAY<VkWriteDescriptorSet> writes);

				virtual void terminate(VkDevice logicalDevice);

				VkDescriptorSetLayout layout = VK_NULL_HANDLE;
				VkDescriptorPool pool = VK_NULL_HANDLE;
				ARRAY<VkDescriptorSet> descriptorSets;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_DESCRIPTOR_H
