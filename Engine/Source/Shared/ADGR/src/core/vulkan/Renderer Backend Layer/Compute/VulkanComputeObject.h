#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_OBJECT_H

#include "VulkanComputePipeline.h"
#include "VulkanComputeDescriptor.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanComputeData {
				VulkanComputeDescriptor computeDescriptor;
				VulkanComputePipeline computePipeline;
				UI32 groupCountX = 0;
				UI32 groupCountY = 0;
				UI32 groupCountZ = 0;
			};

			struct ADGRVulkanComputeObjectInitInfo {
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkDevice logicalDevice = VK_NULL_HANDLE;
				UI32 queueFamilyIndex = 0;
				VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
			};

			class VulkanComputeObject {
			public:
				VulkanComputeObject() {}
				VulkanComputeObject(ADGRVulkanComputeObjectInitInfo info) :
					logicalDevice(info.logicalDevice), queueFamilyIndex(info.queueFamilyIndex),
					physicalDeviceMemoryProperties(info.physicalDeviceMemoryProperties),
					physicalDevice(info.physicalDevice) {}
				virtual ~VulkanComputeObject() {}

				virtual void initialize();

				ADGRVulkanComputeData myComputeData;

			private:
				VkBuffer inputBuffer = VK_NULL_HANDLE;
				VkDeviceMemory inputBufferMemory = VK_NULL_HANDLE;

				VkBuffer outputBuffer = VK_NULL_HANDLE;
				VkDeviceMemory outputBufferMemory = VK_NULL_HANDLE;

				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkDevice logicalDevice = VK_NULL_HANDLE;
				UI32 queueFamilyIndex = 0;
				VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_OBJECT_H
