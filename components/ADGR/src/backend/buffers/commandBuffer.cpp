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
#include "vertexBuffer.h"
#include "backend/queues/queues.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			commandBuffer::commandBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
				VkCommandPool* commandPool) : Buffer(device, physicalDevice, commandPool) {
			}

			void commandBuffer::initBuffer(VkRenderPass renderPass, VkExtent2D swapChainExtent,
				VkPipeline graphicsPipeline) {
				mySize = (size_t)myFrameBuffer.size();
				myCommandBuffer.resize(mySize);

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = *myCommandPool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = (uint32)mySize;

				if (vkAllocateCommandBuffers(*myDevice, &allocInfo, myCommandBuffer.data()) != VK_SUCCESS)
					throw std::runtime_error("failed to allocate command buffers!");

				for (size_t i = 0; i < mySize; i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
					beginInfo.pInheritanceInfo = nullptr;

					if (vkBeginCommandBuffer(myCommandBuffer[i], &beginInfo) != VK_SUCCESS)
						throw std::runtime_error("failed to begin recording command buffer!");

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = renderPass;
					renderPassInfo.framebuffer = myFrameBuffer[i];

					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = swapChainExtent;

					VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
					renderPassInfo.clearValueCount = 1;
					renderPassInfo.pClearValues = &clearColor;

					vkCmdBeginRenderPass(myCommandBuffer[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					vkCmdBindPipeline(myCommandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

					VkBuffer vertexBuffers[] = { *myVertexBuffer };
					VkDeviceSize offsets[] = { 0 };
					vkCmdBindVertexBuffers(myCommandBuffer[i], 0, 1, vertexBuffers, offsets);

					vkCmdBindIndexBuffer(myCommandBuffer[i], *myIndexBuffer, 0, VK_INDEX_TYPE_UINT16);

					vkCmdDrawIndexed(myCommandBuffer[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

					vkCmdEndRenderPass(myCommandBuffer[i]);

					if (vkEndCommandBuffer(myCommandBuffer[i]) != VK_SUCCESS)
						throw std::runtime_error("failed to record command buffer!");
				}
			}

			void commandBuffer::initCommandPool(VkSurfaceKHR surface) {
				queueFamilyindices queueFamilyIndices = findQueueFamilies(*myPhysicalDevice, surface);

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