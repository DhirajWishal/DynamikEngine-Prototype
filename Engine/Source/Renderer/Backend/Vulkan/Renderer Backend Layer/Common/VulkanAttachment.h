#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_ATTACHMENT_H
#define _DYNAMIK_RENDERER_VULKAN_ATTACHMENT_H

#include <vulkan/vulkan.h>
#include <DataTypesLib.h>

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanAttachmentInitInfo {
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue processQueue = VK_NULL_HANDLE;	// graphics/ compute queue
				VkQueue utilityQueue = VK_NULL_HANDLE;	// present/ transfer queue
			};

			enum class VulkanAttachmentType {
				ADGR_ATTACHMENT_TYPE_UNKNOWN,
				ADGR_ATTACHMENT_TYPE_TEXTURE_2D,
				ADGR_ATTACHMENT_TYPE_TEXTURE_3D,
				ADGR_ATTACHMENT_TYPE_TEXTURE_CUBEMAP,
			};

			class VulkanAttachment {
			public:
				VulkanAttachment() {}
				VulkanAttachment(VulkanAttachmentInitInfo info) : logicalDevice(info.logicalDevice),
					physicalDevice(info.physicalDevice), commandPool(info.commandPool),
				processQueue(info.processQueue), utilityQueue(info.utilityQueue){}
				virtual ~VulkanAttachment() {}

				virtual void initialize() {}
				virtual void terminate() {}

			protected:
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue processQueue = VK_NULL_HANDLE;	// graphics/ compute queue
				VkQueue utilityQueue = VK_NULL_HANDLE;	// present/ transfer queue

				VulkanAttachmentType type = VulkanAttachmentType::ADGR_ATTACHMENT_TYPE_UNKNOWN;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_ATTACHMENT_H
