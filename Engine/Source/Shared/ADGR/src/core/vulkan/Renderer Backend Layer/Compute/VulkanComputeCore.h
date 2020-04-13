#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_CORE_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_CORE_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanComputeShaderPhysicalDeviceLimits {
				ADGRVulkanComputeShaderPhysicalDeviceLimits() {}
				ADGRVulkanComputeShaderPhysicalDeviceLimits(VkPhysicalDeviceProperties props)
				{
					maxSharedMemorySize = props.limits.maxComputeSharedMemorySize;

					maxWorkGroupCount[0] = props.limits.maxComputeWorkGroupCount[0];
					maxWorkGroupCount[1] = props.limits.maxComputeWorkGroupCount[1];
					maxWorkGroupCount[2] = props.limits.maxComputeWorkGroupCount[2];

					maxWorkGroupInvocations = props.limits.maxComputeWorkGroupInvocations;

					maxWorkGroupSize[0] = props.limits.maxComputeWorkGroupSize[0];
					maxWorkGroupSize[1] = props.limits.maxComputeWorkGroupSize[1];
					maxWorkGroupSize[2] = props.limits.maxComputeWorkGroupSize[2];
				}
				~ADGRVulkanComputeShaderPhysicalDeviceLimits() {}

				UI32 maxSharedMemorySize = 0;
				UI32 maxWorkGroupCount[3] = { 0 };
				UI32 maxWorkGroupInvocations = 0;
				UI32 maxWorkGroupSize[3] = { 0 };
			};

			struct ADGRVulkanComputeQueue {
				std::optional<UI32> transferFamily;
				std::optional<UI32> computeFamily;

				B1 isComplete();
			};

			class VulkanComputeCore {
			public:
				VulkanComputeCore() {}
				virtual ~VulkanComputeCore() {}

				void initializeInstance();
				void initializeDevice();

				void submitQueue(ARRAY<VkCommandBuffer> commandBuffers);
				void finishCompute();

				static UI32 getBestTransferQueue(VkPhysicalDevice physicalDevice);
				static UI32 getBestComputeQueue(VkPhysicalDevice physicalDevice);

				VkInstance instance = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkDevice logicalDevice = VK_NULL_HANDLE;
				UI32 queueFamilyIndex = 0;
				VkQueue computeQueue;

				ADGRVulkanComputeShaderPhysicalDeviceLimits physicalDeviceLimits;
				VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;

			private:
				void initializePhysicalDevice();
				void initializeLogicalDevice();
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_CORE_H
