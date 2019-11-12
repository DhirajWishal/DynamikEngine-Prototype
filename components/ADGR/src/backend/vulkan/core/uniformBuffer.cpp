#include "adgrafx.h"
#include "uniformBuffer.h"

#include "queues/queues.h"
#include "functions/bufferFunctions.h"

#include "data structures/Uniform.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void uniformBufferManager::createUniformBuffers(ADGRVulkanDataContainer* container, DMKUniformBufferCreateInfo info) {
				VkDeviceSize bufferSize = sizeof(UniformBufferObject);

				info.buffers->resize(container->swapchainContainer.swapChainImages.size());
				info.bufferMemories->resize(container->swapchainContainer.swapChainImages.size());

				for (size_t i = 0; i < container->swapchainContainer.swapChainImages.size(); i++) {
					DMKCreateBufferInfo bufferInfo;
					bufferInfo.device = container->device;
					bufferInfo.physicalDevice = container->physicalDevice;
					bufferInfo.bufferSize = bufferSize;
					bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &info.buffers->at(i);
					bufferInfo.bufferMemory = &info.bufferMemories->at(i);

					functions::createBuffer(bufferInfo);
				}
			}

			void uniformBufferManager::updateBuffer2D(ADGRVulkanDataContainer* container, DMKUniformBufferUpdateInfo info) {
				// TODO: update
				static auto startTime = std::chrono::high_resolution_clock::now();

				auto currentTime = std::chrono::high_resolution_clock::now();
				float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

				if (info.turn[0])
					trn += movementBias;

				else if (info.turn[1])
					trn -= movementBias;

				else;
				//trn = 0.0f;


				if (info.move[0])
					mve += movementBias;

				else if (info.move[1])
					mve -= movementBias;

				else;

				if (info.upDown[0])
					up += upDownBias;

				else if (info.upDown[1])
					up -= upDownBias;

				else;

				if (info.rotation[1])
					rotation += rotationBias;

				else if (info.rotation[0])
					rotation -= rotationBias;

				else;

				UniformBufferObject ubo = {};
				//ubo.model = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(mve, trn, up)),
				//	/*time */ glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

				//ubo.model = glm::ortho(mve, trn, rotation, up, 1.0f,10.0f);
				//ubo.model = glm::frustum(mve, trn, rotation, up, 0.1f, 10.0f);

				ubo.model = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(mve, trn, up)),
					glm::radians(info.cPos[0]), glm::vec3(0.0f, 0.0f, 1.0f))
					* glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
						glm::radians(info.cPos[1]), glm::vec3(0.0f, 1.0f, 0.0f))
					* glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
						glm::radians(rotation), glm::vec3(1.0f, 0.0f, 0.0f));

				ubo.view = glm::lookAt(glm::vec3(0.5f, 3.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 0.0f, 1.0f));
				//ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.001f, 10.0f);
				ubo.proj = glm::ortho(-10.0f, 800.0f, 600.0f, 10.0f, 0.0f, 100.0f);
				ubo.proj[1][1] *= -1;

				void* data;
				vkMapMemory(container->device, info.bufferMemory[info.currentImage], 0, sizeof(ubo), 0, &data);
				memcpy(data, &ubo, sizeof(ubo));
				vkUnmapMemory(container->device, info.bufferMemory[info.currentImage]);
			}

			void uniformBufferManager::updateBuffer3D(ADGRVulkanDataContainer* container, DMKUniformBufferUpdateInfo info) {
				// TODO: update
				static auto startTime = std::chrono::high_resolution_clock::now();

				auto currentTime = std::chrono::high_resolution_clock::now();
				float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

				if (info.turn[0])
					trn += movementBias;

				else if (info.turn[1])
					trn -= movementBias;

				else;
				//trn = 0.0f;


				if (info.move[0])
					mve += movementBias;

				else if (info.move[1])
					mve -= movementBias;

				else;

				if (info.upDown[0])
					up += upDownBias;

				else if (info.upDown[1])
					up -= upDownBias;

				else;

				if (info.rotation[0])
					rotation += rotationBias;

				else if (info.rotation[1])
					rotation -= rotationBias;

				else;	

				UniformBufferObject ubo = {};
				ubo.model = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(mve, trn, up)),
						glm::radians(info.cPos[0]), glm::vec3(0.0f, 0.0f, 1.0f))
					* glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
						glm::radians(info.cPos[1]), glm::vec3(0.0f, 1.0f, 0.0f))
					* glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
						glm::radians(rotation), glm::vec3(1.0f, 0.0f, 0.0f));

				//ubo.model = glm::mat4(glm::mat4(1.0f),
				//	glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.005f, 0.0f)), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)),
				//	glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.005f, 0.0f)), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)),
				//	glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.005f, 0.0f)), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)));

				//+ glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.005f, 0.0f)),
				//	/*time */ glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));

				//ubo.model = glm::ortho(mve, trn, rotation, up, 1.0f,10.0f);
			    //ubo.model = glm::frustum(1.0f, 1.0f, 5.0f, 5.0f, 0.1f, 10.0f);
				// left, right, bottom, top

				ubo.view = glm::lookAt(glm::vec3(0.5f, 3.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 0.0f, 1.0f));
				ubo.proj = glm::perspective(glm::radians(45.0f), (float)container->swapchainContainer.swapchainExtent.width / (float)container->swapchainContainer.swapchainExtent.height, 0.001f, 10.0f);
				ubo.proj[1][1] *= -1;

				void* data;
				vkMapMemory(container->device, info.bufferMemory[info.currentImage], 0, sizeof(ubo), 0, &data);
				memcpy(data, &ubo, sizeof(ubo));
				vkUnmapMemory(container->device, info.bufferMemory[info.currentImage]);

				/*
				 vector<ubo> bufferData;
				 bufferData[0] = pos;
				 bufferData[1] = col;
				 bufferData[2] = tex;

				 vkCmdPushConstants(range, bufferData);

				 in Vertex Shader:
				 layout(push_constants) uniform PushBlock {
					_dataType_ pos;
					_dataType_ col;
					_dataType_ tex;
				 } pushBlockData;
				*/
			}

			void uniformBufferManager::createDescriptorSetLayout(ADGRVulkanDataContainer* container, DMKUniformBufferCreateDescriptorSetLayoutInfo info) {
				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = info.bindIndex[0]; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

				//VkDescriptorSetLayoutBinding dynamicUboLayoutBinding = {};
				//dynamicUboLayoutBinding.binding = info.bindIndex[1]; // info.bindIndex;
				//dynamicUboLayoutBinding.descriptorCount = 1;
				//dynamicUboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				//dynamicUboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				//dynamicUboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

				VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
				samplerLayoutBinding.binding = info.bindIndex[1]; // info.bindIndex;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

				std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
				layoutInfo.pBindings = bindings.data();

				if (vkCreateDescriptorSetLayout(container->device, &layoutInfo, nullptr, info.layout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor set layout!");
			}

			void uniformBufferManager::initDescriptorPool(ADGRVulkanDataContainer* container, VkDescriptorPool* descriptorPool) {
				std::array<VkDescriptorPoolSize, 2> poolSizes = {};
				poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				poolSizes[0].descriptorCount = static_cast<uint32>(container->swapchainContainer.swapChainImages.size());

				//poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				//poolSizes[1].descriptorCount = static_cast<uint32>(swapChainImages.size());

				poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				poolSizes[1].descriptorCount = static_cast<uint32>(container->swapchainContainer.swapChainImages.size());

				VkDescriptorPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				poolInfo.poolSizeCount = static_cast<uint32>(poolSizes.size());
				poolInfo.pPoolSizes = poolSizes.data();
				poolInfo.maxSets = static_cast<uint32>(container->swapchainContainer.swapChainImages.size());

				if (vkCreateDescriptorPool(container->device, &poolInfo, nullptr, descriptorPool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor pool!");
			}

			void uniformBufferManager::initDescriptorSets(ADGRVulkanDataContainer* container, DMKDescriptorSetsInitInfo info) {
				std::vector<VkDescriptorSetLayout> layouts(container->swapchainContainer.swapChainImages.size(), *info.layout);
				VkDescriptorSetAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = info.descriptorPool;
				allocInfo.descriptorSetCount = static_cast<uint32>(container->swapchainContainer.swapChainImages.size());
				allocInfo.pSetLayouts = layouts.data();

				info.descriptorSets->resize(container->swapchainContainer.swapChainImages.size());
				if (vkAllocateDescriptorSets(container->device, &allocInfo, info.descriptorSets->data()) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate descriptor sets!");

				for (size_t i = 0; i < (container->swapchainContainer.swapChainImages.size()); i++) {
					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = info.uniformBuffers->at(i);
					bufferInfo.offset = 0;
					bufferInfo.range = sizeof(UniformBufferObject);

					VkDescriptorImageInfo imageInfo = {};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = info.textureImageView;
					imageInfo.sampler = info.textureSampler;

					std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

					descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites[0].dstSet = info.descriptorSets->at(i);
					descriptorWrites[0].dstBinding = info.bindIndex[0];
					descriptorWrites[0].dstArrayElement = 0;
					descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					descriptorWrites[0].descriptorCount = 1;
					descriptorWrites[0].pBufferInfo = &bufferInfo;

					//descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					//descriptorWrites[1].dstSet = info.descriptorSets->at(i);
					//descriptorWrites[1].dstBinding = info.bindIndex[1];
					//descriptorWrites[1].dstArrayElement = 0;
					//descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
					//descriptorWrites[1].descriptorCount = 1;
					//descriptorWrites[1].pBufferInfo = &bufferInfo;

					descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites[1].dstSet = info.descriptorSets->at(i);
					descriptorWrites[1].dstBinding = info.bindIndex[1];
					descriptorWrites[1].dstArrayElement = 0;
					descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					descriptorWrites[1].descriptorCount = 1;
					descriptorWrites[1].pImageInfo = &imageInfo;

					vkUpdateDescriptorSets(container->device, static_cast<uint32_t>(descriptorWrites.size()),
						descriptorWrites.data(), 0, nullptr);
				} // make two descriptor layouts for each descriptor set
			}

			void uniformBufferManager::deleteBuffer(ADGRVulkanDataContainer* container, VkBuffer* buffer) {
				vkDestroyBuffer(container->device, *buffer, nullptr);
			}
		}
	}
}
