/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		01/08/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "uniformBuffer.h"
#include "buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			uniformBuffer::uniformBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
				std::vector<VkBuffer>* uniformBuffer, std::vector<VkDeviceMemory>* uniformBuffersMemory, VkDescriptorPool* descriptorPool,
				std::vector<VkDescriptorSet>* descriptorSets, VkDescriptorSetLayout* descriptorSetLayout) :
				myDevice(device), myPhysicalDevice(physicalDevice), myUniformBuffer(uniformBuffer),
				myUniformBufferMemory(uniformBuffersMemory), myDescriptorPool(descriptorPool),
				myDescriptorSets(descriptorSets), myDescriptorSetLayout(descriptorSetLayout) {

			}

			uniformBuffer::~uniformBuffer() {

			}

			void uniformBuffer::createDescripterSetLayout() {
				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

				VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
				samplerLayoutBinding.binding = 1;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr;
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

				std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = static_cast<uint32>(bindings.size());
				layoutInfo.pBindings = bindings.data();

				if (vkCreateDescriptorSetLayout(*myDevice, &layoutInfo, nullptr,
					myDescriptorSetLayout) != VK_SUCCESS)
					std::runtime_error("failed to create descriptor set layout!");
			}

			void uniformBuffer::initDescripterPool(std::vector<VkImage> swapChainImages) {
				std::array<VkDescriptorPoolSize, 2> poolSizes = {};
				poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				poolSizes[0].descriptorCount = static_cast<uint32>(swapChainImages.size());
				poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				poolSizes[1].descriptorCount = static_cast<uint32>(swapChainImages.size());

				VkDescriptorPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				poolInfo.poolSizeCount = static_cast<uint32>(poolSizes.size());
				poolInfo.pPoolSizes = poolSizes.data();
				poolInfo.maxSets = static_cast<uint32>(swapChainImages.size());

				if (vkCreateDescriptorPool(*myDevice, &poolInfo, nullptr, myDescriptorPool) != VK_SUCCESS)
					std::runtime_error("failed to create descriptor pool!");
			}

			void uniformBuffer::deleteDescripter() {
				vkDestroyDescriptorSetLayout(*myDevice, *myDescriptorSetLayout, nullptr);
			}

			void uniformBuffer::initBuffer(std::vector<VkImage> swapChainImages) {
				VkDeviceSize bufferSize = sizeof(UniformBufferObject);

				myUniformBuffer->resize(swapChainImages.size());
				myUniformBufferMemory->resize(swapChainImages.size());

				for (size_t i = 0; i < swapChainImages.size(); i++) {
					createBuffer(*myDevice, *myPhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT 
						| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
						myUniformBuffer->at(i), myUniformBufferMemory->at(i));
				}
			}

			void uniformBuffer::updateBuffer(uint32 currentImage, VkExtent2D swapChainExtent,
				bool turn[2], bool mov[2], float pos[2]) {
				static auto startTime = std::chrono::high_resolution_clock::now();

				auto currentTime = std::chrono::high_resolution_clock::now();
				float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

				if (turn[0])
					trn += movementBias;

				else if (turn[1])
					trn -= movementBias;

				else;
				//trn = 0.0f;


				if (mov[0])
					move += movementBias;

				else if (mov[1])
					move -= movementBias;

				else;


				position[0] = pos[0] - (swapChainExtent.width / 2);
				position[1] = pos[1] - (swapChainExtent.height / 2);

				//printf("%f, %f\n", position[0], position[1]);

				UniformBufferObject ubo = {};
				//ubo.model = glm::rotate(glm::mat4(1.0f), /*time **/ glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				//ubo.model = glm::rotate(glm::translate(glm::mat4(myPos), glm::vec3(move, trn, 0.0f)), glm::radians(260.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				ubo.model = glm::rotate(glm::translate(glm::mat4(myPos), glm::vec3(move, trn, 0.0f)), time * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				ubo.view = glm::lookAt(glm::vec3(0.5f, 3.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
				ubo.proj[1][1] *= -1;

				void* data;
				vkMapMemory(*myDevice, myUniformBufferMemory->at(currentImage), 0, sizeof(ubo), 0, &data);
				memcpy(data, &ubo, sizeof(ubo));
				vkUnmapMemory(*myDevice, myUniformBufferMemory->at(currentImage));

				if (myPos == 1.0f)
					myPos = 2.0f;
				else if (myPos == 2.0f)
					myPos = 1.0f;
			}

			void uniformBuffer::initDescripterSets(std::vector<VkImage> swapChainImages,
				VkImageView textureImageView, VkSampler textureSampler) {
				std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), *myDescriptorSetLayout);
				VkDescriptorSetAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = *myDescriptorPool;
				allocInfo.descriptorSetCount = static_cast<uint32>(swapChainImages.size());
				allocInfo.pSetLayouts = layouts.data();

				myDescriptorSets->resize(swapChainImages.size());
				if (vkAllocateDescriptorSets(*myDevice, &allocInfo, myDescriptorSets->data()) != VK_SUCCESS)
					std::runtime_error("failed to allocate descriptor sets!");

				for (size_t i = 0; i < swapChainImages.size(); i++) {
					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = myUniformBuffer->at(i);
					bufferInfo.offset = 0;
					bufferInfo.range = sizeof(UniformBufferObject);

					VkDescriptorImageInfo imageInfo = {};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = textureImageView;
					imageInfo.sampler = textureSampler;

					std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

					descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites[0].dstSet = myDescriptorSets->at(i);
					descriptorWrites[0].dstBinding = 0;
					descriptorWrites[0].dstArrayElement = 0;
					descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					descriptorWrites[0].descriptorCount = 1;
					descriptorWrites[0].pBufferInfo = &bufferInfo;

					descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites[1].dstSet = myDescriptorSets->at(i);
					descriptorWrites[1].dstBinding = 1;
					descriptorWrites[1].dstArrayElement = 0;
					descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					descriptorWrites[1].descriptorCount = 1;
					descriptorWrites[1].pImageInfo = &imageInfo;

					vkUpdateDescriptorSets(*myDevice, static_cast<uint32_t>(descriptorWrites.size()),
						descriptorWrites.data(), 0, nullptr);
				}
			}
		}
	}
}
