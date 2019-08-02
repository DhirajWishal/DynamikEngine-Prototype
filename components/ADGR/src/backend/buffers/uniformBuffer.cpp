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

				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = 1;
				layoutInfo.pBindings = &uboLayoutBinding;

				if (vkCreateDescriptorSetLayout(*myDevice, &layoutInfo, nullptr,
					myDescriptorSetLayout) != VK_SUCCESS)
					throw std::runtime_error("failed to create descriptor set layout!");
			}

			void uniformBuffer::initDescripterPool(std::vector<VkImage> swapChainImages) {
				VkDescriptorPoolSize poolSize = {};
				poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				poolSize.descriptorCount = static_cast<uint32_t>(swapChainImages.size());

				VkDescriptorPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				poolInfo.poolSizeCount = 1;
				poolInfo.pPoolSizes = &poolSize;
				poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

				if (vkCreateDescriptorPool(*myDevice, &poolInfo, nullptr, myDescriptorPool) != VK_SUCCESS)
					throw std::runtime_error("failed to create descriptor pool!");
			}

			void uniformBuffer::deleteDescripter() {
				vkDestroyDescriptorSetLayout(*myDevice, *myDescriptorSetLayout, nullptr);
			}

			void uniformBuffer::initBuffer(std::vector<VkImage> swapChainImages) {
				VkDeviceSize bufferSize = sizeof(UniformBufferObject);

				myUniformBuffer->resize(swapChainImages.size());
				myUniformBufferMemory->resize(swapChainImages.size());

				for (size_t i = 0; i < swapChainImages.size(); i++) {
					createBuffer(*myDevice, *myPhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, myUniformBuffer->at(i), myUniformBufferMemory->at(i));
				}
			}

			void uniformBuffer::updateBuffer(uint32 index, VkExtent2D swapChainExtent,
				uint32 currentImage) {
				static auto startTime = std::chrono::high_resolution_clock::now();

				auto currentTime = std::chrono::high_resolution_clock::now();
				float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

				UniformBufferObject ubo = {};
				ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
				ubo.proj[1][1] *= -1;

				void* data;
				vkMapMemory(*myDevice, myUniformBufferMemory->at(currentImage), 0, sizeof(ubo), 0, &data);
				memcpy(data, &ubo, sizeof(ubo));
				vkUnmapMemory(*myDevice, myUniformBufferMemory->at(currentImage));
			}

			void uniformBuffer::initDescripterSets(std::vector<VkImage> swapChainImages) {
				std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), *myDescriptorSetLayout);
				VkDescriptorSetAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = *myDescriptorPool;
				allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
				allocInfo.pSetLayouts = layouts.data();

				myDescriptorSets->resize(swapChainImages.size());
				if (vkAllocateDescriptorSets(*myDevice, &allocInfo, myDescriptorSets->data()) != VK_SUCCESS)
					throw std::runtime_error("failed to allocate descriptor sets!");

				for (size_t i = 0; i < swapChainImages.size(); i++) {
					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = myUniformBuffer->at(i);
					bufferInfo.offset = 0;
					bufferInfo.range = sizeof(UniformBufferObject);

					VkWriteDescriptorSet descriptorWrite = {};
					descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrite.dstSet = myDescriptorSets->at(i);
					descriptorWrite.dstBinding = 0;
					descriptorWrite.dstArrayElement = 0;
					descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					descriptorWrite.descriptorCount = 1;
					descriptorWrite.pBufferInfo = &bufferInfo;
					descriptorWrite.pImageInfo = nullptr; // Optional
					descriptorWrite.pTexelBufferView = nullptr; // Optional

					vkUpdateDescriptorSets(*myDevice, 1, &descriptorWrite, 0, nullptr);
				}
			}
		}
	}
}
