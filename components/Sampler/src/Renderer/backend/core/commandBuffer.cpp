#include "adgrafx.h"
#include "commandBuffer.h"

#include "queues/queues.h"

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

			void commandBufferManager::bindCommands(ADGRVulkanDataContainer* container, DMKBindCommandBufferInfo info) {
				container->commandBufferContainer.buffers.resize(container->frameBufferContainer.buffers.size());

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = container->commandBufferContainer.commandPool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = static_cast<uint32_t>(container->frameBufferContainer.buffers.size());

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
					renderPassInfo.renderPass = container->pipelineContainers[0].renderPass;
					renderPassInfo.framebuffer = container->frameBufferContainer.buffers[i];
					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = container->swapchainContainer.swapchainExtent;

					std::array<VkClearValue, 2> clearValues = {};
					clearValues[0].color = {
						container->clearScreenValues[0],	// Red
						container->clearScreenValues[1],	// Green
						container->clearScreenValues[2],	// Blue
						container->clearScreenValues[3]		// Alpha
					};
					clearValues[1].depthStencil = { 1.0f, 0 };

					renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
					renderPassInfo.pClearValues = clearValues.data();

					/* BEGIN VULKAN COMMANDS */
					VkDeviceSize offsets[] = { 0 };
					// begin render pass
					vkCmdBeginRenderPass(container->commandBufferContainer.buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					/* TODO: pushConstants */
					// pushConstants[0] = ...
					// vkCmdPushConstants(commandBuffers[i], &pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT,
					//		0, pushConstants.size(), pushConstants.data());

					/* DRAW COMMANDS */
					for (int _itr = 0; _itr < info.objectBindDatas.size(); _itr++) {
						// bind pipeline
						vkCmdBindPipeline(container->commandBufferContainer.buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, info.objectBindDatas[_itr].pipeline);

						// vertex buffer bind
						vkCmdBindVertexBuffers(container->commandBufferContainer.buffers[i], 0,
							info.objectBindDatas[_itr].vertexBuffers.size(), info.objectBindDatas[_itr].vertexBuffers.data(), offsets);

						// binding descriptor set(s)
						//vkCmdBindDescriptorSets(container->commandBufferContainer.buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
						//	info.objectBindDatas[_itr].pipelineLayout, _itr, info.objectBindDatas[_itr].descriptorSets.size(),
						//	info.objectBindDatas[_itr].descriptorSets.data(), 0, nullptr);
						vkCmdBindDescriptorSets(container->commandBufferContainer.buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
							info.objectBindDatas[_itr].pipelineLayout, 0, 1, &info.objectBindDatas[_itr].descriptorSets[i], 0, nullptr);

						// Render type selection
						if (info.objectBindDatas[_itr].renderingType == DMK_ADGR_RENDER_VERTEX) {		// Render as individual vertexes
							// draw command
							vkCmdDraw(container->commandBufferContainer.buffers[i], info.objectBindDatas[_itr].vertexCount, 1, 0, 1);
						}
						else if (info.objectBindDatas[_itr].renderingType == DMK_ADGR_RENDER_INDEXED) {		// Render as individual indexes
							// index buffer bind
							vkCmdBindIndexBuffer(container->commandBufferContainer.buffers[i], info.objectBindDatas[_itr].indexBuffer, 0, VK_INDEX_TYPE_UINT32);

							// draw command
							vkCmdDrawIndexed(container->commandBufferContainer.buffers[i], info.objectBindDatas[_itr].indexCount, 1, 0, 0, 0);
						}
						else if (info.objectBindDatas[_itr].renderingType == DMK_ADGR_RENDER_INDIRECT) {

						}
						else if (info.objectBindDatas[_itr].renderingType == DMK_ADGR_RENDER_INDEXED_INDIRECT) {

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
		}
	}
}