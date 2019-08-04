/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "commandBuffer.h"
#include "backend/queues/queues.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			commandBuffer::commandBuffer(VkDevice* device, VkCommandPool* commandPool) :
				myDevice(device), myCommandPool(commandPool) {
			}

			void commandBuffer::initBuffer(VkCommandPool commandPool, VkRenderPass renderPass,
				std::vector<VkFramebuffer> frameBuff, VkExtent2D swapChainExtent, VkPipeline graphicsPipeline,
				VkBuffer vertexBuffer, VkBuffer indexBuffer, VkPipelineLayout pipelineLayout,
				std::vector<VkDescriptorSet> descriptorSet, model myModel) {
				mySize = frameBuff.size();
				commandBuff.resize(mySize);

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = commandPool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = mySize;

				if (vkAllocateCommandBuffers(*myDevice, &allocInfo, commandBuff.data()) != VK_SUCCESS)
					throw std::runtime_error("failed to allocate command buffers!");

				for (size_t i = 0; i < mySize; i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

					if (vkBeginCommandBuffer(commandBuff[i], &beginInfo) != VK_SUCCESS)
						throw std::runtime_error("failed to begin recording command buffer!");

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = renderPass;
					renderPassInfo.framebuffer = frameBuff[i];
					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = swapChainExtent;

					std::array<VkClearValue, 2> clearValues = {};
					clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
					clearValues[1].depthStencil = { 1.0f, 0 };

					renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
					renderPassInfo.pClearValues = clearValues.data();

					vkCmdBeginRenderPass(commandBuff[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					vkCmdBindPipeline(commandBuff[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

					VkBuffer vertexBuffers[] = { vertexBuffer };
					VkDeviceSize offsets[] = { 0 };
					vkCmdBindVertexBuffers(commandBuff[i], 0, 1, vertexBuffers, offsets);

					vkCmdBindIndexBuffer(commandBuff[i], indexBuffer, 0, VK_INDEX_TYPE_UINT32);

					vkCmdBindDescriptorSets(commandBuff[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
						pipelineLayout, 0, 1, &descriptorSet[i], 0, nullptr);

					vkCmdDrawIndexed(commandBuff[i], static_cast<uint32>(myModel.getIndeices().size()), 1, 0, 0, 0);

					vkCmdEndRenderPass(commandBuff[i]);

					if (vkEndCommandBuffer(commandBuff[i]) != VK_SUCCESS)
						throw std::runtime_error("failed to record command buffer!");
				}
			}

			void commandBuffer::initCommandPool(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
				queueFamilyindices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

				VkCommandPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
				poolInfo.flags = 0;

				if (vkCreateCommandPool(*myDevice, &poolInfo, nullptr, myCommandPool) != VK_SUCCESS)
					throw std::runtime_error("failed to create command pool!");
			}

			VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool) {
				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandPool = commandPool;
				allocInfo.commandBufferCount = 1;

				VkCommandBuffer commandBuffer;
				vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

				vkBeginCommandBuffer(commandBuffer, &beginInfo);

				return commandBuffer;
			}

			void endSingleTimeCommands(VkDevice device, VkCommandPool& commandPool, VkCommandBuffer commandBuffer,
				VkQueue graphicsQueue) {
				vkEndCommandBuffer(commandBuffer);

				VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &commandBuffer;

				vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
				vkQueueWaitIdle(graphicsQueue);

				vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
			}
		}
	}
}