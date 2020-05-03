#include "dmkafx.h"
#include "VulkanPresets.h"

#include "Renderer Backend Layer/Common/VulkanUtilities.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			ADGRVulkanGraphicsPipelineLayoutInitInfo VulkanPresets::pipelineLayoutPreset(ARRAY<VkDescriptorSetLayout> layouts, ARRAY<VkPushConstantRange> pushConstants)
			{
				ADGRVulkanGraphicsPipelineLayoutInitInfo _info;
				_info.layouts = layouts;
				_info.pushConstantRanges = pushConstants;
				return _info;
			}
			
			ADGRVulkanGraphicsPipelineInitInfo VulkanPresets::pipelinePreset3D(VkDevice logicalDevice, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, ARRAY<VulkanGraphicsShader> shaders, ARRAY<DMKVertexAttribute> vertexAttributes, VkExtent2D extent)
			{
				ADGRVulkanGraphicsPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				pipelineInitInfo.vertexBindingDescription = VulkanUtilities::getBindingDescription(vertexAttributes, 1);
				pipelineInitInfo.vertexAttributeDescription = VulkanUtilities::getAttributeDescriptions(vertexAttributes);
				pipelineInitInfo.isTexturesAvailable = true;
				pipelineInitInfo.rasterizerFrontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
				pipelineInitInfo.renderPass = renderPass;
				pipelineInitInfo.swapChainExtent = extent;
				return pipelineInitInfo;
			}

			ADGRVulkanGraphicsPipelineInitInfo VulkanPresets::pipelinePresetSkyBox(VkDevice logicalDevice, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, ARRAY<VulkanGraphicsShader> shaders, ARRAY<DMKVertexAttribute> vertexAttributes, VkExtent2D extent)
			{
				ADGRVulkanGraphicsPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				pipelineInitInfo.vertexBindingDescription = VulkanUtilities::getBindingDescription(vertexAttributes, 1);
				pipelineInitInfo.vertexAttributeDescription = VulkanUtilities::getAttributeDescriptions(vertexAttributes);
				pipelineInitInfo.isTexturesAvailable = true;
				pipelineInitInfo.rasterizerFrontFace = VK_FRONT_FACE_CLOCKWISE;
				pipelineInitInfo.renderPass = renderPass;
				pipelineInitInfo.swapChainExtent = extent;
				return pipelineInitInfo;
			}
		}
	}
}