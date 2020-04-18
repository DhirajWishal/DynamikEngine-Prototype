#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_TEXTURE_ATTACHMENT_CUBE_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_TEXTURE_ATTACHMENT_CUBE_H

#include "VulkanGraphicsTextureAttachment.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanGraphicsTextureAttachmentCube : public VulkanGraphicsTextureAttachment {
			public:
				VulkanGraphicsTextureAttachmentCube();
				VulkanGraphicsTextureAttachmentCube(ADGRVulkanAttachmentInitInfo info);
				virtual ~VulkanGraphicsTextureAttachmentCube() {}

				virtual void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos) override;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_GRAPHICS_TEXTURE_ATTACHMENT_CUBE_H
