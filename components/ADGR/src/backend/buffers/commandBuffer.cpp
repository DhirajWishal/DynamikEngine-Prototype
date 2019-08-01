/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "commandBuffer.h"
#include "indexBuffer.h"
#include "backend/queues/queues.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			commandBuffer::commandBuffer(VkDevice* device, VkCommandPool* commandPool) :
				myDevice(device), myCommandPool(commandPool) {
			}

			void commandBuffer::initBuffer(VkCommandPool commandPool, VkRenderPass renderPass,
				std::vector<VkFramebuffer> frameBuff, VkExtent2D swapChainExtent, VkPipeline graphicsPipeline,
				VkBuffer vertexBuffer, VkBuffer indexBuffer) {
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
					beginInfo.pInheritanceInfo = nullptr;

					if (vkBeginCommandBuffer(commandBuff[i], &beginInfo) != VK_SUCCESS)
						throw std::runtime_error("failed to begin recording command buffer!");

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = renderPass;
					renderPassInfo.framebuffer = frameBuff[i];
					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = swapChainExtent;

					VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
					renderPassInfo.clearValueCount = 1;
					renderPassInfo.pClearValues = &clearColor;

					vkCmdBeginRenderPass(commandBuff[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					vkCmdBindPipeline(commandBuff[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

					VkBuffer vertexBuffers[] = { vertexBuffer };
					VkDeviceSize offsets[] = { 0 };
					vkCmdBindVertexBuffers(commandBuff[i], 0, 1, vertexBuffers, offsets);

					vkCmdBindIndexBuffer(commandBuff[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);

					vkCmdDrawIndexed(commandBuff[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

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
		}
	}
}