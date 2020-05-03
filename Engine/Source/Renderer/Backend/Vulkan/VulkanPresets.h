#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_PRESETS_H
#define _DYNAMIK_RENDERER_VULKAN_PRESETS_H

#include "Renderer Backend Layer/Graphics/VulkanGraphicsPipeline.h"
#include "Renderer Backend Layer/Graphics/VulkanGraphicsFrameBuffer.h"
#include "Objects/InternalFormat/InternalFormat.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			class VulkanPresets {
				VulkanPresets() {}
				~VulkanPresets() {}

			public:
				/* Pipeline presets */
				static VulkanGraphicsPipelineLayoutInitInfo pipelineLayoutPreset(ARRAY<VkDescriptorSetLayout> layouts, ARRAY<VkPushConstantRange> pushConstants = {});
				static VulkanGraphicsPipelineInitInfo pipelinePreset3D(VkDevice logicalDevice, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, ARRAY<VulkanGraphicsShader> shaders, ARRAY<DMKVertexAttribute> vertexAttributes, VkExtent2D extent);
				static VulkanGraphicsPipelineInitInfo pipelinePresetSkyBox(VkDevice logicalDevice, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, ARRAY<VulkanGraphicsShader> shaders, ARRAY<DMKVertexAttribute> vertexAttributes, VkExtent2D extent);
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_PRESETS_H
