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
				static VulkanGraphicsPipelineLayoutInitInfo pipelineLayoutPreset(std::vector<VkDescriptorSetLayout> layouts, std::vector<VkPushConstantRange> pushConstants = {});
				static VulkanGraphicsPipelineInitInfo pipelinePreset3D(VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, std::vector<VulkanGraphicsShader> shaders, std::vector<DMKVertexAttribute> vertexAttributes, VkExtent2D extent);
				static VulkanGraphicsPipelineInitInfo pipelinePresetSkyBox(VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, std::vector<VulkanGraphicsShader> shaders, std::vector<DMKVertexAttribute> vertexAttributes, VkExtent2D extent);
				static VulkanGraphicsPipelineInitInfo pipelinePresetBoundingBox(VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, std::vector<VulkanGraphicsShader> shaders, std::vector<DMKVertexAttribute> vertexAttributes, VkExtent2D extent);
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_PRESETS_H
