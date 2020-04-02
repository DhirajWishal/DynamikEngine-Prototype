#include "adgrafx.h"
#include "VulkanCommandBufferManager.h"
#include "VulkanQueueManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanCommandBufferManager::initializeCommandPool(POINTER<VulkanCoreObject> core)
			{
				VulkanQueueManager queueFamilyIndices = VulkanQueueManager::findQueueFamilies(core);

				VkCommandPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
				poolInfo.flags = 0;

				if (vkCreateCommandPool(core->logicalDevice, &poolInfo, nullptr, &core->commandPool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create command pool!");
			}

			void VulkanCommandBufferManager::initialize(POINTER<VulkanCoreObject> core, ADGRVulkanCommandBufferInitInfo info)
			{
				core->commandBuffers.resize(core->frameBuffers.size());

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = core->commandPool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = static_cast<UI32>(core->frameBuffers.size());

				if (vkAllocateCommandBuffers(core->logicalDevice, &allocInfo, core->commandBuffers.data()) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate command buffers!");

				for (size_t i = 0; i < core->frameBuffers.size(); i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

					if (vkBeginCommandBuffer(core->commandBuffers[i], &beginInfo) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to begin recording command buffer!");

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = core->renderPass;
					renderPassInfo.framebuffer = core->frameBuffers[i];
					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = core->swapChainExtent;

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
					vkCmdBeginRenderPass(core->commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					/* TODO: pushConstants */
					// pushConstants[0] = ...
					// vkCmdPushConstants(commandBuffers[i], &pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT,
					//		0, pushConstants.size(), pushConstants.data());
					//
					// Update light positions
					// w component = light radius scale

					/* DRAW COMMANDS */
					for (I32 _itr = 0; _itr < info.objects->size(); _itr++) {
						vkCmdPushConstants(core->commandBuffers[i], info.objects->at(_itr).pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, info.objects->at(_itr).pushConstants.size(), info.objects->at(_itr).pushConstants.data());

						// Render type selection
						if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_VERTEX) 		// Render as individual vertexes
							drawVertex(core->commandBuffers[i], i, &info.objects->at(_itr), offsets);

						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED) 		// Render as individual indexes
							drawIndexed(core->commandBuffers[i], i, &info.objects->at(_itr), offsets);

						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDIRECT) {
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED_INDIRECT) {
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_VERTEX) {		// Render as individual vertexes
							for (I32 x = 0; x < info.objects->at(_itr).vertexBuffers.size(); x++) {
								// bind pipeline
								vkCmdBindPipeline(core->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, info.objects->at(_itr).pipeline);

								// vertex buffer bind
								vkCmdBindVertexBuffers(core->commandBuffers[i], 0, 1, &info.objects->at(_itr).vertexBuffers[x].buffer, offsets);

								// binding descriptor set(s)
								if (info.objects->at(_itr).descriptorSets.size() > 0)
									vkCmdBindDescriptorSets(core->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
										info.objects->at(_itr).pipelineLayout, 0, 1, &info.objects->at(_itr).descriptorSets[x][i], 0, nullptr);

								// draw command
								vkCmdDraw(core->commandBuffers[i], info.objects->at(_itr).indexBuffers[x].count, 1, 0, 1);
							}
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDEXED) {		// Render as individual indexes
							for (I32 x = 0; x < info.objects->at(_itr).vertexBuffers.size(); x++) {
								// bind pipeline
								vkCmdBindPipeline(core->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, info.objects->at(_itr).pipeline);

								// vertex buffer bind
								vkCmdBindVertexBuffers(core->commandBuffers[i], 0, 1, &info.objects->at(_itr).vertexBuffers[x].buffer, offsets);

								// binding descriptor set(s)
								if (info.objects->at(_itr).descriptorSets.size() > 0)
									vkCmdBindDescriptorSets(core->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
										info.objects->at(_itr).pipelineLayout, 0, 1, &info.objects->at(_itr).descriptorSets[x][i], 0, nullptr);

								// index buffer bind
								vkCmdBindIndexBuffer(core->commandBuffers[i], info.objects->at(_itr).indexBuffers[x].buffer, 0, VK_INDEX_TYPE_UINT32);

								// draw command
								vkCmdDrawIndexed(core->commandBuffers[i], info.objects->at(_itr).indexBuffers[x].count, 1, 0, 0, 0);
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
					vkCmdEndRenderPass(core->commandBuffers[i]);

					/* END VULKAN COMMANDS */
					if (vkEndCommandBuffer(core->commandBuffers[i]) != VK_SUCCESS)
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
			void VulkanCommandBufferManager::drawVertex(VkCommandBuffer buffer, I32 index, VulkanRenderObject* object, VkDeviceSize* offsets) {
				for (UI32 i = 0; i < object->vertexBuffers.size(); i++) {
					// bind pipeline
					vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipeline);

					// vertex buffer bind
					vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBuffers[i].buffer, offsets);

					// binding descriptor set(s)
					if (object->descriptorSets.size())
						vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineLayout, 0, 1, &object->descriptorSets[i][index], 0, nullptr);

					// draw command
					vkCmdDraw(buffer, object->vertexBuffers[i].count, 1, 0, 1);
				}
			}

			// if drawing in indexed
			void VulkanCommandBufferManager::drawIndexed(VkCommandBuffer buffer, I32 index, VulkanRenderObject* object, VkDeviceSize* offsets) {
				for (UI32 i = 0; i < object->vertexBuffers.size(); i++) {
					// bind pipeline
					vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipeline);

					// vertex buffer bind
					vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBuffers[i].buffer, offsets);

					// binding descriptor set(s)
					if (object->descriptorSets.size())
						vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineLayout, 0, 1, &object->descriptorSets[i][index], 0, nullptr);

					// index buffer bind
					vkCmdBindIndexBuffer(buffer, object->indexBuffers[i].buffer, 0, VK_INDEX_TYPE_UINT32);

					// draw command
					vkCmdDrawIndexed(buffer, object->indexBuffers[i].count, 1, 0, 0, 0);
				}
			}

			void VulkanCommandBufferManager::terminate(POINTER<VulkanCoreObject> core)
			{
				vkFreeCommandBuffers(core->logicalDevice, core->commandPool, static_cast<UI32>(core->commandBuffers.size()), core->commandBuffers.data());
				vkDestroyCommandPool(core->logicalDevice, core->commandPool, nullptr);
			}
		}
	}
}