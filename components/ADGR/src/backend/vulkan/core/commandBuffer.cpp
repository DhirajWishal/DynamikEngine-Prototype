#include "adgrafx.h"
#include "commandBuffer.h"

#include "queues/queues.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			void commandBufferManager::createBuffer(VkBuffer* buffer, size_t size,
				VkBufferUsageFlags flags, VkBufferUsageFlagBits memoryFlags) {
			}

			void commandBufferManager::initCommandPool() {
				queueFamilyindices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

				VkCommandPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
				poolInfo.flags = 0;

				if (vkCreateCommandPool(*m_device, &poolInfo, nullptr, m_commandPool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create command pool!");
			}

			void commandBufferManager::bindCommands(DMKBindCommandBufferInfo info) {
				commandBuffers.resize(frameBuffers.size());

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = commandPool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = static_cast<uint32_t>(frameBuffers.size());

				if (vkAllocateCommandBuffers(*m_device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate command buffers!");

				for (size_t i = 0; i < frameBuffers.size(); i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

					if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to begin recording command buffer!");

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = renderPass;
					renderPassInfo.framebuffer = frameBuffers[i];
					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = swapChainExtent;

					std::array<VkClearValue, 2> clearValues = {};
					clearValues[0].color = {
						clearScreenValues[0],	// Red
						clearScreenValues[1],	// Green
						clearScreenValues[2],	// Blue
						clearScreenValues[3]	// Alpha
					};
					clearValues[1].depthStencil = { 1.0f, 0 };

					renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
					renderPassInfo.pClearValues = clearValues.data();

					/* BEGIN VULKAN COMMANDS */
					VkDeviceSize offsets[] = { 0 };
					// begin render pass
					vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					/* INITIALIZE SKYBOX */
					//vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
					//	0, 1, &info.skybox.descriptorSet, 0, NULL);
					//vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &info.skybox.vertexBuffer, offsets);
					//vkCmdBindIndexBuffer(commandBuffers[i], info.skybox.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
					//vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, info.skybox.pipeline);
					//vkCmdDrawIndexed(commandBuffers[i], info.skybox.indexes.size(), 1, 0, 0, 0);

					/* TODO: pushConstants */
					// pushConstants[0] = ...
					// vkCmdPushConstants(commandBuffers[i], &pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT,
					//		0, pushConstants.size(), pushConstants.data());

					// bind pipeline
					vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

					// vertex buffer bind
					for (int count = 0; count < info.vertexBuffers.size(); count++)
						vkCmdBindVertexBuffers(commandBuffers[i], count, 1,
							&info.vertexBuffers[count], offsets);

					// index buffer bind
					vkCmdBindIndexBuffer(commandBuffers[i], info.indexBuffer, 0, VK_INDEX_TYPE_UINT32);

					// binding descriptor set(s)
					for (int x = 0; x < info.descriptorSets.size(); x++) 
						vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
							pipelineLayout, x, 1, &info.descriptorSets[x]->at(i), 0, nullptr);

					// draw command
					vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32>(info.indices.size()), 1, 0, 0, 0);

					// end renderPass
					vkCmdEndRenderPass(commandBuffers[i]);

					/* END VULKAN COMMANDS */

					if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to record command buffer!");

					/*
					 Vulkan command (draw) order:
						begin render pass
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
