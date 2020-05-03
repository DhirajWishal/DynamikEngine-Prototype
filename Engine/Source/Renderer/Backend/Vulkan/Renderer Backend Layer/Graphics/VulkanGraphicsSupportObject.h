#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_GRAPHICS_SUPPORT_OBJECT_H
#define _DYNAMIK_RENDERER_VULKAN_GRAPHICS_SUPPORT_OBJECT_H

#include "VulkanGraphicsPipeline.h"
#include "VulkanGraphicsDescriptor.h"
#include "VulkanGraphicsFrameBuffer.h"
#include "VulkanGraphicsContainers.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanGraphicsSupportObjectInitInfo {
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;
			};

			struct VulkanGraphicsOffscreenFrameBufferContainer {
				VkImage image = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkFramebuffer frameBuffer = VK_NULL_HANDLE;
			};

			class VulkanGraphicsSupportObject {
			public:
				VulkanGraphicsSupportObject() {}
				VulkanGraphicsSupportObject(VulkanGraphicsSupportObjectInitInfo info) :
					logicalDevice(info.logicalDevice), physicalDevice(info.physicalDevice),
					commandPool(info.commandPool), graphicsQueue(info.graphicsQueue),
					presentQueue(info.presentQueue) {}
				virtual ~VulkanGraphicsSupportObject() {}

				void setResources(VulkanGraphicsSupportObjectInitInfo info)
				{
					logicalDevice = info.logicalDevice;
					physicalDevice = info.physicalDevice;
					commandPool = info.commandPool;
					graphicsQueue = info.graphicsQueue;
					presentQueue = info.presentQueue;
				}

				virtual void initialize()
				{
					_initializeTexture();
					_initializeRenderPass();
					_initializeFrameBuffer();
					_initializeDescriptorSetLayout();
					_initializeDescriptorPool();
					_initializeDescriptorSets();
					_initializePipelineLayout();
					_initializePipeline();
					_initializeCommandBuffers();
				}
				virtual void terminate() {}

				VulkanTextureContainer myTextureContainer;
				VulkanGraphicsDescriptor myDescriptorContainer;
				VulkanGraphicsFrameBuffer myFrameBuffer;
				VulkanGraphicsPipeline myPipeline;

			protected:
				virtual void _initializeTexture() {}
				virtual void _initializeRenderPass() {}
				virtual void _initializeFrameBuffer() {}
				virtual void _initializeOffscreenFrameBuffer() {}
				virtual void _initializeDescriptorSetLayout() {}
				virtual void _initializeDescriptorPool() {}
				virtual void _initializeDescriptorSets() {}
				virtual void _initializePipelineLayout() {}
				virtual void _initializePipeline() {}
				virtual void _initializeCommandBuffers() {}

				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_GRAPHICS_SUPPORT_OBJECT_H
