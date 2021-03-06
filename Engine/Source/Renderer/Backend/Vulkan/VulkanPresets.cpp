#include "dmkafx.h"
#include "VulkanPresets.h"

#include "Renderer Backend Layer/Common/VulkanUtilities.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			VulkanGraphicsPipelineLayoutInitInfo VulkanPresets::pipelineLayoutPreset(std::vector<VkDescriptorSetLayout> layouts, std::vector<VkPushConstantRange> pushConstants)
			{
				VulkanGraphicsPipelineLayoutInitInfo _info;
				_info.layouts = layouts;
				_info.pushConstantRanges = pushConstants;
				return _info;
			}

			VulkanGraphicsPipelineInitInfo VulkanPresets::pipelinePreset3D(VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, std::vector<VulkanGraphicsShader> shaders, std::vector<DMKVertexAttribute> vertexAttributes, VkExtent2D extent)
			{
				VulkanGraphicsPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				if (vertexAttributes.size())
				{
					pipelineInitInfo.vertexBindingDescription = VulkanUtilities::getBindingDescription(vertexAttributes, 1);
					pipelineInitInfo.vertexAttributeDescription = VulkanUtilities::getAttributeDescriptions(vertexAttributes);
				}
				pipelineInitInfo.isTexturesAvailable = true;
				pipelineInitInfo.renderPass = renderPass;
				pipelineInitInfo.swapChainExtent = extent;
				return pipelineInitInfo;
			}

			VulkanGraphicsPipelineInitInfo VulkanPresets::pipelinePresetSkyBox(VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, std::vector<VulkanGraphicsShader> shaders, std::vector<DMKVertexAttribute> vertexAttributes, VkExtent2D extent)
			{
				VulkanGraphicsPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				if (vertexAttributes.size())
				{
					pipelineInitInfo.vertexBindingDescription = VulkanUtilities::getBindingDescription(vertexAttributes, 1);
					pipelineInitInfo.vertexAttributeDescription = VulkanUtilities::getAttributeDescriptions(vertexAttributes);
				}
				pipelineInitInfo.isTexturesAvailable = true;
				pipelineInitInfo.rasterizerFrontFace = VK_FRONT_FACE_CLOCKWISE;
				pipelineInitInfo.renderPass = renderPass;
				pipelineInitInfo.swapChainExtent = extent;
				return pipelineInitInfo;
			}

			VulkanGraphicsPipelineInitInfo VulkanPresets::pipelinePresetBoundingBox(VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, std::vector<VulkanGraphicsShader> shaders, std::vector<DMKVertexAttribute> vertexAttributes, VkExtent2D extent)
			{
				VulkanGraphicsPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				if (vertexAttributes.size())
				{
					pipelineInitInfo.vertexBindingDescription = VulkanUtilities::getBindingDescription(vertexAttributes, 1);
					pipelineInitInfo.vertexAttributeDescription = VulkanUtilities::getAttributeDescriptions(vertexAttributes);
				}
				pipelineInitInfo.isTexturesAvailable = true;
				pipelineInitInfo.renderPass = renderPass;
				pipelineInitInfo.swapChainExtent = extent;
				pipelineInitInfo.rasterizerPolygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;
				pipelineInitInfo.inputAssemblyTopology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
				return pipelineInitInfo;
			}
		}
	}
}