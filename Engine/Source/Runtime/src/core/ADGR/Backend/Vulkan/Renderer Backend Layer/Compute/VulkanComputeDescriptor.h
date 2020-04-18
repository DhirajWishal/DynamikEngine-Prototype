#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_DESCRIPTOR_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_DESCRIPTOR_H

#include "../Graphics/VulkanGraphicsRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVUlkanComputeDescriptorContainer {
				VkDescriptorSetLayout layout = VK_NULL_HANDLE;
				VkDescriptorPool pool = VK_NULL_HANDLE;
				ARRAY<VkDescriptorSet> descriptorSets = VK_NULL_HANDLE;
			};

			class VulkanComputeDescriptor {
			public:
				VulkanComputeDescriptor() {}
				virtual ~VulkanComputeDescriptor() {}

				virtual void initializeDescriptorSetLayout(VkDevice device, ADGRVulkanDescriptorSetLayoutInitInfo info);
				virtual void initializeDescriptorPool(VkDevice device, ADGRVulkanDescriptorPoolInitInfo info);
				virtual void initializeDescriptorSet(VkDevice device, ADGRVulkanDescriptorSetsInitInfo info);

				ADGRVUlkanComputeDescriptorContainer container;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_DESCRIPTOR_H
