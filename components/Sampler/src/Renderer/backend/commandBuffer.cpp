#include "smpafx.h"
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
					std::runtime_error("failed to create command pool!");
			}

			void commandBufferManager::bindCommands(DMKBindCommandBufferInfo info) {
				commandBuffers.resize(frameBuffers.size());

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = commandPool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = frameBuffers.size();

				if (vkAllocateCommandBuffers(*m_device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
					std::runtime_error("failed to allocate command buffers!");

				for (size_t i = 0; i < frameBuffers.size(); i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

					if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
						std::runtime_error("failed to begin recording command buffer!");

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = renderPass;
					renderPassInfo.framebuffer = frameBuffers[i];
					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = swapChainExtent;

					std::array<VkClearValue, 2> clearValues = {};
					clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
					clearValues[1].depthStencil = { 1.0f, 0 };

					renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
					renderPassInfo.pClearValues = clearValues.data();

					// draw command

					vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

					// vertex buffer bind

					//VkBuffer vertexBuffers[info.vertexBuffers.size()] = { vertexBuffer };
					VkDeviceSize offsets[] = { 0 };
					vkCmdBindVertexBuffers(commandBuffers[i], 0, info.vertexBuffers.size(),
						info.vertexBuffers.data(), offsets);

					// index buffer bind

					vkCmdBindIndexBuffer(commandBuffers[i], info.indexBuffer, 0, VK_INDEX_TYPE_UINT32);

					//vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
					//	pipelineLayout, 0, info.descriptorSets[i]->size(), info.descriptorSets[i]->data(), 0,
					//	nullptr);

					vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
						pipelineLayout, 0, 1, &info.descriptorSets[0]->at(i), 0,
						nullptr);

					vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32>(info.indices.size()), 1, 0, 0, 0);

					vkCmdEndRenderPass(commandBuffers[i]);

					if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
						std::runtime_error("failed to record command buffer!");

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
