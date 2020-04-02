#include "adgrafx.h"
#include "VulkanPipelineManager.h"

#include "VulkanDescriptorManager.h"
#include "VulkanShaderManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanPipelineManager::initialize(POINTER<VulkanCoreObject> core, ADGRVulkanPipelineInitInfo info, POINTER<VulkanRenderObject> object)
			{
				VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
				pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipelineLayoutInfo.setLayoutCount = 1;
				pipelineLayoutInfo.pSetLayouts = &object->descriptorLayout;

				if (info.pushConstantsEnable) {
					ARRAY<VkPushConstantRange> pushConstantInfos;

					core->pushConstants.resize(info.pushConstantCount);
					/*
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
					*/
					// initialize push constants
					for (I32 i = 0; i < info.pushConstantCount; i++) {
						VkPushConstantRange pushConsInfo = {};
						pushConsInfo.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;// TODO::
						pushConsInfo.size = sizeof(core->pushConstants);
						pushConsInfo.offset = info.pushConstantOffset;

						pushConstantInfos.push_back(pushConsInfo);
					}
					pipelineLayoutInfo.pushConstantRangeCount = info.pushConstantCount;
					pipelineLayoutInfo.pPushConstantRanges = pushConstantInfos.data();
				}
				else
				{
					pipelineLayoutInfo.pushConstantRangeCount = 0;
					pipelineLayoutInfo.pPushConstantRanges = nullptr;
				}

				// create the pipeline layout
				if (vkCreatePipelineLayout(core->logicalDevice, &pipelineLayoutInfo, nullptr, &object->pipelineLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create pipeline layout!");

				auto bindingDescription = Vertex::getBindingDescription(1);
				auto attributeDescriptions = Vertex::getAttributeDescriptions();

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
				std::array<VkViewport, 1> viewports;
				for (I32 i = 0; i < info.viweportCount; i++) {
					VkViewport viewport = {};
					viewport.x = 0.0f;
					viewport.y = 0.0f;
					viewport.width = core->swapChainExtent.width;
					viewport.height = core->swapChainExtent.height;
					viewport.minDepth = 0.0f;
					viewport.maxDepth = 1.0f;

					viewports[0] = (viewport);
				}

				// initialize the scissor(s)
				std::array<VkRect2D, 1> scissors;
				for (I32 i = 0; i < info.scissorCount; i++) {
					VkRect2D scissor = {};
					scissor.offset = info.offsets[0];
					scissor.extent = core->swapChainExtent;

					scissors[0] = (scissor);
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
				multisampling.rasterizationSamples = core->msaaSamples;
				multisampling.sampleShadingEnable = info.multisamplerSampleShadingEnable; // enable sample shading in the pipeline
				multisampling.minSampleShading = info.multisamplerMinSampleShading; // min fraction for sample shading; closer to one is smoother

				VkPipelineDepthStencilStateCreateInfo depthStencil = {};
				depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
				depthStencil.depthTestEnable = info.depthStencilEnable;
				depthStencil.depthWriteEnable = info.depthStencilWriteEnable;
				depthStencil.depthCompareOp = info.depthStencilCompareOp;
				depthStencil.depthBoundsTestEnable = info.depthStencilBoundsTestEnable;
				depthStencil.stencilTestEnable = info.depthStencilTestEnable;

				std::array<VkPipelineColorBlendAttachmentState, 1> colorBlendAttachments;

				// initialize the color blender(s)
				for (I32 i = 0; i < info.colorBlendingColorBlendCount; i++) {
					VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
					colorBlendAttachment.colorWriteMask = info.colorBlenderColorWriteMasks[i];
					colorBlendAttachment.blendEnable = info.colorBlenderBlendEnables[i];

					colorBlendAttachments[0] = (colorBlendAttachment);
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

				//ARRAY<VkPipelineShaderStageCreateInfo> shaderStages;
				//for (VulkanShaderManager _shader : info.shaders)
				//	shaderStages.push_back(_shader.stageCreateInfo);

				VkPipelineShaderStageCreateInfo _infoV;
				_infoV.stage = VK_SHADER_STAGE_VERTEX_BIT;
				_infoV.module = VulkanShaderManager::createModule(core->logicalDevice, VulkanShaderManager::getCode(info.vertex));
				_infoV.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				_infoV.pName = "main";
				_infoV.pNext = VK_NULL_HANDLE;
				_infoV.pSpecializationInfo = VK_NULL_HANDLE;
				_infoV.flags = VK_NULL_HANDLE;

				VkPipelineShaderStageCreateInfo _infoF;
				_infoF.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				_infoF.module = VulkanShaderManager::createModule(core->logicalDevice, VulkanShaderManager::getCode(info.fragment));
				_infoF.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				_infoF.pName = "main";
				_infoF.pNext = VK_NULL_HANDLE;
				_infoF.pSpecializationInfo = VK_NULL_HANDLE;
				_infoF.flags = VK_NULL_HANDLE;

				std::array<VkPipelineShaderStageCreateInfo, 2> stages = { _infoV , _infoF };

				// initialize the pipeline
				VkGraphicsPipelineCreateInfo pipelineInfo = {};
				pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
				//pipelineInfo.stageCount = shaderStages.size();
				//pipelineInfo.pStages = shaderStages.data();
				pipelineInfo.stageCount = 2;
				pipelineInfo.pStages = stages.data();
				pipelineInfo.pVertexInputState = &vertexInputInfo;
				pipelineInfo.pInputAssemblyState = &inputAssembly;
				pipelineInfo.pViewportState = &viewportState;
				pipelineInfo.pRasterizationState = &rasterizer;
				pipelineInfo.pMultisampleState = &multisampling;
				pipelineInfo.pDepthStencilState = &depthStencil;
				pipelineInfo.pColorBlendState = &colorBlending;
				pipelineInfo.layout = object->pipelineLayout;
				pipelineInfo.renderPass = *object->renderPass;
				pipelineInfo.subpass = info.pipelineSubPass;
				pipelineInfo.basePipelineHandle = info.pipelineBasePipelineHandle;
				pipelineInfo.basePipelineIndex = info.pipelineBasePipelineIndex;

				// initialize dynamic state
				if (info.dynamicStateEnable) {
					VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
					ARRAY<VkDynamicState> dynamicState = {
						VK_DYNAMIC_STATE_VIEWPORT,
						VK_DYNAMIC_STATE_SCISSOR
					};

					dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
					dynamicStateInfo.dynamicStateCount = static_cast<UI32>(dynamicState.size());
					dynamicStateInfo.pDynamicStates = dynamicState.data();
					dynamicStateInfo.flags = info.dynamicStateFlags;
					pipelineInfo.pDynamicState = &dynamicStateInfo;
				}

				// create the pipeline
				if (vkCreateGraphicsPipelines(core->logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &object->pipeline) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create graphics pipeline!");
			}

			void VulkanPipelineManager::terminate(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object)
			{
				vkDestroyPipeline(core->logicalDevice, object->pipeline, nullptr);
				vkDestroyPipelineLayout(core->logicalDevice, object->pipelineLayout, nullptr);
			}
		}
	}
}