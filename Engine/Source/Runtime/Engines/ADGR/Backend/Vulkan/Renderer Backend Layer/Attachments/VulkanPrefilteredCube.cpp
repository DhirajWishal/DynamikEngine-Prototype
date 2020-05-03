#include "dmkafx.h"
#include "VulkanPrefilteredCube.h"

#include "../Common/VulkanUtilities.h"
#include "../Common/VulkanOneTimeCommandBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanPrefilteredCube::initialize()
			{
				myTextureContainer.format = VK_FORMAT_R16G16B16A16_SFLOAT;
				dimentions = 512;

				_initializeTexture();
				_initializeRenderPass();
				_initializeFrameBuffer();
				_initializeDescriptorSetLayout();
				_initializeDescriptorPool();
				_initializeDescriptorSets();
				_initializePipelineLayout();
				_initializePipeline();
				_initializeCommandBuffers();
			}

			void VulkanPrefilteredCube::_initializeTexture()
			{
				myTextureContainer.mipLevels = static_cast<UI32>(floor(log2(dimentions))) + 1;
				myTextureContainer.width = dimentions;
				myTextureContainer.height = dimentions;

				ADGRVulkanCreateImageInfo cinfo;
				cinfo.width = myTextureContainer.width;
				cinfo.height = myTextureContainer.height;
				cinfo.format = myTextureContainer.format;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &myTextureContainer.image;
				cinfo.imageMemory = &myTextureContainer.imageMemory;
				cinfo.mipLevels = myTextureContainer.mipLevels;
				cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
				cinfo.arrayLayers = 6;
				cinfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
				VulkanUtilities::createImage(logicalDevice, physicalDevice, cinfo);

				ADGRVulkanTextureSamplerInitInfo samplerInitInfo;
				samplerInitInfo.magFilter = VK_FILTER_LINEAR;
				samplerInitInfo.minFilter = VK_FILTER_LINEAR;
				samplerInitInfo.maxMipLevels = myTextureContainer.mipLevels;
				samplerInitInfo.minMipLevels = 0.0f;
				samplerInitInfo.modeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				samplerInitInfo.modeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				samplerInitInfo.modeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				samplerInitInfo.mipLoadBias = 0.0f;
				samplerInitInfo.compareOp = VK_COMPARE_OP_NEVER;
				samplerInitInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
				myTextureContainer.imageSampler = VulkanUtilities::createImageSampler(logicalDevice, samplerInitInfo);

				ADGRVulkanCreateImageViewInfo cinfo2;
				cinfo2.image = myTextureContainer.image;
				cinfo2.format = myTextureContainer.format;
				cinfo2.mipLevels = myTextureContainer.mipLevels;
				cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				cinfo2.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
				cinfo2.layerCount = 6;
				cinfo2.component = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
				myTextureContainer.imageView = VulkanUtilities::createImageView(logicalDevice, cinfo2);
			}

			void VulkanPrefilteredCube::_initializePipelineLayout()
			{
				VkPushConstantRange pushBlockRange;
				pushBlockRange.size = sizeof(PushBlock);
				pushBlockRange.offset = 0;
				pushBlockRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

				ADGRVulkanGraphicsPipelineLayoutInitInfo initInfo;
				initInfo.layouts = { myDescriptorContainer.layout };
				initInfo.pushConstantRanges = { pushBlockRange };
				myPipeline.initializePipelineLayout(logicalDevice, initInfo);
			}

			void VulkanPrefilteredCube::_initializePipeline()
			{
				ADGRVulkanGraphicsShaderInitInfo VSInitInfo;
				VSInitInfo.path = "Shaders/Prefiltered/vert.spv";
				VSInitInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;
				VulkanGraphicsShader VS;
				VS.initialize(logicalDevice, VSInitInfo);

				ADGRVulkanGraphicsShaderInitInfo FSInitInfo;
				FSInitInfo.path = "Shaders/Prefiltered/frag.spv";
				FSInitInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;
				VulkanGraphicsShader FS;
				FS.initialize(logicalDevice, FSInitInfo);

				ADGRVulkanGraphicsPipelineInitInfo initInfo;
				initInfo.shaders = { VS, FS };
				//initInfo.vertexAttributeDescription = VertexP::getAttributeDescriptions();
				//initInfo.vertexBindingDescription = VertexP::getBindingDescription(1);
				initInfo.swapChainExtent.width = dimentions;
				initInfo.swapChainExtent.height = dimentions;
				initInfo.inputAssemblyTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				initInfo.renderPass = myFrameBuffer.renderPass;
				initInfo.dynamicStateEnable = true;
				myPipeline.initializePipeline(logicalDevice, initInfo);
			}

			void VulkanPrefilteredCube::_initializeCommandBuffers()
			{
				std::vector<glm::mat4> matrices = {
					// POSITIVE_X
					glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// NEGATIVE_X
					glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// POSITIVE_Y
					glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// NEGATIVE_Y
					glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// POSITIVE_Z
					glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// NEGATIVE_Z
					glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
				};

				VulkanOneTimeCommandBuffer oneTimeCommandBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue);
				std::array<VkClearValue, 1> clearValues;
				clearValues[0].color = { { 0.0f, 0.0f, 0.2f, 0.0f } };

				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = myFrameBuffer.renderPass;
				renderPassInfo.framebuffer = offscreenContainer.frameBuffer;
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent.width = dimentions;
				renderPassInfo.renderArea.extent.height = dimentions;
				renderPassInfo.clearValueCount = clearValues.size();
				renderPassInfo.pClearValues = clearValues.data();

				VkCommandBuffer buffer = oneTimeCommandBuffer.buffer;

				VkViewport viewport;
				viewport.height = dimentions;
				viewport.width = dimentions;
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;
				viewport.x = 0;
				viewport.y = 0;
				vkCmdSetViewport(buffer, 0, 1, &viewport);

				VkRect2D scissor;
				scissor.extent.width = dimentions;
				scissor.extent.height = dimentions;
				scissor.offset.x = 0;
				scissor.offset.y = 0;
				vkCmdSetScissor(buffer, 0, 1, &scissor);

				ADGRVulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = myTextureContainer.image;
				transitionInfo.format = myTextureContainer.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.mipLevels = myTextureContainer.mipLevels;
				transitionInfo.layerCount = 6;
				VulkanUtilities::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

				for (UI32 m = 0; m < myTextureContainer.mipLevels; m++) {
					for (UI32 f = 0; f < 6; f++) {
						viewport.width = static_cast<float>(dimentions * std::pow(0.5f, m));
						viewport.height = static_cast<float>(dimentions * std::pow(0.5f, m));
						vkCmdSetViewport(buffer, 0, 1, &viewport);

						// Render scene from cube face's point of view
						vkCmdBeginRenderPass(buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

						// Update shader push constant block
						float FOV = M_PI / 2.0f;
						pushBlock.mvp = glm::perspective(FOV, 1.0f, 0.1f, 512.0f) * matrices[f];

						vkCmdPushConstants(buffer, myPipeline.layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushBlock), &pushBlock);

						vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, myPipeline.pipeline);
						for (VkDescriptorSet _set : myDescriptorContainer.descriptorSets)
							vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, myPipeline.layout, 0, 1, &_set, 0, NULL);

						VkDeviceSize offsets[1] = { 0 };

						//vkCmdBindVertexBuffers(buffer, 0, 1, &skyboxRenderData.vertexBuffers[0], offsets);
						//vkCmdBindIndexBuffer(buffer, skyboxRenderData.indexBuffers[0], 0, VK_INDEX_TYPE_UINT32);
						//vkCmdDrawIndexed(buffer, skyboxRenderData.indexCount, 1, 0, 0, 0);

						vkCmdEndRenderPass(buffer);

						transitionInfo.image = offscreenContainer.image;
						transitionInfo.format = myTextureContainer.format;
						transitionInfo.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
						transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
						transitionInfo.mipLevels = 1;
						transitionInfo.layerCount = 1;
						VulkanUtilities::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

						// Copy region for transfer from framebuffer to cube face
						VkImageCopy copyRegion = {};

						copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						copyRegion.srcSubresource.baseArrayLayer = 0;
						copyRegion.srcSubresource.mipLevel = 0;
						copyRegion.srcSubresource.layerCount = 1;
						copyRegion.srcOffset = { 0, 0, 0 };

						copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						copyRegion.dstSubresource.baseArrayLayer = f;
						copyRegion.dstSubresource.mipLevel = m;
						copyRegion.dstSubresource.layerCount = 1;
						copyRegion.dstOffset = { 0, 0, 0 };

						copyRegion.extent.width = static_cast<UI32>(viewport.width);
						copyRegion.extent.height = static_cast<UI32>(viewport.height);
						copyRegion.extent.depth = 1;

						{
							VulkanOneTimeCommandBuffer oneTimeBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue);

							vkCmdCopyImage(
								oneTimeBuffer.buffer,
								offscreenContainer.image,
								VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
								myTextureContainer.image,
								VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
								1,
								&copyRegion);
						}

						// Transform framebuffer color attachment back 
						transitionInfo.image = offscreenContainer.image;
						transitionInfo.format = myTextureContainer.format;
						transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
						transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
						transitionInfo.mipLevels = 1;
						transitionInfo.layerCount = 1;
						VulkanUtilities::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);
					}
				}

				transitionInfo.image = myTextureContainer.image;
				transitionInfo.format = myTextureContainer.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				transitionInfo.mipLevels = myTextureContainer.mipLevels;
				transitionInfo.layerCount = 6;
				VulkanUtilities::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);
			}
		}
	}
}