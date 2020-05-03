#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_DESCRIPTOR_H
#define _DYNAMIK_RENDERER_VULKAN_DESCRIPTOR_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanDescriptorSetLayoutInitInfo {
				ARRAY<VkDescriptorSetLayoutBinding> bindings;
			};

			struct VulkanDescriptorPoolInitInfo {
				ARRAY<VkDescriptorPoolSize> poolSizes;
				UI32 maxDescriptorSets = 1;
			};

			struct VulkanDescriptorSetsInitInfo {
				ARRAY<VkWriteDescriptorSet> descriptorWrites;
				UI32 setCount = 1;
			};

			class VulkanDescriptor {
			public:
				VulkanDescriptor() {}
				virtual ~VulkanDescriptor() {}

				virtual void initializeLayout(VkDevice logicalDevice, VulkanDescriptorSetLayoutInitInfo info);
				virtual void initializePool(VkDevice logicalDevice, VulkanDescriptorPoolInitInfo info);
				virtual void allocateSets(VkDevice logicalDevice, UI32 setCount = 1);
				virtual void initializeSets(VkDevice logicalDevice, VulkanDescriptorSetsInitInfo info);
				virtual void updateSet(VkDevice logicalDevice, ARRAY<VkWriteDescriptorSet> writes);

				virtual void terminate(VkDevice logicalDevice);

				VkDescriptorSetLayout layout = VK_NULL_HANDLE;
				VkDescriptorPool pool = VK_NULL_HANDLE;
				ARRAY<VkDescriptorSet> descriptorSets;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_DESCRIPTOR_H
