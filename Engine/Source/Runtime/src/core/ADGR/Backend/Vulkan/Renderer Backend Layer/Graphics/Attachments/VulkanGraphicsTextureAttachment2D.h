#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_TEXTURE_ATTACHMENT_2D_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_TEXTURE_ATTACHMENT_2D_H

#include "VulkanGraphicsTextureAttachment.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanGraphicsTextureAttachment2D : public VulkanGraphicsTextureAttachment {
			public:
				VulkanGraphicsTextureAttachment2D() {}
				VulkanGraphicsTextureAttachment2D(ADGRVulkanAttachmentInitInfo info) : VulkanGraphicsTextureAttachment(info) {}
				virtual ~VulkanGraphicsTextureAttachment2D() {}

				virtual void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos) override;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_GRAPHICS_TEXTURE_ATTACHMENT_2D_H
