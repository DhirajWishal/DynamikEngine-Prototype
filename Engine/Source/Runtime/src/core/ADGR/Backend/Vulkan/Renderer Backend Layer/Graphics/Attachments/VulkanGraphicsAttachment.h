#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_ATTACHMENT_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_ATTACHMENT_H

#include "core/ADGR/Backend/Vulkan/Renderer Backend Layer/Common/VulkanAttachment.h"
#include "core/ADGR/Backend/Vulkan/Renderer Backend Layer/Graphics/VulkanGraphicsDescriptor.h"
#include "core/ADGR/Backend/Vulkan/Renderer Backend Layer/Graphics/VulkanGraphicsPipeline.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanGraphicsAttachment : public VulkanAttachment {
			public:
				VulkanGraphicsAttachment() { type = ADGRVulkanAttachmentType::ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_DESCRIPTOR; }
				VulkanGraphicsAttachment(ADGRVulkanAttachmentInitInfo info) : VulkanAttachment(info)
				{
					type = ADGRVulkanAttachmentType::ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_DESCRIPTOR;
				}
				virtual ~VulkanGraphicsAttachment() {}

				virtual void initializeDescriptor(UI32 destination, VkShaderStageFlags shaderStage) {}
				virtual void terminateDescriptor() {}

				VulkanGraphicsDescriptor descriptor;

			protected:
				virtual void _initializeDescriptorLayout(UI32 destination, VkDescriptorType descriptorType, VkShaderStageFlags shaderStage);
				virtual void _initializeDescriptorPool() {}
				virtual void _initializeDescriptorSets(UI32 destination = 0) {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_GRAPHICS_ATTACHMENT_H
