#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_OBJECT_H

#include "VulkanComputePipeline.h"
#include "VulkanComputeDescriptor.h"

#include "../Graphics/VulkanGraphicsContainers.h"

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
				VkQueue computeQueue = VK_NULL_HANDLE;
				UI32 queueFamilyIndex = 0;
				VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
				VkCommandPool commandPool = VK_NULL_HANDLE;
			};

			struct ADGRVulkanComputeInputContainer {
				ADGRVulkanComputeInputContainer() {}
				virtual ~ADGRVulkanComputeInputContainer() {}

				VkBuffer dataBuffer = VK_NULL_HANDLE;
				VkDeviceMemory dataBufferMemory = VK_NULL_HANDLE;
			};

			struct ADGRVulkanComputeBufferInputContainer : public ADGRVulkanComputeInputContainer {
				ADGRVulkanComputeBufferInputContainer() {}
				~ADGRVulkanComputeBufferInputContainer() {}
			};

			struct ADGRVulkanComputeImageInputContainer : public ADGRVulkanComputeInputContainer {
				ADGRVulkanComputeImageInputContainer() {}
				~ADGRVulkanComputeImageInputContainer() {}

				ADGRVulkanTextureContainer inputTextureData;
			};

			class VulkanComputeObject {
			public:
				VulkanComputeObject() {}
				VulkanComputeObject(ADGRVulkanComputeObjectInitInfo info) :
					logicalDevice(info.logicalDevice), computeQueue(info.computeQueue),
					queueFamilyIndex(info.queueFamilyIndex),
					physicalDeviceMemoryProperties(info.physicalDeviceMemoryProperties),
					physicalDevice(info.physicalDevice), commandPool(info.commandPool) {}
				virtual ~VulkanComputeObject() {}

				virtual void initialize();
				virtual void initialize(ADGRVulkanComputeInputContainer container);
				virtual void initialize(ADGRVulkanComputeBufferInputContainer container);
				virtual void initialize(ADGRVulkanComputeImageInputContainer container);
				virtual void terminate();

				ADGRVulkanComputeData myComputeData;

			protected:
				virtual void _initializeDescriptorSetLayout() {}
				virtual void _initializePipelineLayout() {}
				virtual void _initializePipeline() {}
				virtual void _initializeDescriptorPool() {}
				virtual void _initializeDescriptorSets() {}

				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkQueue computeQueue = VK_NULL_HANDLE;
				UI32 queueFamilyIndex = 0;
				VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
				VkCommandPool commandPool = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_OBJECT_H
