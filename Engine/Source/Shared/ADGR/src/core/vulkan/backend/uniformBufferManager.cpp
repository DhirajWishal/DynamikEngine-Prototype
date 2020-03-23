#include "adgrafx.h"
#include "uniformBufferManager.h"

#include "queues/queues.h"
#include "functions/bufferFunctions.h"

#include "data structures/Uniform.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void uniformBufferManager::createUniformBuffers(ADGRVulkanDataContainer* container, vulkanFormat* format) {
				VkDeviceSize bufferSize = sizeof(UniformBufferObject);

				format->myUniformBuffers.resize(container->swapchainContainer.swapChainImages.size());
				format->myUniformBufferMemories.resize(container->swapchainContainer.swapChainImages.size());

				for (size_t i = 0; i < container->swapchainContainer.swapChainImages.size(); i++) {
					DMKCreateBufferInfo bufferInfo;
					bufferInfo.device = container->device;
					bufferInfo.physicalDevice = container->physicalDevice;
					bufferInfo.bufferSize = bufferSize;
					bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &format->myUniformBuffers.at(i);
					bufferInfo.bufferMemory = &format->myUniformBufferMemories.at(i);

					functions::createBuffer(bufferInfo);
				}
			}

			void uniformBufferManager::createUniformBuffers(ADGRVulkanDataContainer* container,
				ARRAY<VkBuffer>* uniformBuffers, ARRAY<VkDeviceMemory>* uniformBufferMemories) {
				VkDeviceSize bufferSize = sizeof(UniformBufferObject);

				uniformBuffers->resize(container->swapchainContainer.swapChainImages.size());
				uniformBufferMemories->resize(container->swapchainContainer.swapChainImages.size());

				for (size_t i = 0; i < container->swapchainContainer.swapChainImages.size(); i++) {
					DMKCreateBufferInfo bufferInfo;
					bufferInfo.device = container->device;
					bufferInfo.physicalDevice = container->physicalDevice;
					bufferInfo.bufferSize = bufferSize;
					bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &uniformBuffers->at(i);
					bufferInfo.bufferMemory = &uniformBufferMemories->at(i);

					functions::createBuffer(bufferInfo);
				}
			}

			void uniformBufferManager::updateBuffer3D(ADGRVulkanDataContainer* container, std::deque<DMKEventContainer> eventContainers, vulkanFormat* format, UI32 currentImage) {
				// TODO: update
				DMKUpdateInfo updateInfo = {};
				updateInfo = format->myRendererFormat->myInternalFormat->myGameObject->draw(eventContainers);

				UniformBufferObject ubo = {};
				ubo.model = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(updateInfo.leftRight, updateInfo.frontBack, updateInfo.upDown)),
					glm::radians(updateInfo.rotationX), glm::vec3(0.0f, 0.0f, 1.0f))
					* glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
						glm::radians(updateInfo.rotationY), glm::vec3(0.0f, 1.0f, 0.0f))
					* glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
						glm::radians(updateInfo.rotationZ), glm::vec3(1.0f, 0.0f, 0.0f));
				ubo.view = glm::lookAt(glm::vec3(0.5f, 3.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 0.0f, 1.0f));
				ubo.proj = glm::perspective(glm::radians(45.0f), (F32)container->swapchainContainer.swapchainExtent.width / (F32)container->swapchainContainer.swapchainExtent.height, 0.001f, 10.0f);
				ubo.proj[1][1] *= -1;

				void* data = nullptr;
				vkMapMemory(container->device, format->myUniformBufferMemories[currentImage], 0, sizeof(ubo), 0, &data);
				memcpy(data, &ubo, sizeof(ubo));
				vkUnmapMemory(container->device, format->myUniformBufferMemories[currentImage]);
			}

			void uniformBufferManager::clean(ADGRVulkanDataContainer* container, vulkanFormat* format) {
				// destroy uniformNuffers and uniformBufferMemories
				for (I32 x = 0; x < format->myUniformBuffers.size(); x++) {
					vkDestroyBuffer(container->device, format->myUniformBuffers[x], nullptr);
					vkFreeMemory(container->device, format->myUniformBufferMemories[x], nullptr);
				}

				// destroy descriptor pools
				for (VkDescriptorPool descriptorPool : format->myDescriptorPools)
					vkDestroyDescriptorPool(container->device, descriptorPool, nullptr);

				format->myUniformBuffers.clear();
				format->myUniformBufferMemories.clear();
				format->myDescriptorPools.clear();
			}

			void uniformBufferManager::createDescriptorSetLayout(ADGRVulkanDataContainer* container, vulkanFormat* format) {
				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
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
				samplerLayoutBinding.binding = 1; // info.bindIndex;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

				std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = static_cast<UI32>(bindings.size());
				layoutInfo.pBindings = bindings.data();

				if (vkCreateDescriptorSetLayout(container->device, &layoutInfo, nullptr, &format->myDescriptorSetLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor set layout!");
			}

			void uniformBufferManager::createDescriptorSetLayout(ADGRVulkanDataContainer* container, VkDescriptorSetLayout* descriptorSetLayout) {
				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
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
				samplerLayoutBinding.binding = 1; // info.bindIndex;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

				std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = static_cast<UI32>(bindings.size());
				layoutInfo.pBindings = bindings.data();

				if (vkCreateDescriptorSetLayout(container->device, &layoutInfo, nullptr, descriptorSetLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor set layout!");
			}

			void uniformBufferManager::initDescriptorPool(ADGRVulkanDataContainer* container, vulkanFormat* format) {
				for (UI32 itr = 0; itr < format->myTextureImages.size(); itr++) {
					std::array<VkDescriptorPoolSize, 2> poolSizes = {};
					poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					poolSizes[0].descriptorCount = static_cast<UI32>(container->swapchainContainer.swapChainImages.size());

					//poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					//poolSizes[1].descriptorCount = static_cast<UI32>(swapChainImages.size());

					poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					poolSizes[1].descriptorCount = static_cast<UI32>(container->swapchainContainer.swapChainImages.size());

					VkDescriptorPoolCreateInfo poolInfo = {};
					poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
					poolInfo.poolSizeCount = static_cast<UI32>(poolSizes.size());
					poolInfo.pPoolSizes = poolSizes.data();
					poolInfo.maxSets = static_cast<UI32>(container->swapchainContainer.swapChainImages.size());

					VkDescriptorPool _localDescriptorPool = VK_NULL_HANDLE;

					if (vkCreateDescriptorPool(container->device, &poolInfo, nullptr, &_localDescriptorPool) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create descriptor pool!");

					format->myDescriptorPools.push_back(_localDescriptorPool);
				}
			}

			void uniformBufferManager::initDescriptorSets(ADGRVulkanDataContainer* container, vulkanFormat* format) {
				std::vector<VkDescriptorSetLayout> layouts(container->swapchainContainer.swapChainImages.size(), format->myDescriptorSetLayout);
				format->myDescriptorSets.resize(format->myTextureImages.size());

				for (UI32 itr = 0; itr < format->myTextureImages.size(); itr++) {
					VkDescriptorSetAllocateInfo allocInfo = {};
					allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
					allocInfo.descriptorPool = format->myDescriptorPools[itr];
					allocInfo.descriptorSetCount = static_cast<UI32>(container->swapchainContainer.swapChainImages.size());
					allocInfo.pSetLayouts = layouts.data();

					format->myDescriptorSets[itr].resize(container->swapchainContainer.swapChainImages.size());
					if (vkAllocateDescriptorSets(container->device, &allocInfo, format->myDescriptorSets[itr].data()) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to allocate descriptor sets!");

					for (size_t i = 0; i < (container->swapchainContainer.swapChainImages.size()); i++) {
						VkDescriptorBufferInfo bufferInfo = {};
						bufferInfo.buffer = format->myUniformBuffers[i];
						bufferInfo.offset = 0;
						bufferInfo.range = sizeof(UniformBufferObject);

						VkDescriptorImageInfo imageInfo = {};
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						imageInfo.imageView = format->myTextureImageViews[itr];
						imageInfo.sampler = format->myTextureImageSamplers[itr];

						std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

						descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						descriptorWrites[0].dstSet = format->myDescriptorSets[itr][i];
						descriptorWrites[0].dstBinding = 0;
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
						descriptorWrites[1].dstSet = format->myDescriptorSets[itr][i];
						descriptorWrites[1].dstBinding = 1;
						descriptorWrites[1].dstArrayElement = 0;
						descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						descriptorWrites[1].descriptorCount = 1;
						descriptorWrites[1].pImageInfo = &imageInfo;

						vkUpdateDescriptorSets(container->device, static_cast<UI32>(descriptorWrites.size()),
							descriptorWrites.data(), 0, nullptr);
					} // make two descriptor layouts for each descriptor set
				}
			}
		}
	}
}
#endif // DMK_USE_VULKAN