#include "adgrafx.h"
#include "VulkanBufferFunctions.h"

#include "core/vulkan/Renderer Backend Layer/Managers/VulkanFunctions.h"
#include "core/vulkan/Renderer Backend Layer/Managers/VulkanQueueManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			/* HELPER FUNCTIONS */
			/* COMMAND POOL AND BUFFER */
			// if drawing in vertex
			void drawVertex(VkCommandBuffer buffer, I32 index, VulkanRenderObject* object, VkDeviceSize* offsets) {
				for (UI32 i = 0; i < object->vertexBuffers.size(); i++) {
					// bind pipeline
					vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipeline);

					// vertex buffer bind
					vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBuffers[i].buffer, offsets);

					// binding descriptor set(s)
					if (object->descriptorSets.size())
						vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineLayout, 0, 1, &object->descriptorSets[i][index], 0, nullptr);

					// draw command
					vkCmdDraw(buffer, object->vertexBuffers[i].count, 1, 0, 1);
				}
			}

			// if drawing in indexed
			void drawIndexed(VkCommandBuffer buffer, I32 index, VulkanRenderObject* object, VkDeviceSize* offsets) {
				for (UI32 i = 0; i < object->vertexBuffers.size(); i++) {
					// bind pipeline
					vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipeline);

					// vertex buffer bind
					vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBuffers[i].buffer, offsets);

					// binding descriptor set(s)
					if (object->descriptorSets.size())
						vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineLayout, 0, 1, &object->descriptorSets[i][index], 0, nullptr);

					// index buffer bind
					if (object->indexBuffers[i].typeSize = sizeof(UI8))
						vkCmdBindIndexBuffer(buffer, object->indexBuffers[i].buffer, 0, VK_INDEX_TYPE_UINT8_EXT);
					else if (object->indexBuffers[i].typeSize = sizeof(UI16))
						vkCmdBindIndexBuffer(buffer, object->indexBuffers[i].buffer, 0, VK_INDEX_TYPE_UINT16);
					else if (object->indexBuffers[i].typeSize = sizeof(UI32))
						vkCmdBindIndexBuffer(buffer, object->indexBuffers[i].buffer, 0, VK_INDEX_TYPE_UINT32);
					else if (object->indexBuffers[i].typeSize = sizeof(UI64))
						vkCmdBindIndexBuffer(buffer, object->indexBuffers[i].buffer, 0, VK_INDEX_TYPE_UINT32);

					// draw command
					vkCmdDrawIndexed(buffer, object->indexBuffers[i].count, 1, 0, 0, 0);
				}
			}

			/* CLASS DEFINITIONS */
						/* COMMAND POOL AND BUFFER */
			void VulkanBufferFunctions::initializeCommandPool(POINTER<VulkanCoreObject> core)
			{
				VulkanQueueManager queueFamilyIndices = VulkanQueueManager::findQueueFamilies(core);

				VkCommandPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
				poolInfo.flags = 0;

				if (vkCreateCommandPool(core->logicalDevice, &poolInfo, nullptr, &core->commandPool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create command pool!");
			}

			void VulkanBufferFunctions::initializeCommandBuffer(POINTER<VulkanCoreObject> core, ADGRVulkanCommandBufferInitInfo info)
			{
				core->commandBuffers.resize(core->frameBuffers.size());

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = core->commandPool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = static_cast<UI32>(core->frameBuffers.size());

				if (vkAllocateCommandBuffers(core->logicalDevice, &allocInfo, core->commandBuffers.data()) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate command buffers!");

				for (size_t i = 0; i < core->frameBuffers.size(); i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

					if (vkBeginCommandBuffer(core->commandBuffers[i], &beginInfo) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to begin recording command buffer!");

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = core->renderPass;
					renderPassInfo.framebuffer = core->frameBuffers[i];
					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = core->swapChainExtent;

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
					vkCmdBeginRenderPass(core->commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					/* TODO: pushConstants */
					// pushConstants[0] = ...
					// vkCmdPushConstants(commandBuffers[i], &pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT,
					//		0, pushConstants.size(), pushConstants.data());
					//
					// Update light positions
					// w component = light radius scale

					/* DRAW COMMANDS */
					for (I32 _itr = 0; _itr < info.objects->size(); _itr++) {
						vkCmdPushConstants(core->commandBuffers[i], info.objects->at(_itr).pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, info.objects->at(_itr).pushConstants.size(), info.objects->at(_itr).pushConstants.data());

						// Render type selection
						if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_VERTEX) 		// Render as individual vertexes
							drawVertex(core->commandBuffers[i], i, &info.objects->at(_itr), offsets);

						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED) 		// Render as individual indexes
							drawIndexed(core->commandBuffers[i], i, &info.objects->at(_itr), offsets);

						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDIRECT) {
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED_INDIRECT) {
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_VERTEX) {		// Render as individual vertexes
							for (I32 x = 0; x < info.objects->at(_itr).vertexBuffers.size(); x++) {
								// bind pipeline
								vkCmdBindPipeline(core->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, info.objects->at(_itr).pipeline);

								// vertex buffer bind
								vkCmdBindVertexBuffers(core->commandBuffers[i], 0, 1, &info.objects->at(_itr).vertexBuffers[x].buffer, offsets);

								// binding descriptor set(s)
								if (info.objects->at(_itr).descriptorSets.size() > 0)
									vkCmdBindDescriptorSets(core->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
										info.objects->at(_itr).pipelineLayout, 0, 1, &info.objects->at(_itr).descriptorSets[x][i], 0, nullptr);

								// draw command
								vkCmdDraw(core->commandBuffers[i], info.objects->at(_itr).indexBuffers[x].count, 1, 0, 1);
							}
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDEXED) {		// Render as individual indexes
							for (I32 x = 0; x < info.objects->at(_itr).vertexBuffers.size(); x++) {
								// bind pipeline
								vkCmdBindPipeline(core->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, info.objects->at(_itr).pipeline);

								// vertex buffer bind
								vkCmdBindVertexBuffers(core->commandBuffers[i], 0, 1, &info.objects->at(_itr).vertexBuffers[x].buffer, offsets);

								// binding descriptor set(s)
								if (info.objects->at(_itr).descriptorSets.size() > 0)
									vkCmdBindDescriptorSets(core->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
										info.objects->at(_itr).pipelineLayout, 0, 1, &info.objects->at(_itr).descriptorSets[x][i], 0, nullptr);

								// index buffer bind
								if (info.objects->at(_itr).indexBuffers[x].typeSize = sizeof(UI8))
									vkCmdBindIndexBuffer(core->commandBuffers[i], info.objects->at(_itr).indexBuffers[x].buffer, 0, VK_INDEX_TYPE_UINT8_EXT);
								else if (info.objects->at(_itr).indexBuffers[x].typeSize = sizeof(UI16))
									vkCmdBindIndexBuffer(core->commandBuffers[i], info.objects->at(_itr).indexBuffers[x].buffer, 0, VK_INDEX_TYPE_UINT16);
								else if (info.objects->at(_itr).indexBuffers[x].typeSize = sizeof(UI32))
									vkCmdBindIndexBuffer(core->commandBuffers[i], info.objects->at(_itr).indexBuffers[x].buffer, 0, VK_INDEX_TYPE_UINT32);
								else if (info.objects->at(_itr).indexBuffers[x].typeSize = sizeof(UI64))
									vkCmdBindIndexBuffer(core->commandBuffers[i], info.objects->at(_itr).indexBuffers[x].buffer, 0, VK_INDEX_TYPE_UINT32);

								// draw command
								vkCmdDrawIndexed(core->commandBuffers[i], info.objects->at(_itr).indexBuffers[x].count, 1, 0, 0, 0);
							}
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDIRECT) {
						}
						else if (info.objects->at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDEXED_INDIRECT) {
						}
						else
							DMK_CORE_FATAL("Invalid rendering type!");
					}

					// end renderPass
					vkCmdEndRenderPass(core->commandBuffers[i]);

					/* END VULKAN COMMANDS */
					if (vkEndCommandBuffer(core->commandBuffers[i]) != VK_SUCCESS)
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

			void VulkanBufferFunctions::terminateCommandBuffer(POINTER<VulkanCoreObject> core)
			{
				vkFreeCommandBuffers(core->logicalDevice, core->commandPool, static_cast<UI32>(core->commandBuffers.size()), core->commandBuffers.data());
				vkDestroyCommandPool(core->logicalDevice, core->commandPool, nullptr);
			}

			/* FRAME BUFFER */
			void VulkanBufferFunctions::initializeFrameBuffer(POINTER<VulkanCoreObject> core, ADGRVulkanFrameBufferInitInfo info)
			{
				core->frameBuffers.resize(core->swapChainImageViews.size());

				for (size_t i = 0; i < core->swapChainImageViews.size(); i++)
				{
					ARRAY<VkImageView> attachments;
					for (VkImageView _imageView : info.preAttachments)
						attachments.push_back(_imageView);

					attachments.push_back(core->swapChainImageViews[i]);

					for (VkImageView _imageView : info.additionalAttachments)
						attachments.push_back(_imageView);

					VkFramebufferCreateInfo framebufferInfo = {};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = core->renderPass;
					framebufferInfo.attachmentCount = static_cast<UI32>(attachments.size());
					framebufferInfo.pAttachments = attachments.data();
					framebufferInfo.width = core->swapChainExtent.width;
					framebufferInfo.height = core->swapChainExtent.height;
					framebufferInfo.layers = 1;

					if (vkCreateFramebuffer(core->logicalDevice, &framebufferInfo, nullptr, &core->frameBuffers[i]) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create framebuffer!");
				}
			}

			void VulkanBufferFunctions::terminateFrameBuffer(POINTER<VulkanCoreObject> core)
			{
				for (VkFramebuffer buffer : core->frameBuffers)
					vkDestroyFramebuffer(core->logicalDevice, buffer, nullptr);

				core->frameBuffers.clear();
			}

			/* VERTEX BUFFER */
			void VulkanBufferFunctions::initializeVertexBuffer(POINTER<VulkanCoreObject> core, ARRAY<Vertex>* vertexes, POINTER<VulkanRenderObject> object)
			{
				ADGRVulkanVertexBufferContainer _container;
				_container.count = vertexes->size();
				VkDeviceSize bufferSize = _container.count * vertexes->typeSize();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanFunctions::createBuffer(core, bufferInfo);

				void* data = nullptr;
				vkMapMemory(core->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, vertexes->data(), (size_t)bufferSize);
				vkUnmapMemory(core->logicalDevice, stagingBufferMemory);

				ADGRCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &_container.buffer;
				vertBufferInfo.bufferMemory = &_container.bufferMemory;

				VulkanFunctions::createBuffer(core, vertBufferInfo);

				VulkanFunctions::copyBuffer(core, stagingBuffer, _container.buffer, bufferSize);

				vkDestroyBuffer(core->logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(core->logicalDevice, stagingBufferMemory, nullptr);

				object->vertexBuffers.pushBack(_container);
			}

			void VulkanBufferFunctions::terminateVertexBuffer(POINTER<VulkanCoreObject> core, ADGRVulkanVertexBufferContainer container)
			{
				vkDestroyBuffer(core->logicalDevice, container.buffer, nullptr);
				vkFreeMemory(core->logicalDevice, container.bufferMemory, nullptr);
			}

			/* INDEX BUFFER */
			void VulkanBufferFunctions::initializeIndexBufferUI8(POINTER<VulkanCoreObject> core, ARRAY<UI8>* indexes, POINTER<VulkanRenderObject> object)
			{
				ADGRVulkanIndexBufferContainer _container;
				_container.count = indexes->size();
				VkDeviceSize bufferSize = _container.count * indexes->typeSize();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanFunctions::createBuffer(core, bufferInfo);

				void* data = nullptr;
				vkMapMemory(core->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(core->logicalDevice, stagingBufferMemory);

				ADGRCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &_container.buffer;
				vertBufferInfo.bufferMemory = &_container.bufferMemory;

				VulkanFunctions::createBuffer(core, vertBufferInfo);

				VulkanFunctions::copyBuffer(core, stagingBuffer, _container.buffer, bufferSize);

				vkDestroyBuffer(core->logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(core->logicalDevice, stagingBufferMemory, nullptr);

				_container.typeSize = indexes->typeSize();
				object->indexBuffers.pushBack(_container);
			}

			void VulkanBufferFunctions::initializeIndexBufferUI16(POINTER<VulkanCoreObject> core, ARRAY<UI16>* indexes, POINTER<VulkanRenderObject> object)
			{
				ADGRVulkanIndexBufferContainer _container;
				_container.count = indexes->size();
				VkDeviceSize bufferSize = _container.count * indexes->typeSize();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanFunctions::createBuffer(core, bufferInfo);

				void* data = nullptr;
				vkMapMemory(core->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(core->logicalDevice, stagingBufferMemory);

				ADGRCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &_container.buffer;
				vertBufferInfo.bufferMemory = &_container.bufferMemory;

				VulkanFunctions::createBuffer(core, vertBufferInfo);

				VulkanFunctions::copyBuffer(core, stagingBuffer, _container.buffer, bufferSize);

				vkDestroyBuffer(core->logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(core->logicalDevice, stagingBufferMemory, nullptr);

				_container.typeSize = indexes->typeSize();
				object->indexBuffers.pushBack(_container);
			}

			void VulkanBufferFunctions::initializeIndexBufferUI32(POINTER<VulkanCoreObject> core, ARRAY<UI32>* indexes, POINTER<VulkanRenderObject> object)
			{
				ADGRVulkanIndexBufferContainer _container;
				_container.count = indexes->size();
				VkDeviceSize bufferSize = _container.count * indexes->typeSize();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanFunctions::createBuffer(core, bufferInfo);

				void* data = nullptr;
				vkMapMemory(core->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(core->logicalDevice, stagingBufferMemory);

				ADGRCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &_container.buffer;
				vertBufferInfo.bufferMemory = &_container.bufferMemory;

				VulkanFunctions::createBuffer(core, vertBufferInfo);

				VulkanFunctions::copyBuffer(core, stagingBuffer, _container.buffer, bufferSize);

				vkDestroyBuffer(core->logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(core->logicalDevice, stagingBufferMemory, nullptr);

				_container.typeSize = indexes->typeSize();
				object->indexBuffers.pushBack(_container);
			}

			void VulkanBufferFunctions::initializeIndexBufferUI64(POINTER<VulkanCoreObject> core, ARRAY<UI64>* indexes, POINTER<VulkanRenderObject> object)
			{
				ADGRVulkanIndexBufferContainer _container;
				_container.count = indexes->size();
				VkDeviceSize bufferSize = _container.count * indexes->typeSize();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanFunctions::createBuffer(core, bufferInfo);

				void* data = nullptr;
				vkMapMemory(core->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(core->logicalDevice, stagingBufferMemory);

				ADGRCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &_container.buffer;
				vertBufferInfo.bufferMemory = &_container.bufferMemory;

				VulkanFunctions::createBuffer(core, vertBufferInfo);

				VulkanFunctions::copyBuffer(core, stagingBuffer, _container.buffer, bufferSize);

				vkDestroyBuffer(core->logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(core->logicalDevice, stagingBufferMemory, nullptr);

				_container.typeSize = indexes->typeSize();
				object->indexBuffers.pushBack(_container);
			}

			void VulkanBufferFunctions::terminateIndexBuffer(POINTER<VulkanCoreObject> core, ADGRVulkanIndexBufferContainer container)
			{
				vkDestroyBuffer(core->logicalDevice, container.buffer, nullptr);
				vkFreeMemory(core->logicalDevice, container.bufferMemory, nullptr);
			}

			/* UNIFORM BUFFER */
			void VulkanBufferFunctions::initializeUniformBuffer(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object)
			{
				VkDeviceSize bufferSize = sizeof(UniformBufferObject);

				object->uniformBuffers.resize(core->swapChainImages.size());
				object->uniformBufferMemories.resize(core->swapChainImages.size());

				for (size_t i = 0; i < core->swapChainImages.size(); i++) {
					ADGRCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = bufferSize;
					bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &object->uniformBuffers[i];
					bufferInfo.bufferMemory = &object->uniformBufferMemories[i];

					VulkanFunctions::createBuffer(core, bufferInfo);
				}
			}

			void VulkanBufferFunctions::updateUniformBuffer(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object, UniformBufferObject uniformBuferObject, UI32 currentImage)
			{
				void* data = nullptr;
				vkMapMemory(core->logicalDevice, object->uniformBufferMemories[currentImage], 0, sizeof(uniformBuferObject), 0, &data);
				memcpy(data, &uniformBuferObject, sizeof(uniformBuferObject));
				vkUnmapMemory(core->logicalDevice, object->uniformBufferMemories[currentImage]);
			}

			void VulkanBufferFunctions::terminateUniformBuffer(POINTER<VulkanCoreObject> core, VulkanRenderObject object)
			{
				for (I32 x = 0; x < object.uniformBuffers.size(); x++) {
					vkDestroyBuffer(core->logicalDevice, object.uniformBuffers[x], nullptr);
					vkFreeMemory(core->logicalDevice, object.uniformBufferMemories[x], nullptr);
				}
			}
			
			/* COLOR BUFFER */
			ADGRVulkanColorBufferContainer VulkanBufferFunctions::initializeColorBuffer(POINTER<VulkanCoreObject> core)
			{
				ADGRVulkanColorBufferContainer _container;
				VkFormat colorFormat = core->swapChainImageFormat;

				ADGRCreateImageInfo cinfo;
				cinfo.width = core->swapChainExtent.width;
				cinfo.height = core->swapChainExtent.height;
				cinfo.format = colorFormat;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &_container.image;
				cinfo.imageMemory = &_container.imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = core->msaaSamples;
				cinfo.flags = NULL;

				VulkanFunctions::createImage(core, cinfo);

				ADGRCreateImageViewInfo viewInfo;
				viewInfo.image = _container.image;
				viewInfo.format = colorFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.mipLevels = 1;

				_container.imageView = VulkanFunctions::createImageView(core, viewInfo);

				ADGRTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = _container.image;
				transitionInfo.format = colorFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				transitionInfo.layerCount = 1;

				VulkanFunctions::transitionImageLayout(core, transitionInfo);

				return _container;
			}
			
			void VulkanBufferFunctions::terminateColorBuffer(POINTER<VulkanCoreObject> core, ADGRVulkanColorBufferContainer container)
			{
				vkDestroyImageView(core->logicalDevice, container.imageView, nullptr);
				vkDestroyImage(core->logicalDevice, container.image, nullptr);
				vkFreeMemory(core->logicalDevice, container.imageMemory, nullptr);
			}
			
			/* DEPTH BUFFER */
			ADGRVulkanDepthBufferContainer VulkanBufferFunctions::initializeDepthBuffer(POINTER<VulkanCoreObject> core)
			{
				ADGRVulkanDepthBufferContainer _container;
				VkFormat depthFormat = VulkanFunctions::findDepthFormat(core->physicalDevice);

				ADGRCreateImageInfo cinfo;
				cinfo.width = core->swapChainExtent.width;
				cinfo.height = core->swapChainExtent.height;
				cinfo.format = depthFormat;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &_container.image;
				cinfo.imageMemory = &_container.imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = core->msaaSamples;
				cinfo.flags = NULL;

				VulkanFunctions::createImage(core, cinfo);

				ADGRCreateImageViewInfo viewInfo;
				viewInfo.image = _container.image;
				viewInfo.format = depthFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
				viewInfo.mipLevels = 1;

				_container.imageView = VulkanFunctions::createImageView(core, viewInfo);

				ADGRTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = _container.image;
				transitionInfo.format = depthFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				transitionInfo.layerCount = 2;

				VulkanFunctions::transitionImageLayout(core, transitionInfo);

				return _container;
			}
			
			void VulkanBufferFunctions::terminateDepthBuffer(POINTER<VulkanCoreObject> core, ADGRVulkanDepthBufferContainer container)
			{
				vkDestroyImageView(core->logicalDevice, container.imageView, nullptr);
				vkDestroyImage(core->logicalDevice, container.image, nullptr);
				vkFreeMemory(core->logicalDevice, container.imageMemory, nullptr);
			}
		}
	}
}