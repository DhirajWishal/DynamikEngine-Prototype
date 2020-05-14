#include "dmkafx.h"
#include "VulkanGraphicsCommandBuffer.h"

#include "../Common/VulkanUtilities.h"

#define GRID_DIM 7

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			// if drawing in vertex
			void drawVertex(VkCommandBuffer buffer, I32 index, VulkanRenderData* object, VkDeviceSize* offsets)
			{
				DMK_BEGIN_PROFILE_TIMER();

				for (UI32 i = 0; i < object->renderObject.size(); i++) {
					for (UI32 _itr = 0; _itr < object->renderObject[i].vertexBufferContainer.size(); _itr++)
					{
						// bind pipeline
						vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->renderObject[_itr].pipeline.pipeline);

						// vertex buffer bind
						vkCmdBindVertexBuffers(buffer, 0, 1, &object->renderObject[i].vertexBufferContainer[_itr].buffer, offsets);

						// binding descriptor set(s)
						for (VkDescriptorSet _set : object->renderObject[i].descriptors.descriptorSets)
							vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->renderObject[i].pipeline.layout, 0, 1, &_set, 0, nullptr);

						// draw command
						vkCmdDraw(buffer, object->renderObject[i].vertexBufferContainer[_itr].dataCount, 1, 0, 1);
					}
				}
			}

			// if drawing in indexed
			void drawIndexed(VkCommandBuffer buffer, I32 index, VulkanRenderData* object, VkDeviceSize* offsets)
			{
				DMK_BEGIN_PROFILE_TIMER();

				for (UI32 i = 0; i < object->renderObject.size(); i++) {
					for (UI32 _itr = 0; _itr < object->renderObject[i].vertexBufferContainer.size(); _itr++)
					{
						// bind pipeline
						vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->renderObject[i].pipeline.pipeline);

						// vertex buffer bind
						vkCmdBindVertexBuffers(buffer, 0, 1, &object->renderObject[i].vertexBufferContainer[_itr].buffer, offsets);

						// binding descriptor set(s)
						for (VkDescriptorSet _set : object->renderObject[i].descriptors.descriptorSets)
							vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->renderObject[i].pipeline.layout, 0, 1, &_set, 0, nullptr);

						// index buffer bind
						if (object->renderObject[i].indexBufferType == DMKDataType::DMK_DATA_TYPE_UI8)
							vkCmdBindIndexBuffer(buffer, object->renderObject[i].indexBufferContainer[_itr].buffer, 0, VK_INDEX_TYPE_UINT8_EXT);
						else if (object->renderObject[i].indexBufferType == DMKDataType::DMK_DATA_TYPE_UI16)
							vkCmdBindIndexBuffer(buffer, object->renderObject[i].indexBufferContainer[_itr].buffer, 0, VK_INDEX_TYPE_UINT16);
						else if (object->renderObject[i].indexBufferType == DMKDataType::DMK_DATA_TYPE_UI32)
							vkCmdBindIndexBuffer(buffer, object->renderObject[i].indexBufferContainer[_itr].buffer, 0, VK_INDEX_TYPE_UINT32);
						else if (object->renderObject[i].indexBufferType == DMKDataType::DMK_DATA_TYPE_UI64)
							vkCmdBindIndexBuffer(buffer, object->renderObject[i].indexBufferContainer[_itr].buffer, 0, VK_INDEX_TYPE_UINT32);

						// draw command
						vkCmdDrawIndexed(buffer, object->renderObject[i].indexBufferContainer[_itr].dataCount, 1, 0, 0, 0);
					}
				}
			}

			void drawIndexedPBR(VkCommandBuffer buffer, I32 index, VulkanRenderData* object, VkDeviceSize* offsets)
			{
				DMK_BEGIN_PROFILE_TIMER();

				for (UI32 i = 0; i < object->renderObject.size(); i++) {
					for (UI32 _itr = 0; _itr < object->renderObject[i].vertexBufferContainer.size(); _itr++)
					{
						// bind pipeline
						vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->renderObject[i].pipeline.pipeline);

						// vertex buffer bind
						vkCmdBindVertexBuffers(buffer, 0, 1, &object->renderObject[i].vertexBufferContainer[_itr].buffer, offsets);

						// binding descriptor set(s)
						for (VkDescriptorSet _set : object->renderObject[i].descriptors.descriptorSets)
							vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->renderObject[_itr].pipeline.layout, 0, 1, &_set, 0, nullptr);

						// index buffer bind
						if (object->renderObject[i].indexBufferType == DMKDataType::DMK_DATA_TYPE_UI8)
							vkCmdBindIndexBuffer(buffer, object->renderObject[i].indexBufferContainer[_itr].buffer, 0, VK_INDEX_TYPE_UINT8_EXT);
						else if (object->renderObject[i].indexBufferType == DMKDataType::DMK_DATA_TYPE_UI16)
							vkCmdBindIndexBuffer(buffer, object->renderObject[i].indexBufferContainer[_itr].buffer, 0, VK_INDEX_TYPE_UINT16);
						else if (object->renderObject[i].indexBufferType == DMKDataType::DMK_DATA_TYPE_UI32)
							vkCmdBindIndexBuffer(buffer, object->renderObject[i].indexBufferContainer[_itr].buffer, 0, VK_INDEX_TYPE_UINT32);
						else if (object->renderObject[i].indexBufferType == DMKDataType::DMK_DATA_TYPE_UI64)
							vkCmdBindIndexBuffer(buffer, object->renderObject[i].indexBufferContainer[_itr].buffer, 0, VK_INDEX_TYPE_UINT32);

						// draw command
						vkCmdDrawIndexed(buffer, object->renderObject[i].indexBufferContainer[_itr].dataCount, 1, 0, 0, 0);
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

			void VulkanGraphicsCommandBuffer::initializeResources(VulkanGraphicsCommandBufferInitResources info)
			{
				logicalDevice = info.logicalDevice;
				physicalDevice = info.physicalDevice;
				surface = info.surface;
			}

			void VulkanGraphicsCommandBuffer::initializeCommandPool()
			{
				DMK_BEGIN_PROFILE_TIMER();

				VulkanQueue queueFamilyIndices = VulkanGraphicsCore::findQueueFamilies(physicalDevice, surface);

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

			void VulkanGraphicsCommandBuffer::initializeCommandBuffers(VulkanGraphicsCommandBufferInitInfo info)
			{
				DMK_BEGIN_PROFILE_TIMER();

				buffers.resize(info.count);
				if (info.captureFrames)
					_generateImages(info.count, info.swapChain.swapChainExtent);

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

					if (info.captureFrames)
						_beginCapture(buffers[i], i);

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = info.renderPass.renderPass;
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

					if (info.captureFrames)
						_endCapture(buffers[i], i);

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

			VulkanTextureContainer VulkanGraphicsCommandBuffer::getImage(UI32 index)
			{
				if (index > (images.size() - 1))
				{
					DMK_CORE_FATAL("Invalid image index requested! Maybe the image capture is not set.");
					return VulkanTextureContainer();
				}

				return images[index];
			}

			void VulkanGraphicsCommandBuffer::_generateImages(UI32 count, VkExtent2D extent)
			{
				while (count--)
				{
					VulkanTextureContainer _container;
					_container.format = VK_FORMAT_R8G8B8A8_UINT;
					_container.width = extent.width;
					_container.height = extent.height;

					VulkanCreateImageInfo cinfo;
					cinfo.width = extent.width;
					cinfo.height = extent.height;
					cinfo.format = _container.format;
					cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
					cinfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
					cinfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
					cinfo.image = &_container.image;
					cinfo.imageMemory = &_container.imageMemory;
					cinfo.mipLevels = 1;
					cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
					cinfo.arrayLayers = 1;
					cinfo.flags = NULL;
					VulkanUtilities::createImage(logicalDevice, physicalDevice, cinfo);

					VulkanCreateImageViewInfo cinfo2;
					cinfo2.image = _container.image;
					cinfo2.format = _container.format;
					cinfo2.mipLevels = VK_REMAINING_MIP_LEVELS;
					cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					cinfo2.viewType = VK_IMAGE_VIEW_TYPE_2D;
					cinfo2.layerCount = VK_REMAINING_ARRAY_LAYERS;
					cinfo2.component = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
					_container.imageView = VulkanUtilities::createImageView(logicalDevice, cinfo2);

					images.push_back(_container);
				}
			}

			void VulkanGraphicsCommandBuffer::_beginCapture(VkCommandBuffer commandBuffer, UI32 index)
			{
				VkImageMemoryBarrier _barrier;
				_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				_barrier.pNext = VK_NULL_HANDLE;
				_barrier.srcAccessMask = VK_NULL_HANDLE;
				_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				_barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				_barrier.image = images[index].image;
				_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				_barrier.subresourceRange.baseMipLevel = 0;
				_barrier.subresourceRange.levelCount = 1;
				_barrier.subresourceRange.baseArrayLayer = 0;
				_barrier.subresourceRange.layerCount = 1;

				vkCmdPipelineBarrier(
					commandBuffer,
					VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
					0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &_barrier);
			}

			void VulkanGraphicsCommandBuffer::_endCapture(VkCommandBuffer commandBuffer, UI32 index)
			{
				VkImageMemoryBarrier _barrier;
				_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				_barrier.pNext = VK_NULL_HANDLE;
				_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				_barrier.dstAccessMask = VK_ACCESS_HOST_READ_BIT | VK_ACCESS_MEMORY_READ_BIT;
				_barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				_barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
				_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				_barrier.image = images[index].image;
				_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				_barrier.subresourceRange.baseMipLevel = 0;
				_barrier.subresourceRange.levelCount = 1;
				_barrier.subresourceRange.baseArrayLayer = 0;
				_barrier.subresourceRange.layerCount = 1;

				vkCmdPipelineBarrier(
					commandBuffer,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
					VK_PIPELINE_STAGE_HOST_BIT,
					0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &_barrier);
			}
		}
	}
}