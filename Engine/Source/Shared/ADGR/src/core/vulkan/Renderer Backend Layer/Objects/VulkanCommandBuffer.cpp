#include "adgrafx.h"
#include "VulkanCommandBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanCommandBuffer::initializeCommandPool(VulkanInstance instance, VulkanDevice device)
			{
				VulkanQueue queueFamilyIndices = VulkanQueue::findQueueFamilies(device, instance);

				VkCommandPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
				poolInfo.flags = 0;

				if (vkCreateCommandPool(device.logicalDevice, &poolInfo, nullptr, &myCommandPool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create command pool!");
			}

			void VulkanCommandBuffer::initialize(ADGRVulkanCommandBufferInitInfo info)
			{
				myCommandBuffers.resize(info.frameBuffer.frameBuffers.size());

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = myCommandPool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = static_cast<UI32>(info.frameBuffer.frameBuffers.size());

				if (vkAllocateCommandBuffers(info.device.logicalDevice, &allocInfo, myCommandBuffers.data()) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate command buffers!");

				for (size_t i = 0; i < info.frameBuffer.frameBuffers.size(); i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

					if (vkBeginCommandBuffer(myCommandBuffers[i], &beginInfo) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to begin recording command buffer!");

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = info.renderPass.renderPass;
					renderPassInfo.framebuffer = info.frameBuffer.frameBuffers[i];
					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = info.swapChain.mySwapChainExtent;

					/*
					 (2.0f / 256.0f),
					 (8.0f / 256.0f),
					 (32.0f / 256.0f),
					 (1.00000000f)
					*/

					std::array<VkClearValue, 2> clearValues = {};
					//clearValues[0].color = {
					//	container->clearScreenValues[0],	// Red
					//	container->clearScreenValues[1],	// Green
					//	container->clearScreenValues[2],	// Blue
					//	container->clearScreenValues[3]		// Alpha
					//};

					clearValues[0].color = {
					 (2.0f / 256.0f),
					 (8.0f / 256.0f),
					 (32.0f / 256.0f),
					 (1.00000000f)
					};
					clearValues[1].depthStencil = { 1.0f, 0 };

					renderPassInfo.clearValueCount = static_cast<UI32>(clearValues.size());
					renderPassInfo.pClearValues = clearValues.data();

					/* BEGIN VULKAN COMMANDS */
					VkDeviceSize offsets[] = { 0 };
					// begin render pass
					vkCmdBeginRenderPass(myCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					/* TODO: pushConstants */
					// pushConstants[0] = ...
					// vkCmdPushConstants(commandBuffers[i], &pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT,
					//		0, pushConstants.size(), pushConstants.data());
					//
					// Update light positions
					// w component = light radius scale

					/* DRAW COMMANDS */
					for (I32 _itr = 0; _itr < info.objects->size(); _itr++) {
						vkCmdPushConstants(myCommandBuffers[i], info.objects->at(_itr).pipeline.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, info.objects->at(_itr).pushConstants.size(), info.objects->at(_itr).pushConstants.data());

						// Render type selection
						if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_VERTEX) 		// Render as individual vertexes
							drawVertex(myCommandBuffers[i], i, &info.objects->at(_itr), offsets);

						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED) 		// Render as individual indexes
							drawIndexed(myCommandBuffers[i], i, &info.objects->at(_itr), offsets);

						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDIRECT) {
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED_INDIRECT) {
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_VERTEX) {		// Render as individual vertexes
							for (I32 x = 0; x < info.objects->at(_itr).vertexBuffers.size(); x++) {
								// bind pipeline
								vkCmdBindPipeline(myCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, info.objects->at(_itr).pipeline.pipeline);

								// vertex buffer bind
								vkCmdBindVertexBuffers(myCommandBuffers[i], 0, 1, &info.objects->at(_itr).vertexBuffers[x].buffer, offsets);

								// binding descriptor set(s)
								if (info.objects->at(_itr).descriptorSet.descriptorSets.size() > 0)
									vkCmdBindDescriptorSets(myCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
										info.objects->at(_itr).pipeline.pipelineLayout, 0, 1, &info.objects->at(_itr).descriptorSet.descriptorSets[x][i], 0, nullptr);

								// draw command
								vkCmdDraw(myCommandBuffers[i], info.objects->at(_itr).indexBuffers[x].count, 1, 0, 1);
							}
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDEXED) {		// Render as individual indexes
							for (I32 x = 0; x < info.objects->at(_itr).vertexBuffers.size(); x++) {
								// bind pipeline
								vkCmdBindPipeline(myCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, info.objects->at(_itr).pipeline.pipeline);

								// vertex buffer bind
								vkCmdBindVertexBuffers(myCommandBuffers[i], 0, 1, &info.objects->at(_itr).vertexBuffers[x].buffer, offsets);

								// binding descriptor set(s)
								if (info.objects->at(_itr).descriptorSet.descriptorSets.size() > 0)
									vkCmdBindDescriptorSets(myCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
										info.objects->at(_itr).pipeline.pipelineLayout, 0, 1, &info.objects->at(_itr).descriptorSet.descriptorSets[x][i], 0, nullptr);

								// index buffer bind
								vkCmdBindIndexBuffer(myCommandBuffers[i], info.objects->at(_itr).indexBuffers[x].buffer, 0, VK_INDEX_TYPE_UINT32);

								// draw command
								vkCmdDrawIndexed(myCommandBuffers[i], info.objects->at(_itr).indexBuffers[x].count, 1, 0, 0, 0);
							}
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDIRECT) {
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDEXED_INDIRECT) {
						}
						else
							DMK_CORE_FATAL("Invalid rendering type!");
					}

					// end renderPass
					vkCmdEndRenderPass(myCommandBuffers[i]);

					/* END VULKAN COMMANDS */
					if (vkEndCommandBuffer(myCommandBuffers[i]) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to record command buffer!");

					/*
					 Vulkan command (draw) order:
						begin render pass
						Per-object:
							init pipeline
							bind vertex buffers
							bind index buffers
							bind descriptor sets
							draw indexed data
						end render pass
						end command buffer
					*/
				}
			}

			// if drawing in vertex
			void VulkanCommandBuffer::drawVertex(VkCommandBuffer buffer, I32 index, VulkanRenderObject* object, VkDeviceSize* offsets) {
				for (UI32 i = 0; i < object->vertexBuffers.size(); i++) {
					// bind pipeline
					vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipeline.pipeline);

					// vertex buffer bind
					vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBuffers[i].buffer, offsets);

					// binding descriptor set(s)
					if (object->descriptorSet.descriptorSets.size())
						vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipeline.pipelineLayout, 0, 1, &object->descriptorSet.descriptorSets[i][index], 0, nullptr);

					// draw command
					vkCmdDraw(buffer, object->vertexBuffers[i].count, 1, 0, 1);
				}
			}

			// if drawing in indexed
			void VulkanCommandBuffer::drawIndexed(VkCommandBuffer buffer, I32 index, VulkanRenderObject* object, VkDeviceSize* offsets) {
				for (UI32 i = 0; i < object->vertexBuffers.size(); i++) {
					// bind pipeline
					vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipeline.pipeline);

					// vertex buffer bind
					vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBuffers[i].buffer, offsets);

					// binding descriptor set(s)
					if (object->descriptorSet.descriptorSets.size())
						vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipeline.pipelineLayout, 0, 1, &object->descriptorSet.descriptorSets[i][index], 0, nullptr);

					// index buffer bind
					vkCmdBindIndexBuffer(buffer, object->indexBuffers[i].buffer, 0, VK_INDEX_TYPE_UINT32);

					// draw command
					vkCmdDrawIndexed(buffer, object->indexBuffers[i].count, 1, 0, 0, 0);
				}
			}

			void VulkanCommandBuffer::terminate(VulkanDevice device)
			{
				vkFreeCommandBuffers(device.logicalDevice, myCommandPool, static_cast<UI32>(myCommandBuffers.size()), myCommandBuffers.data());
				vkDestroyCommandPool(device.logicalDevice, myCommandPool, nullptr);
			}
		}
	}
}