#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_BIDIRECTIONAL_REFLECTANCE_DESTRIBUTION_FUNCTION_H
#define _DYNAMIK_ADGR_VULKAN_BIDIRECTIONAL_REFLECTANCE_DESTRIBUTION_FUNCTION_H

#include "Graphics/VulkanGraphicsRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanBRDFInitInfo {
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;
			};

			class VulkanBRDF {
			public:
				VulkanBRDF() {}
				VulkanBRDF(ADGRVulkanBRDFInitInfo info) :
					logicalDevice(info.logicalDevice), physicalDevice(info.physicalDevice),
					commandPool(info.commandPool), graphicsQueue(info.graphicsQueue),
					presentQueue(info.presentQueue) {}
				virtual ~VulkanBRDF() {}

				virtual void initialize();

				ADGRVulkanTextureContainer myTextureContainer;
				VulkanGraphicsDescriptor myDescriptorContainer;
				VulkanGraphicsFrameBuffer myFrameBuffer;
				VulkanGraphicsPipeline myPipeline;

				UI32 dimentions = 512;

			protected:
				virtual void _initializeTexture();
				virtual void _initializeRenderPass();
				virtual void _initializeFrameBuffer();
				virtual void _initializeDescriptorSetLayout();
				virtual void _initializeDescriptorPool();

				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_BIDIRECTIONAL_REFLECTANCE_DESTRIBUTION_FUNCTION_H
