#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_UNIFORM_BUFFER_ATTACHMENT_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_UNIFORM_BUFFER_ATTACHMENT_H

#include "VulkanGraphicsAttachment.h"
#include "../VulkanGraphicsFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanUnformBufferContainer {
				ARRAY<VkBuffer> buffers;
				ARRAY<VkDeviceMemory> bufferMemories;
			};

			class VulkanGraphicsUniformBufferAttachment : public VulkanGraphicsAttachment {
			public:
				VulkanGraphicsUniformBufferAttachment();
				VulkanGraphicsUniformBufferAttachment(ADGRVulkanAttachmentInitInfo info);
				~VulkanGraphicsUniformBufferAttachment() {}

				void initializeDescriptor(UI32 destination, VkShaderStageFlags shaderStage) override;
				void initializeUniformBuffer(VkDeviceSize bufferSize, UI32 count, UI32 offset = 0);
				void updateUniformBuffer(VPTR data, UI32 imageIndex);
				void terminate() override;

				ADGRVulkanUnformBufferContainer bufferContainer;

			private:
				VkDeviceSize bufferSize = 0;
				UI32 bufferOffset = 0;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_GRAPHICS_UNIFORM_BUFFER_ATTACHMENT_H.
