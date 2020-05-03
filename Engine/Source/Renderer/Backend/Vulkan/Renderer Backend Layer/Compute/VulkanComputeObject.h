#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_COMPUTE_OBJECT_H
#define _DYNAMIK_RENDERER_VULKAN_COMPUTE_OBJECT_H

#include "VulkanComputePipeline.h"
#include "VulkanComputeDescriptor.h"

#include "../Graphics/VulkanGraphicsContainers.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanComputeData {
				VulkanComputeDescriptor computeDescriptor;
				VulkanComputePipeline computePipeline;
				UI32 groupCountX = 0;
				UI32 groupCountY = 0;
				UI32 groupCountZ = 0;
			};

			struct VulkanComputeObjectInitInfo {
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkQueue computeQueue = VK_NULL_HANDLE;
				UI32 queueFamilyIndex = 0;
				VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
				VkCommandPool commandPool = VK_NULL_HANDLE;
			};

			struct VulkanComputeInputContainer {
				VulkanComputeInputContainer() {}
				virtual ~VulkanComputeInputContainer() {}

				VkBuffer dataBuffer = VK_NULL_HANDLE;
				VkDeviceMemory dataBufferMemory = VK_NULL_HANDLE;
			};

			struct VulkanComputeBufferInputContainer : public VulkanComputeInputContainer {
				VulkanComputeBufferInputContainer() {}
				~VulkanComputeBufferInputContainer() {}
			};

			struct VulkanComputeImageInputContainer : public VulkanComputeInputContainer {
				VulkanComputeImageInputContainer() {}
				~VulkanComputeImageInputContainer() {}

				VulkanTextureContainer inputTextureData;
			};

			class VulkanComputeObject {
			public:
				VulkanComputeObject() {}
				VulkanComputeObject(VulkanComputeObjectInitInfo info) :
					logicalDevice(info.logicalDevice), computeQueue(info.computeQueue),
					queueFamilyIndex(info.queueFamilyIndex),
					physicalDeviceMemoryProperties(info.physicalDeviceMemoryProperties),
					physicalDevice(info.physicalDevice), commandPool(info.commandPool) {}
				virtual ~VulkanComputeObject() {}

				virtual void initialize();
				virtual void initialize(VulkanComputeInputContainer container);
				virtual void initialize(VulkanComputeBufferInputContainer container);
				virtual void initialize(VulkanComputeImageInputContainer container);
				virtual void terminate();

				VulkanComputeData myComputeData;

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

#endif // !_DYNAMIK_RENDERER_VULKAN_COMPUTE_OBJECT_H
