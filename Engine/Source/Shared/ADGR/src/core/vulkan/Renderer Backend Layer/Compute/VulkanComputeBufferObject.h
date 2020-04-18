#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_BUFFER_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_BUFFER_OBJECT_H

#include "VulkanComputeObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanComputeBufferObject : public VulkanComputeObject {
			public:
				VulkanComputeBufferObject() {}
				VulkanComputeBufferObject(ADGRVulkanComputeObjectInitInfo info) : VulkanComputeObject(info) {}
				virtual ~VulkanComputeBufferObject() {}

				virtual void initialize() override;

			protected:
				virtual void _initializeDescriptorSetLayout() override;
				virtual void _initializePipelineLayout() override;
				virtual void _initializePipeline() override;
				virtual void _initializeDescriptorPool() override;
				virtual void _initializeDescriptorSets() override;

				VkBuffer inputBuffer = VK_NULL_HANDLE;
				VkDeviceMemory inputBufferMemory = VK_NULL_HANDLE;

				VkBuffer outputBuffer = VK_NULL_HANDLE;
				VkDeviceMemory outputBufferMemory = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_BUFFER_OBJECT_H
