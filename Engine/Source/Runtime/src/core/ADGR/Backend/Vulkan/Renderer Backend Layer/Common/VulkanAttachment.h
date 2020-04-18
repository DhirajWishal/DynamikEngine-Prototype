#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_ATTACHMENT_H
#define _DYNAMIK_ADGR_VULKAN_ATTACHMENT_H

#include <vulkan/vulkan.h>
#include <DataTypesLib/DataTypesLib.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanAttachmentInitInfo {
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue processQueue = VK_NULL_HANDLE;	// graphics/ compute queue
				VkQueue utilityQueue = VK_NULL_HANDLE;	// present/ transfer queue
			};

			enum class ADGRVulkanAttachmentType {
				ADGR_VULKAN_ATTACHMENT_TYPE_UNKNOWN,
				ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_DESCRIPTOR,
				ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_TEXTURE_2D,
				ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_TEXTURE_3D,
				ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_TEXTURE_CUBEMAP,
				ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_UNIFORM_BUFFER,
			};

			class VulkanAttachment {
			public:
				VulkanAttachment() {}
				VulkanAttachment(ADGRVulkanAttachmentInitInfo info) : logicalDevice(info.logicalDevice),
					physicalDevice(info.physicalDevice), commandPool(info.commandPool),
				processQueue(info.processQueue), utilityQueue(info.utilityQueue){}
				virtual ~VulkanAttachment() {}

				virtual void initialize() {}
				virtual void terminate() {}

				ADGRVulkanAttachmentType type = ADGRVulkanAttachmentType::ADGR_VULKAN_ATTACHMENT_TYPE_UNKNOWN;

			protected:
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue processQueue = VK_NULL_HANDLE;	// graphics/ compute queue
				VkQueue utilityQueue = VK_NULL_HANDLE;	// present/ transfer queue
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_ATTACHMENT_H
