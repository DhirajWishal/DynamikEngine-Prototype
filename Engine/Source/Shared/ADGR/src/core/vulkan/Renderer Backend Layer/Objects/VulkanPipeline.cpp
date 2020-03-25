#include "adgrafx.h"
#include "VulkanPipeline.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanPipeline::initialize(ADGRVulkanPipelineInitInfo info)
			{
				auto bindingDescription = Vertex::getBindingDescription(1);
				auto attributeDescriptions = Vertex::getAttributeDescriptions(4);

				VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
				vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
				vertexInputInfo.vertexBindingDescriptionCount = static_cast<UI32>(bindingDescription.size());
				vertexInputInfo.vertexAttributeDescriptionCount = static_cast<UI32>(attributeDescriptions.size());
				vertexInputInfo.pVertexBindingDescriptions = bindingDescription.data();
				vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

				// initialize the input assembler
				VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
				inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
				inputAssembly.topology = info.inputAssemblyTopology;
				inputAssembly.primitiveRestartEnable = info.inputAssemblyPrimitiveRestartEnable;

				// initialize the viewport(s)
				ARRAY<VkViewport> viewports;
				for (I32 i = 0; i < info.viweportCount; i++) {
					VkViewport viewport = {};
					viewport.x = 0.0f;
					viewport.y = 0.0f;
					viewport.width = info.swapChain.mySwapChainExtent.width;
					viewport.height = info.swapChain.mySwapChainExtent.height;
					viewport.minDepth = 0.0f;
					viewport.maxDepth = 1.0f;

					viewports.push_back(viewport);
				}

				// initialize the scissor(s)
				ARRAY<VkRect2D> scissors;
				for (I32 i = 0; i < info.scissorCount; i++) {
					VkRect2D scissor = {};
					scissor.offset = info.offsets[0];
					scissor.extent = info.swapChain.mySwapChainExtent;

					scissors.push_back(scissor);
				}

				// initialize the viewport state
				VkPipelineViewportStateCreateInfo viewportState = {};
				viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
				viewportState.viewportCount = info.viweportCount;
				viewportState.pViewports = viewports.data();
				viewportState.scissorCount = info.scissorCount;
				viewportState.pScissors = scissors.data();

				// initialize the rasterizer
				VkPipelineRasterizationStateCreateInfo rasterizer = {};
				rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
				rasterizer.depthClampEnable = info.rasterizerDepthClampEnable;
				rasterizer.rasterizerDiscardEnable = info.rasterizerDiscardEnable;
				rasterizer.polygonMode = info.rasterizerPolygonMode;
				rasterizer.lineWidth = info.rasterizerLineWidth;
				rasterizer.cullMode = info.rasterizerCullMode;
				rasterizer.frontFace = info.rasterizerFrontFace;
				rasterizer.depthBiasEnable = info.rasterizerDepthBiasEnable;

				// initialize multisampling
				VkPipelineMultisampleStateCreateInfo multisampling = {};
				multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
				multisampling.rasterizationSamples = info.global->msaaSamples;
				multisampling.sampleShadingEnable = info.multisamplerSampleShadingEnable; // enable sample shading in the pipeline
				multisampling.minSampleShading = info.multisamplerMinSampleShading; // min fraction for sample shading; closer to one is smoother

				VkPipelineDepthStencilStateCreateInfo depthStencil = {};
				depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
				depthStencil.depthTestEnable = info.depthStencilEnable;
				depthStencil.depthWriteEnable = info.depthStencilWriteEnable;
				depthStencil.depthCompareOp = info.depthStencilCompareOp;
				depthStencil.depthBoundsTestEnable = info.depthStencilBoundsTestEnable;
				depthStencil.stencilTestEnable = info.depthStencilTestEnable;

				ARRAY<VkPipelineColorBlendAttachmentState> colorBlendAttachments;

				// initialize the color blender(s)
				for (I32 i = 0; i < info.colorBlendingColorBlendCount; i++) {
					VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
					colorBlendAttachment.colorWriteMask = info.colorBlenderColorWriteMasks[i];
					colorBlendAttachment.blendEnable = info.colorBlenderBlendEnables[i];

					colorBlendAttachments.push_back(colorBlendAttachment);
				}

				// initialize the color blender state
				VkPipelineColorBlendStateCreateInfo colorBlending = {};
				colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
				colorBlending.logicOpEnable = info.colorBlendingLogicOpEnable;
				colorBlending.logicOp = info.colorBlendingLogicOp;
				colorBlending.attachmentCount = info.colorBlendingColorBlendCount;
				colorBlending.pAttachments = colorBlendAttachments.data();
				colorBlending.blendConstants[0] = info.colorBlendingBlendConstants[0];
				colorBlending.blendConstants[1] = info.colorBlendingBlendConstants[1];
				colorBlending.blendConstants[2] = info.colorBlendingBlendConstants[2];
				colorBlending.blendConstants[3] = info.colorBlendingBlendConstants[3];

				// initialize the pipeline layout
				ARRAY<VkDescriptorSetLayout> layouts;
				for (VulkanDescriptorSet _sets : info.layouts)
					layouts.pushBack(_sets.layout);

				VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
				pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipelineLayoutInfo.setLayoutCount = static_cast<UI32>(layouts.size());
				pipelineLayoutInfo.pSetLayouts = layouts.data();

				if (info.pushConstantsEnable) {
					ARRAY<VkPushConstantRange> pushConstantInfos;

					info.global->pushConstants.resize(6);

#define r 7.5f
#define sin_t sin(glm::radians(1.5f * 360))
#define cos_t cos(glm::radians(1.5f * 360))
#define y -4.0f

					info.global->pushConstants[0] = glm::vec4(r * 1.1 * sin_t, y, r * 1.1 * cos_t, 1.0f);
					info.global->pushConstants[1] = glm::vec4(-r * sin_t, y, -r * cos_t, 1.0f);
					info.global->pushConstants[2] = glm::vec4(r * 0.85f * sin_t, y, -sin_t * 2.5f, 1.5f);
					info.global->pushConstants[3] = glm::vec4(0.0f, y, r * 1.25f * cos_t, 1.5f);
					info.global->pushConstants[4] = glm::vec4(r * 2.25f * cos_t, y, 0.0f, 1.25f);
					info.global->pushConstants[5] = glm::vec4(r * 2.5f * cos_t, y, r * 2.5f * sin_t, 1.25f);
#undef r
#undef y
#undef sin_t
#undef cos_t

					// initialize push constants
					for (I32 i = 0; i <= info.global->pushConstants.size(); i++) {
						VkPushConstantRange pushConsInfo = {};
						pushConsInfo.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;// TODO::
						pushConsInfo.size = info.global->pushConstants.size();
						pushConsInfo.offset = info.pushConstantOffset;

						pushConstantInfos.push_back(pushConsInfo);
					}
					pipelineLayoutInfo.pushConstantRangeCount = info.pushConstantCount;
					pipelineLayoutInfo.pPushConstantRanges = pushConstantInfos.data();
				}

				// create the pipeline layout
				VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
				if (vkCreatePipelineLayout(info.device.logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create pipeline layout!");

				// initialize dynamic state
				VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
				if (info.dynamicStateEnable) {
					ARRAY<VkDynamicState> dynamicState = {
						VK_DYNAMIC_STATE_VIEWPORT,
						VK_DYNAMIC_STATE_SCISSOR
					};

					dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
					dynamicStateInfo.dynamicStateCount = static_cast<UI32>(dynamicState.size());
					dynamicStateInfo.pDynamicStates = dynamicState.data();
					dynamicStateInfo.flags = info.dynamicStateFlags;
				}

				ARRAY<VkPipelineShaderStageCreateInfo> shaderStages;
				for (VulkanShader _info : info.shaders)
					shaderStages.pushBack(_info.stageCreateInfo);

				// initialize the pipeline
				VkGraphicsPipelineCreateInfo pipelineInfo = {};
				pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
				pipelineInfo.pNext = nullptr;
				pipelineInfo.stageCount = info.shaders.size();
				pipelineInfo.pStages = shaderStages.data();
				pipelineInfo.pVertexInputState = &vertexInputInfo;
				pipelineInfo.pInputAssemblyState = &inputAssembly;
				pipelineInfo.pViewportState = &viewportState;
				pipelineInfo.pRasterizationState = &rasterizer;
				pipelineInfo.pMultisampleState = &multisampling;
				pipelineInfo.pDepthStencilState = &depthStencil;
				pipelineInfo.pColorBlendState = &colorBlending;
				pipelineInfo.layout = pipelineLayout;
				pipelineInfo.renderPass = info.renderPass.renderPass;
				pipelineInfo.subpass = info.pipelineSubPass;
				pipelineInfo.basePipelineHandle = info.pipelineBasePipelineHandle;
				pipelineInfo.basePipelineIndex = info.pipelineBasePipelineIndex;
				pipelineInfo.pTessellationState = nullptr;

				if (info.dynamicStateEnable)
					pipelineInfo.pDynamicState = &dynamicStateInfo;

				// create the pipeline
				VkPipeline pipeline = VK_NULL_HANDLE;
				if (vkCreateGraphicsPipelines(info.device.logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create graphics pipeline!");
			}
			
			void VulkanPipeline::terminate(VulkanDevice device)
			{
				vkDestroyPipeline(device.logicalDevice, pipeline, nullptr);
				vkDestroyPipelineLayout(device.logicalDevice, pipelineLayout, nullptr);
			}
		}
	}
}