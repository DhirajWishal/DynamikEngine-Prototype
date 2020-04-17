#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_ATTACHMENT_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_ATTACHMENT_H

#include "core/vulkan/Renderer Backend Layer/Common/VulkanAttachment.h"
#include "core/vulkan/Renderer Backend Layer/Graphics/VulkanGraphicsDescriptor.h"
#include "core/vulkan/Renderer Backend Layer/Graphics/VulkanGraphicsPipeline.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanGraphicsAttachment : public VulkanAttachment {
			public:
				VulkanGraphicsAttachment() {}
				VulkanGraphicsAttachment(ADGRVulkanAttachmentInitInfo info) : VulkanAttachment(info) {}
				virtual ~VulkanGraphicsAttachment() {}

				virtual void initializeDescriptor(UI32 destination = 0) {}
				virtual void terminateDescriptor() {}

				VulkanGraphicsDescriptor descriptor;

			protected:
				virtual void _initializeDescriptorLayout() {}
				virtual void _initializeDescriptorPool() {}
				virtual void _initializeDescriptorSets(UI32 destination = 0) {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_GRAPHICS_ATTACHMENT_H
