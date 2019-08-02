#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

namespace Dynamik {
	namespace ADGR {
		namespace core {

			// Graphics pipeline class
			class pipeline {
			public:
				pipeline(VkDevice* device, VkRenderPass* renderPass, VkPipeline* graphicsPipeline);
				// initialize the render pass
				void initRenderPass(VkFormat swapChainImageFormat);

				// initialize the shaders
				void initShaders(std::vector<char> vertShaderCode, std::vector<char> fragShaderCode);

				// initialize the pipeline
				void initPipeline(VkExtent2D swapChainExtent, VkPipelineLayout* pipelineLayout,
					VkDescriptorSetLayout* descriptorSetLayout);

				// delete the shaders
				void deleteShaders();

			private:
				VkDevice* myDevice;
				VkRenderPass* myRenderPass;
				VkPipeline* myGraphicsPipeline;

				VkShaderModule createShaderModule(VkDevice& device, const std::vector<char>& code);
				inline VkPipelineShaderStageCreateInfo getFragShaderStageInfo();
				inline VkPipelineShaderStageCreateInfo getVertShaderStageInfo();

				VkDynamicState dynamicStates[2] = {
					VK_DYNAMIC_STATE_VIEWPORT,
					VK_DYNAMIC_STATE_LINE_WIDTH
				};

				VkShaderModule vertShaderModule = {};
				VkShaderModule fragShaderModule = {};

				VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
				VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};

				static VkShaderModuleCreateInfo createInfo;
				static VkShaderModule shaderModule;
			};
		}
	}
}
