#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_PRESETS_H
#define _DYNAMIK_ADGR_VULKAN_PRESETS_H

#include "Renderer Backend Layer/Graphics/VulkanGraphicsPipeline.h"
#include "Renderer Backend Layer/Graphics/VulkanGraphicsFrameBuffer.h"
#include "Objects/InternalFormat/InternalFormat.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanPresets {
				VulkanPresets() {}
				~VulkanPresets() {}

			public:
				/* Pipeline presets */
				static ADGRVulkanGraphicsPipelineLayoutInitInfo pipelineLayoutPreset(ARRAY<VkDescriptorSetLayout> layouts, ARRAY<VkPushConstantRange> pushConstants = {});
				static ADGRVulkanGraphicsPipelineInitInfo pipelinePreset3D(VkDevice logicalDevice, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, ARRAY<VulkanGraphicsShader> shaders, ARRAY<DMKVertexAttribute> vertexAttributes, VkExtent2D extent);
				static ADGRVulkanGraphicsPipelineInitInfo pipelinePresetSkyBox(VkDevice logicalDevice, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, ARRAY<VulkanGraphicsShader> shaders, ARRAY<DMKVertexAttribute> vertexAttributes, VkExtent2D extent);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_PRESETS_H
