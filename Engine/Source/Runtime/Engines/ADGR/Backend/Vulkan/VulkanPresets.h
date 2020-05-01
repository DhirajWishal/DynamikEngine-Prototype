#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_PRESETS_H
#define _DYNAMIK_ADGR_VULKAN_PRESETS_H

#include "Graphics/VulkanGraphicsCore.h"
#include "Graphics/VulkanGraphicsPrimitiveManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			/* Vulkan Preset Functional class
			 * This class contains preset initialization containers for some Vulkan functions.
			 */
			class VulkanPresets {
				VulkanPresets() {}
				~VulkanPresets() {}

			public:
				/* Render pass presets */
				static ADGRVulkanRenderPassInitInfo renderPassPreset3D(VkFormat swapChainImageFormat, VkFormat depthFormat, VkSampleCountFlagBits msaaFlags);
				static ADGRVulkanRenderPassInitInfo renderPassPresetOverlay(VkFormat swapChainImageFormat, VkFormat depthFormat);

				/* Frame buffer presets */
				static ADGRVulkanGraphicsFrameBufferInitInfo frameBufferPreset3D(VkImageView colorImage, VkImageView depthImage, ARRAY<VkImageView> swapChainImageViews, VkExtent2D bufferExtent, UI32 bufferCount);

				/* Pipeline presets */
				static ADGRVulkanGraphicsPipelineInitInfo pipelinePreset3D(ARRAY<DMKVertexAttribute> attributes, ARRAY<Shader> shaders);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_PRESETS_H
