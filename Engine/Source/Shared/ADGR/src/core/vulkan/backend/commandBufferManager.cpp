#include "adgrafx.h"

#include "commandBufferManager.h"

#include "queues/queues.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		namespace core {
			void commandBufferManager::initCommandPool(ADGRVulkanDataContainer* container) {
				queueFamilyindices queueFamilyIndices = findQueueFamilies(container->physicalDevice, container->surface);

				VkCommandPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
				poolInfo.flags = 0;

				if (vkCreateCommandPool(container->device, &poolInfo, nullptr, &container->commandBufferContainer.commandPool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create command pool!");
			}

			void commandBufferManager::bindCommands(ADGRVulkanDataContainer* container, ARRAY<vulkanFormat>* formats) {
				container->commandBufferContainer.buffers.resize(container->frameBufferContainer.buffers.size());

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = container->commandBufferContainer.commandPool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = static_cast<UI32>(container->frameBufferContainer.buffers.size());

				if (vkAllocateCommandBuffers(container->device, &allocInfo, container->commandBufferContainer.buffers.data()) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate command buffers!");

				for (size_t i = 0; i < container->frameBufferContainer.buffers.size(); i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

					if (vkBeginCommandBuffer(container->commandBufferContainer.buffers[i], &beginInfo) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to begin recording command buffer!");

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = container->renderPass;
					renderPassInfo.framebuffer = container->frameBufferContainer.buffers[i];
					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = container->swapchainContainer.swapchainExtent;

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
					vkCmdBeginRenderPass(container->commandBufferContainer.buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					/* TODO: pushConstants */
					// pushConstants[0] = ...
					// vkCmdPushConstants(commandBuffers[i], &pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT,
					//		0, pushConstants.size(), pushConstants.data());
					//
					// Update light positions
					// w component = light radius scale

					/* DRAW COMMANDS */
					for (I32 _itr = 0; _itr < formats->size(); _itr++) {
#define r 7.5f
#define sin_t sin(glm::radians(1.5f * 360))
#define cos_t cos(glm::radians(1.5f * 360))
#define y -4.0f
						container->pushConstants[0] = glm::vec4(r * 1.1 * sin_t, y, r * 1.1 * cos_t, 1.0f);
						container->pushConstants[1] = glm::vec4(-r * sin_t, y, -r * cos_t, 1.0f);
						container->pushConstants[2] = glm::vec4(r * 0.85f * sin_t, y, -sin_t * 2.5f, 1.5f);
						container->pushConstants[3] = glm::vec4(0.0f, y, r * 1.25f * cos_t, 1.5f);
						container->pushConstants[4] = glm::vec4(r * 2.25f * cos_t, y, 0.0f, 1.25f);
						container->pushConstants[5] = glm::vec4(r * 2.5f * cos_t, y, r * 2.5f * sin_t, 1.25f);
#undef r
#undef y
#undef sin_t
#undef cos_t
						vkCmdPushConstants(container->commandBufferContainer.buffers[i], formats->at(_itr).myPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(container->pushConstants), container->pushConstants.data());

						// Render type selection
						if (formats->at(_itr).myRendererFormat->myRenderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_VERTEX) 		// Render as individual vertexes
							drawVertex(container->commandBufferContainer.buffers[i], i, &formats->at(_itr), offsets);

						else if (formats->at(_itr).myRendererFormat->myRenderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED) 		// Render as individual indexes
							drawIndexed(container->commandBufferContainer.buffers[i], i, &formats->at(_itr), offsets);

						else if (formats->at(_itr).myRendererFormat->myRenderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDIRECT) {
						}
						else if (formats->at(_itr).myRendererFormat->myRenderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED_INDIRECT) {
						}
						else if (formats->at(_itr).myRendererFormat->myRenderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_VERTEX) {		// Render as individual vertexes
							for (I32 x = 0; x < formats->at(_itr).myVertexBuffers.size(); x++) {
								// bind pipeline
								vkCmdBindPipeline(container->commandBufferContainer.buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, formats->at(_itr).myPipeline);

								// vertex buffer bind
								vkCmdBindVertexBuffers(container->commandBufferContainer.buffers[i], 0, 1, &formats->at(_itr).myVertexBuffers[x], offsets);

								// binding descriptor set(s)
								if (formats->at(_itr).myDescriptorSets.size() > 0)
									vkCmdBindDescriptorSets(container->commandBufferContainer.buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
										formats->at(_itr).myPipelineLayout, 0, 1, &formats->at(_itr).myDescriptorSets[x][i], 0, nullptr);

								// draw command
								vkCmdDraw(container->commandBufferContainer.buffers[i], formats->at(_itr).myRendererFormat->myInternalFormat->myIndexCounts[0], 1, 0, 1);
							}
						}
						else if (formats->at(_itr).myRendererFormat->myRenderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDEXED) {		// Render as individual indexes
							for (I32 x = 0; x < formats->at(_itr).myVertexBuffers.size(); x++) {
								// bind pipeline
								vkCmdBindPipeline(container->commandBufferContainer.buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, formats->at(_itr).myPipeline);

								// vertex buffer bind
								vkCmdBindVertexBuffers(container->commandBufferContainer.buffers[i], 0, 1, &formats->at(_itr).myVertexBuffers[x], offsets);

								// binding descriptor set(s)
								if (formats->at(_itr).myDescriptorSets.size() > 0)
									vkCmdBindDescriptorSets(container->commandBufferContainer.buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
										formats->at(_itr).myPipelineLayout, 0, 1, &formats->at(_itr).myDescriptorSets[x][i], 0, nullptr);

								// index buffer bind
								vkCmdBindIndexBuffer(container->commandBufferContainer.buffers[i], formats->at(_itr).myIndexBuffers[0], 0, VK_INDEX_TYPE_UINT32);

								// draw command
								vkCmdDrawIndexed(container->commandBufferContainer.buffers[i], formats->at(_itr).myRendererFormat->myInternalFormat->myIndexCounts[0], 1, 0, 0, 0);
							}
						}
						else if (formats->at(_itr).myRendererFormat->myRenderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDIRECT) {
						}
						else if (formats->at(_itr).myRendererFormat->myRenderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDEXED_INDIRECT) {
						}
						else
							DMK_CORE_FATAL("Invalid rendering type!");
					}

					// end renderPass
					vkCmdEndRenderPass(container->commandBufferContainer.buffers[i]);

					/* END VULKAN COMMANDS */
					if (vkEndCommandBuffer(container->commandBufferContainer.buffers[i]) != VK_SUCCESS)
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

			// free command buffer and command pool
			void commandBufferManager::clear(ADGRVulkanDataContainer* container) {
				vkFreeCommandBuffers(container->device, container->commandBufferContainer.commandPool,
					static_cast<UI32>(container->commandBufferContainer.buffers.size()),
					container->commandBufferContainer.buffers.data());

				vkDestroyCommandPool(container->device, container->commandBufferContainer.commandPool, nullptr);
			}

			// if drawing in vertex
			void commandBufferManager::drawVertex(VkCommandBuffer buffer, I32 index, vulkanFormat* format, VkDeviceSize* offsets) {
				for (UI32 i = 0; i < format->myVertexBuffers.size(); i++) {
					// bind pipeline
					vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, format->myPipeline);

					// vertex buffer bind
					vkCmdBindVertexBuffers(buffer, 0, 1, &format->myVertexBuffers[i], offsets);

					// binding descriptor set(s)
					if (format->myDescriptorSets.size())
						vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, format->myPipelineLayout, 0, 1, &format->myDescriptorSets[i][index], 0, nullptr);

					// draw command
					vkCmdDraw(buffer, format->myRendererFormat->myInternalFormat->myVertexCounts[i], 1, 0, 1);
				}
			}

			// if drawing in indexed
			void commandBufferManager::drawIndexed(VkCommandBuffer buffer, I32 index, vulkanFormat* format, VkDeviceSize* offsets) {
				for (UI32 i = 0; i < format->myVertexBuffers.size(); i++) {
					// bind pipeline
					vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, format->myPipeline);

					// vertex buffer bind
					vkCmdBindVertexBuffers(buffer, 0, 1, &format->myVertexBuffers[i], offsets);

					// binding descriptor set(s)
					if (format->myDescriptorSets.size())
						vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, format->myPipelineLayout, 0, 1, &format->myDescriptorSets[i][index], 0, nullptr);

					// index buffer bind
					vkCmdBindIndexBuffer(buffer, format->myIndexBuffers[i], 0, VK_INDEX_TYPE_UINT32);

					// draw command
					vkCmdDrawIndexed(buffer, format->myRendererFormat->myInternalFormat->myIndexCounts[i], 1, 0, 0, 0);
				}
			}

			/* ONE TIME COMMAND BUFFER CLASS DEFINITION */
			oneTimeCommandBufferManager::oneTimeCommandBufferManager(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, UI32 count) {
				myDevice = device;
				myCommandPool = commandPool;
				myGraphicsQueue = graphicsQueue;
				myCount = count;

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandPool = myCommandPool;
				allocInfo.commandBufferCount = myCount;

				myCommandBuffers.resize(myCount);
				vkAllocateCommandBuffers(myDevice, &allocInfo, myCommandBuffers.data());

				for (I32 i = 0; i < myCount; i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

					vkBeginCommandBuffer(myCommandBuffers[i], &beginInfo);
				}
			}

			oneTimeCommandBufferManager::~oneTimeCommandBufferManager() {
				if (!isDestroyed)
					destroyBuffers();
			}

			void oneTimeCommandBufferManager::destroyBuffers() {
				for (I32 i = 0; i < myCount; i++)
					vkEndCommandBuffer(myCommandBuffers[i]);

				VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = myCount;
				submitInfo.pCommandBuffers = myCommandBuffers.data();

				vkQueueSubmit(myGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
				vkQueueWaitIdle(myGraphicsQueue);

				vkFreeCommandBuffers(myDevice, myCommandPool, myCount, myCommandBuffers.data());

				isDestroyed = true;
			}

			ARRAY<VkCommandBuffer> oneTimeCommandBufferManager::getCommandBuffers() {
				return myCommandBuffers;
			}
		}
	}
}
#endif // DMK_USE_VULKAN