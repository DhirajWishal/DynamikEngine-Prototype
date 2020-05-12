#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_DESCRIPTOR_H
#define _DYNAMIK_RENDERER_VULKAN_DESCRIPTOR_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanDescriptorSetLayoutInitInfo {
				std::vector<VkDescriptorSetLayoutBinding> bindings;
			};

			struct VulkanDescriptorPoolInitInfo {
				std::vector<VkDescriptorPoolSize> poolSizes;
				UI32 maxDescriptorSets = 1;
			};

			struct VulkanDescriptorSetsInitInfo {
				std::vector<VkWriteDescriptorSet> descriptorWrites;
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
				virtual void updateSet(VkDevice logicalDevice, std::vector<VkWriteDescriptorSet> writes);

				virtual void terminate(VkDevice logicalDevice);

				VkDescriptorSetLayout layout = VK_NULL_HANDLE;
				VkDescriptorPool pool = VK_NULL_HANDLE;
				std::vector<VkDescriptorSet> descriptorSets;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_DESCRIPTOR_H
