#include "dmkafx.h"
#include "VulkanGraphicsCommandBuffer.h"

#include "../Common/VulkanUtilities.h"

#define GRID_DIM 7

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			// if drawing in vertex
			void drawVertex(VkCommandBuffer buffer, I32 index, ADGRVulkanRenderData* object, VkDeviceSize* offsets) 
			{
				DMK_BEGIN_PROFILE_TIMER();

				for (UI32 i = 0; i < object->vertexBufferContainer.size(); i++) {
					for (UI32 _itr = 0; _itr < object->pipelineContainers.size(); _itr++)
					{
						// bind pipeline
						vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineContainers[_itr].pipeline);

						// vertex buffer bind
						vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBufferContainer[i].buffer, offsets);

						// binding descriptor set(s)
						for (VkDescriptorSet _set : object->descriptors.descriptorSets)
							vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineContainers[_itr].layout, 0, 1, &_set, 0, nullptr);

						// draw command
						vkCmdDraw(buffer, object->vertexBufferContainer[i].dataCount, 1, 0, 1);
					}
				}
			}

			// if drawing in indexed
			void drawIndexed(VkCommandBuffer buffer, I32 index, ADGRVulkanRenderData* object, VkDeviceSize* offsets) 
			{
				DMK_BEGIN_PROFILE_TIMER();

				for (UI32 i = 0; i < object->vertexBufferContainer.size(); i++) {
					for (UI32 _itr = 0; _itr < object->pipelineContainers.size(); _itr++)
					{
						// bind pipeline
						vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineContainers[_itr].pipeline);

						// vertex buffer bind
						vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBufferContainer[i].buffer, offsets);

						// binding descriptor set(s)
						for (VkDescriptorSet _set : object->descriptors.descriptorSets)
							vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineContainers[_itr].layout, 0, 1, &_set, 0, nullptr);

						// index buffer bind
						if (object->indexBufferType == DMKDataType::DMK_DATA_TYPE_UI8)
							vkCmdBindIndexBuffer(buffer, object->indexBufferContainer[i].buffer, 0, VK_INDEX_TYPE_UINT8_EXT);
						else if (object->indexBufferType == DMKDataType::DMK_DATA_TYPE_UI16)
							vkCmdBindIndexBuffer(buffer, object->indexBufferContainer[i].buffer, 0, VK_INDEX_TYPE_UINT16);
						else if (object->indexBufferType == DMKDataType::DMK_DATA_TYPE_UI32)
							vkCmdBindIndexBuffer(buffer, object->indexBufferContainer[i].buffer, 0, VK_INDEX_TYPE_UINT32);
						else if (object->indexBufferType == DMKDataType::DMK_DATA_TYPE_UI64)
							vkCmdBindIndexBuffer(buffer, object->indexBufferContainer[i].buffer, 0, VK_INDEX_TYPE_UINT32);

						// draw command
						vkCmdDrawIndexed(buffer, object->indexBufferContainer[i].dataCount, 1, 0, 0, 0);
					}
				}
			}

			void drawIndexedPBR(VkCommandBuffer buffer, I32 index, ADGRVulkanRenderData* object, VkDeviceSize* offsets)
			{
				DMK_BEGIN_PROFILE_TIMER();

				for (UI32 i = 0; i < object->vertexBufferContainer.size(); i++) {
					for (UI32 _itr = 0; _itr < object->pipelineContainers.size(); _itr++)
					{
						// bind pipeline
						vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineContainers[_itr].pipeline);

						// vertex buffer bind
						vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBufferContainer[i].buffer, offsets);

						// binding descriptor set(s)
						for (VkDescriptorSet _set : object->descriptors.descriptorSets)
							vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineContainers[_itr].layout, 0, 1, &_set, 0, nullptr);

						// index buffer bind
						if (object->indexBufferType == DMKDataType::DMK_DATA_TYPE_UI8)
							vkCmdBindIndexBuffer(buffer, object->indexBufferContainer[i].buffer, 0, VK_INDEX_TYPE_UINT8_EXT);
						else if (object->indexBufferType == DMKDataType::DMK_DATA_TYPE_UI16)
							vkCmdBindIndexBuffer(buffer, object->indexBufferContainer[i].buffer, 0, VK_INDEX_TYPE_UINT16);
						else if (object->indexBufferType == DMKDataType::DMK_DATA_TYPE_UI32)
							vkCmdBindIndexBuffer(buffer, object->indexBufferContainer[i].buffer, 0, VK_INDEX_TYPE_UINT32);
						else if (object->indexBufferType == DMKDataType::DMK_DATA_TYPE_UI64)
							vkCmdBindIndexBuffer(buffer, object->indexBufferContainer[i].buffer, 0, VK_INDEX_TYPE_UINT32);

						vkCmdDrawIndexed(buffer, object->indexBufferContainer[i].dataCount, 1, 0, 0, 0);
						//for (uint32_t y = 0; y < GRID_DIM; y++) {
						//	object->materialDescriptor.params.metallic = (float)y / (float)(GRID_DIM);
						//	for (uint32_t x = 0; x < GRID_DIM; x++) {
						//		glm::vec3 pos = glm::vec3(float(x - (GRID_DIM / 2.0f)) * 2.5f, 0.0f, float(y - (GRID_DIM / 2.0f)) * 2.5f);
						//		object->pushConstants[0].data = &pos;
						//		object->pushConstants[0].pushData(buffer, object->pipelineContainers[_itr].layout);
						//		object->materialDescriptor.params.roughness = glm::clamp((float)x / (float)(GRID_DIM), 0.05f, 1.0f);
						//		object->pushConstants[1].data = &object->materialDescriptor.params;
						//		object->pushConstants[1].pushData(buffer, object->pipelineContainers[_itr].layout);
						//	}
						//}


						//glm::vec3 pos = glm::vec3(0.0f, 0.0f, 1.0f);
						//object->pushConstants[0].data = &pos;
						//object->pushConstants[0].pushData(buffer, object->pipelineContainers[_itr].layout);
						//object->pushConstants[1].data = &object->materialDescriptor.params;
						//object->pushConstants[1].pushData(buffer, object->pipelineContainers[_itr].layout);
						//vkCmdDrawIndexed(buffer, object->indexCount, 1, 0, 0, 0);
					}
				}
			}

			void VulkanGraphicsCommandBuffer::initializeResources(ADGRVulkanGraphicsCommandBufferInitResources info)
			{
				logicalDevice = info.logicalDevice;
				physicalDevice = info.physicalDevice;
				surface = info.surface;
			}

			void VulkanGraphicsCommandBuffer::initializeCommandPool()
			{
				DMK_BEGIN_PROFILE_TIMER();

				ADGRVulkanQueue queueFamilyIndices = VulkanGraphicsCore::findQueueFamilies(physicalDevice, surface);

				VkCommandPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
				poolInfo.flags = 0;

				if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &pool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create command pool!");
			}

			void VulkanGraphicsCommandBuffer::terminateCommandPool()
			{
				vkDestroyCommandPool(logicalDevice, pool, nullptr);
			}

			void VulkanGraphicsCommandBuffer::initializeCommandBuffers(ADGRVulkanGraphicsCommandBufferInitInfo info)
			{
				DMK_BEGIN_PROFILE_TIMER();

				buffers.resize(info.count);

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = pool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = static_cast<UI32>(info.count);

				if (vkAllocateCommandBuffers(logicalDevice, &allocInfo, buffers.data()) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate command buffers!");

				for (size_t i = 0; i < info.count; i++) {
					DMK_BEGIN_PROFILE_TIMER();

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

					for (I32 _itr = 0; _itr < info.objects.size(); _itr++) 
					{
						DMK_BEGIN_PROFILE_TIMER();

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
						if (info.objects.at(_itr).renderTechnology == DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_VERTEX) 		// Render as individual vertexes
							drawVertex(buffers[i], i, &info.objects.at(_itr), offsets);

						else if (info.objects.at(_itr).renderTechnology == DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_INDEXED) 		// Render as individual indexes
						{
							if (info.objects.at(_itr).enableMaterials)
								drawIndexedPBR(buffers[i], i, &info.objects.at(_itr), offsets);
							else
								drawIndexed(buffers[i], i, &info.objects.at(_itr), offsets);
						}
						else if (info.objects.at(_itr).renderTechnology == DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_INDIRECT) {
						}
						else if (info.objects.at(_itr).renderTechnology == DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_INDEXED_INDIRECT) {
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

			void VulkanGraphicsCommandBuffer::terminateCommandBuffers()
			{
				vkFreeCommandBuffers(logicalDevice, pool, static_cast<UI32>(buffers.size()), buffers.data());
			}
		}
	}
}