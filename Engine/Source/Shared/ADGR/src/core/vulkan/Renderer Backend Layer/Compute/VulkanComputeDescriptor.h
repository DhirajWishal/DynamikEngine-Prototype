#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_DESCRIPTOR_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_DESCRIPTOR_H

#include "../Objects/VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanComputeDescriptor {
			public:
				VulkanComputeDescriptor() {}
				virtual ~VulkanComputeDescriptor() {}

				virtual void initializeDescriptorSetLayout(VkDevice device, ADGRVulkanDescriptorSetLayoutInitInfo info);
				virtual void initializeDescriptorPool(VkDevice device, ADGRVulkanDescriptorPoolInitInfo info);
				virtual void initializeDescriptorSet(VkDevice device, ADGRVulkanDescriptorSetsInitInfo info);

				ADGRVulkanDescrpitorContainer container;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_DESCRIPTOR_H
