#include "adgrafx.h"
#include "VulkanCommandBuffer.h"

#include "VulkanFunctions.h"

#define GRID_DIM 7

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			// if drawing in vertex
			void drawVertex(VkCommandBuffer buffer, I32 index, ADGRVulkanRenderData* object, VkDeviceSize* offsets) {
				for (UI32 i = 0; i < object->vertexBuffers.size(); i++) {
					for (UI32 _itr = 0; _itr < object->pipelineContainers.size(); _itr++)
					{
						// bind pipeline
						vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineContainers[_itr].pipeline);

						// vertex buffer bind
						vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBuffers[i], offsets);

						// binding descriptor set(s)
						if (object->descriptors.descriptorSet != VK_NULL_HANDLE)
							vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineContainers[_itr].layout, 0, 1, &object->descriptors.descriptorSet, 0, nullptr);

						// draw command
						vkCmdDraw(buffer, object->vertexCount, 1, 0, 1);
					}
				}
			}

			// if drawing in indexed
			void drawIndexed(VkCommandBuffer buffer, I32 index, ADGRVulkanRenderData* object, VkDeviceSize* offsets) {
				for (UI32 i = 0; i < object->vertexBuffers.size(); i++) {
					for (UI32 _itr = 0; _itr < object->pipelineContainers.size(); _itr++)
					{
						// bind pipeline
						vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineContainers[_itr].pipeline);

						// vertex buffer bind
						vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBuffers[i], offsets);

						// binding descriptor set(s)
						if (object->descriptors.descriptorSet != VK_NULL_HANDLE)
							vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineContainers[_itr].layout, 0, 1, &object->descriptors.descriptorSet, 0, nullptr);

						// index buffer bind
						if (object->indexbufferObjectTypeSize == sizeof(UI8))
							vkCmdBindIndexBuffer(buffer, object->indexBuffers[i], 0, VK_INDEX_TYPE_UINT8_EXT);
						else if (object->indexbufferObjectTypeSize == sizeof(UI16))
							vkCmdBindIndexBuffer(buffer, object->indexBuffers[i], 0, VK_INDEX_TYPE_UINT16);
						else if (object->indexbufferObjectTypeSize == sizeof(UI32))
							vkCmdBindIndexBuffer(buffer, object->indexBuffers[i], 0, VK_INDEX_TYPE_UINT32);
						else if (object->indexbufferObjectTypeSize == sizeof(UI64))
							vkCmdBindIndexBuffer(buffer, object->indexBuffers[i], 0, VK_INDEX_TYPE_UINT32);

						// draw command
						vkCmdDrawIndexed(buffer, object->indexCount, 1, 0, 0, 0);
					}
				}
			}

			void drawIndexedPBR(VkCommandBuffer buffer, I32 index, ADGRVulkanRenderData* object, VkDeviceSize* offsets)
			{
				for (UI32 i = 0; i < object->vertexBuffers.size(); i++) {
					for (UI32 _itr = 0; _itr < object->pipelineContainers.size(); _itr++)
					{
						// bind pipeline
						vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineContainers[_itr].pipeline);

						// vertex buffer bind
						vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBuffers[i], offsets);

						// binding descriptor set(s)
						if (object->descriptors.descriptorSet != VK_NULL_HANDLE)
							vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineContainers[_itr].layout, 0, 1, &object->descriptors.descriptorSet, 0, nullptr);

						// index buffer bind
						if (object->indexbufferObjectTypeSize == sizeof(UI8))
							vkCmdBindIndexBuffer(buffer, object->indexBuffers[i], 0, VK_INDEX_TYPE_UINT8_EXT);
						else if (object->indexbufferObjectTypeSize == sizeof(UI16))
							vkCmdBindIndexBuffer(buffer, object->indexBuffers[i], 0, VK_INDEX_TYPE_UINT16);
						else if (object->indexbufferObjectTypeSize == sizeof(UI32))
							vkCmdBindIndexBuffer(buffer, object->indexBuffers[i], 0, VK_INDEX_TYPE_UINT32);
						else if (object->indexbufferObjectTypeSize == sizeof(UI64))
							vkCmdBindIndexBuffer(buffer, object->indexBuffers[i], 0, VK_INDEX_TYPE_UINT32);

						glm::vec3 pos = glm::vec3(0.0f, 0.0f, 1.0f);
						object->pushConstants[0].data = &pos;
						object->pushConstants[0].pushData(buffer, object->pipelineContainers[_itr].layout);
						object->pushConstants[1].data = &object->materialDescriptor.params;
						object->pushConstants[1].pushData(buffer, object->pipelineContainers[_itr].layout);
						vkCmdDrawIndexed(buffer, object->indexCount, 1, 0, 0, 0);
					}
				}
			}

			void VulkanCommandBuffer::initializeResources(ADGRVulkanCommandBufferInitResources info)
			{
				logicalDevice = info.logicalDevice;
				physicalDevice = info.physicalDevice;
				surface = info.surface;
			}

			void VulkanCommandBuffer::initializeCommandPool()
			{
				ADGRVulkanQueue queueFamilyIndices = VulkanCore::findQueueFamilies(physicalDevice, surface);

				VkCommandPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
				poolInfo.flags = 0;

				if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &pool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create command pool!");
			}

			void VulkanCommandBuffer::terminateCommandPool()
			{
				vkDestroyCommandPool(logicalDevice, pool, nullptr);
			}

			void VulkanCommandBuffer::initializeCommandBuffers(ADGRVulkanCommandBufferInitInfo info)
			{
				buffers.resize(info.count);

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = pool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = static_cast<UI32>(info.count);

				if (vkAllocateCommandBuffers(logicalDevice, &allocInfo, buffers.data()) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate command buffers!");

				for (size_t i = 0; i < info.count; i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

					if (vkBeginCommandBuffer(buffers[i], &beginInfo) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to begin recording command commandBuffers[i]!");

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = info.frameBuffer.renderPass;
					renderPassInfo.framebuffer = info.frameBuffer.buffers[i];
					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = info.swapChain.swapChainExtent;

					std::array<VkClearValue, 2> clearValues = {};

					clearValues[0].color = {
						info.clearValues[0],
						info.clearValues[1],
						info.clearValues[2],
						info.clearValues[3]
					};
					clearValues[1].depthStencil = { info.depthStencilDepth, info.stencilIndex };

					renderPassInfo.clearValueCount = static_cast<UI32>(clearValues.size());
					renderPassInfo.pClearValues = clearValues.data();

					/* BEGIN VULKAN COMMANDS */
					VkDeviceSize offsets[] = { 0 };
					// begin render pass
					vkCmdBeginRenderPass(buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					for (I32 _itr = 0; _itr < info.objects.size(); _itr++) {
						/* TODO: pushConstants */
						// pushConstants[0] = ...
						// vkCmdPushConstants(commandBuffers[i], &pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT,
						//		0, pushConstants.size(), pushConstants.data());
						//
						// Update light positions
						// w component = light radius scale

						/* DRAW COMMANDS */
						//if (info.objects.at(_itr).swapChainPointer->getPushConstantCount())
						//	vkCmdPushConstants(buffers[i], info.objects.at(_itr).pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, info.objects.at(_itr).swapChainPointer->pushConstants.size(), info.objects.at(_itr).swapChainPointer->pushConstants.data());

						// Render type selection
						if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_VERTEX) 		// Render as individual vertexes
							drawVertex(buffers[i], i, &info.objects.at(_itr), offsets);

						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED) 		// Render as individual indexes
						{
							if (info.objects.at(_itr).enableMaterials)
								drawIndexedPBR(buffers[i], i, &info.objects.at(_itr), offsets);
							else
								drawIndexed(buffers[i], i, &info.objects.at(_itr), offsets);
						}
						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDIRECT) {
						}
						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED_INDIRECT) {
						}
						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_VERTEX) {		// Render as individual vertexes
							drawVertex(buffers[i], i, &info.objects.at(_itr), offsets);
						}
						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDEXED) {		// Render as individual indexes
							drawIndexed(buffers[i], i, &info.objects.at(_itr), offsets);
						}
						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDIRECT) {
						}
						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDEXED_INDIRECT) {
						}
						else
							DMK_CORE_FATAL("Invalid rendering type!");
					}

					// end renderPass
					vkCmdEndRenderPass(buffers[i]);

					/* END VULKAN COMMANDS */
					if (vkEndCommandBuffer(buffers[i]) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to record command commandBuffers[i]!");

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
						end command commandBuffers[i]
					*/
				}
			}

			void VulkanCommandBuffer::terminateCommandBuffers()
			{
				vkFreeCommandBuffers(logicalDevice, pool, static_cast<UI32>(buffers.size()), buffers.data());
			}
		}
	}
}