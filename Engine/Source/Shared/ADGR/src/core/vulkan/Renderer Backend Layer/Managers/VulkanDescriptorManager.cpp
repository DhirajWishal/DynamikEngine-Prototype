#include "adgrafx.h"
#include "VulkanDescriptorManager.h"

#include "VulkanTextureManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanDescriptorManager::initializeLayout(
				POINTER<VulkanCoreObject> core,
				ADGRVulkanDescriptorSetLayoutInitInfo info,
				POINTER<VulkanRenderObject> object)
			{
				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

				VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
				samplerLayoutBinding.binding = 1; // info.bindIndex;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

				ARRAY<VkDescriptorSetLayoutBinding> bindings;
				bindings.push_back(uboLayoutBinding);
				bindings.push_back(samplerLayoutBinding);

				for (VkDescriptorSetLayoutBinding _binding : info.additionalBindings)
					bindings.push_back(_binding);

				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = static_cast<UI32>(bindings.size());
				layoutInfo.pBindings = bindings.data();

				if (vkCreateDescriptorSetLayout(core->logicalDevice, &layoutInfo, nullptr, &object->descriptorLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor set layout!");
			}

			void VulkanDescriptorManager::terminateLayout(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object)
			{
				vkDestroyDescriptorSetLayout(core->logicalDevice, object->descriptorLayout, nullptr);
			}

			void VulkanDescriptorManager::initializeDescriptorPool(
				POINTER<VulkanCoreObject> core,
				ADGRVulkanDescriptorPoolInitInfo info,
				POINTER<VulkanRenderObject> object)
			{
				for (UI32 itr = 0; itr < info.poolCount; itr++) {
					ARRAY<VkDescriptorPoolSize> poolSizes = {};
					VkDescriptorPoolSize _poolSize1;
					_poolSize1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					_poolSize1.descriptorCount = static_cast<UI32>(core->swapChainImages.size());

					VkDescriptorPoolSize _poolSize2;
					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = static_cast<UI32>(core->swapChainImages.size());

					poolSizes.push_back(_poolSize1);
					poolSizes.push_back(_poolSize2);

					for (VkDescriptorPoolSize _size : info.additionalSizes)
						poolSizes.push_back(_size);

					VkDescriptorPoolCreateInfo poolInfo = {};
					poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
					poolInfo.poolSizeCount = static_cast<UI32>(poolSizes.size());
					poolInfo.pPoolSizes = poolSizes.data();
					poolInfo.maxSets = static_cast<UI32>(core->swapChainImages.size());

					VkDescriptorPool _localDescriptorPool = VK_NULL_HANDLE;

					if (vkCreateDescriptorPool(core->logicalDevice, &poolInfo, nullptr, &_localDescriptorPool) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create descriptor pool!");

					object->descriptorPools.push_back(_localDescriptorPool);
				}
			}

			void VulkanDescriptorManager::terminateDescriptorPool(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object)
			{
				for (auto descriptorPool : object->descriptorPools)
					vkDestroyDescriptorPool(core->logicalDevice, descriptorPool, nullptr);
			}

			void VulkanDescriptorManager::initializeDescriptorSets(
				POINTER<VulkanCoreObject> core,
				ADGRVulkanDescriptorSetsInitInfo info,
				POINTER<VulkanRenderObject> object)
			{
				ARRAY<VkDescriptorSetLayout> layouts(core->swapChainImages.size(), object->descriptorLayout);
				object->descriptorSets.resize(object->textures.size());

				for (UI32 itr = 0; itr < object->textures.size(); itr++) {
					VkDescriptorSetAllocateInfo allocInfo = {};
					allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
					allocInfo.descriptorPool = object->descriptorPools[itr];
					allocInfo.descriptorSetCount = static_cast<UI32>(core->swapChainImages.size());
					allocInfo.pSetLayouts = layouts.data();

					object->descriptorSets.resize(core->swapChainImages.size());
					if (vkAllocateDescriptorSets(core->logicalDevice, &allocInfo, object->descriptorSets[itr].data()) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to allocate descriptor sets!");

					for (size_t i = 0; i < (core->swapChainImages.size()); i++) {
						VkDescriptorBufferInfo bufferInfo = {};
						bufferInfo.buffer = object->uniformBuffers[i];
						bufferInfo.offset = 0;
						bufferInfo.range = sizeof(UniformBufferObject);

						VkDescriptorImageInfo imageInfo = {};
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						imageInfo.imageView = object->textures[itr].imageView;
						imageInfo.sampler = object->textures[itr].imageSampler;

						ARRAY<VkWriteDescriptorSet> descriptorWrites = {};

						VkWriteDescriptorSet _writes1;
						_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes1.dstSet = object->descriptorSets[itr][i];
						_writes1.dstBinding = 0;
						_writes1.dstArrayElement = 0;
						_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
						_writes1.descriptorCount = 1;
						_writes1.pBufferInfo = &bufferInfo;

						VkWriteDescriptorSet _writes2;
						_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes2.dstSet = object->descriptorSets[itr][i];
						_writes2.dstBinding = 1;
						_writes2.dstArrayElement = 0;
						_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						_writes2.descriptorCount = 1;
						_writes2.pImageInfo = &imageInfo;

						descriptorWrites.push_back(_writes1);
						descriptorWrites.push_back(_writes2);

						for (VkWriteDescriptorSet _write : info.additionalWrites)
							descriptorWrites.push_back(_write);

						vkUpdateDescriptorSets(core->logicalDevice, static_cast<UI32>(descriptorWrites.size()),
							descriptorWrites.data(), 0, nullptr);
					} // make two descriptor layouts for each descriptor set
				}
			}

			void VulkanDescriptorManager::terminateDescriptorSets(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object)
			{
			}
		}
	}
}