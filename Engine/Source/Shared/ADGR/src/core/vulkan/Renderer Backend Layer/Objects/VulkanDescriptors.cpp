#include "adgrafx.h"
#include "VulkanDescriptors.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanDescriptors::initializeLayout(ADGRVulkanDescriptorSetLayoutInitInfo info)
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
				bindings.pushBack(uboLayoutBinding);
				bindings.pushBack(samplerLayoutBinding);

				for (VkDescriptorSetLayoutBinding _binding : info.additionalBindings)
					bindings.pushBack(_binding);

				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = static_cast<UI32>(bindings.size());
				layoutInfo.pBindings = bindings.data();

				if (vkCreateDescriptorSetLayout(info.device.logicalDevice, &layoutInfo, nullptr, &layout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor set layout!");
			}

			void VulkanDescriptors::terminateLayout(VulkanDevice device)
			{
				vkDestroyDescriptorSetLayout(device.logicalDevice, layout, nullptr);
			}

			void VulkanDescriptors::initializeDescriptorPool(ADGRVulkanDescriptorPoolInitInfo info)
			{
				for (UI32 itr = 0; itr < info.poolCount; itr++) {
					ARRAY<VkDescriptorPoolSize> poolSizes = {};
					VkDescriptorPoolSize _poolSize1;
					_poolSize1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					_poolSize1.descriptorCount = static_cast<UI32>(info.swapChain.mySwapChainImages.size());

					VkDescriptorPoolSize _poolSize2;
					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = static_cast<UI32>(info.swapChain.mySwapChainImages.size());

					poolSizes.pushBack(_poolSize1);
					poolSizes.pushBack(_poolSize2);

					for (VkDescriptorPoolSize _size : info.additionalSizes)
						poolSizes.pushBack(_size);

					VkDescriptorPoolCreateInfo poolInfo = {};
					poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
					poolInfo.poolSizeCount = static_cast<UI32>(poolSizes.size());
					poolInfo.pPoolSizes = poolSizes.data();
					poolInfo.maxSets = static_cast<UI32>(info.swapChain.mySwapChainImages.size());

					VkDescriptorPool _localDescriptorPool = VK_NULL_HANDLE;

					if (vkCreateDescriptorPool(info.device.logicalDevice, &poolInfo, nullptr, &_localDescriptorPool) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create descriptor pool!");

					descriptorPools.push_back(_localDescriptorPool);
				}
			}

			void VulkanDescriptors::terminateDescriptorPool(VulkanDevice device)
			{
				for (VkDescriptorPool descriptorPool : descriptorPools)
					vkDestroyDescriptorPool(device.logicalDevice, descriptorPool, nullptr);
			}

			void VulkanDescriptors::initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info)
			{
				std::vector<VkDescriptorSetLayout> layouts(info.swapChain.mySwapChainImages.size(), layout);
				descriptorSets.resize(info.textures.size());

				for (UI32 itr = 0; itr < info.textures.size(); itr++) {
					VkDescriptorSetAllocateInfo allocInfo = {};
					allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
					allocInfo.descriptorPool = descriptorPools[itr];
					allocInfo.descriptorSetCount = static_cast<UI32>(info.swapChain.mySwapChainImages.size());
					allocInfo.pSetLayouts = layouts.data();

					descriptorSets.resize(info.swapChain.mySwapChainImages.size());
					if (vkAllocateDescriptorSets(info.device.logicalDevice, &allocInfo, descriptorSets[itr].data()) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to allocate descriptor sets!");

					for (size_t i = 0; i < (info.swapChain.mySwapChainImages.size()); i++) {
						VkDescriptorBufferInfo bufferInfo = {};
						bufferInfo.buffer = info.uniformBuffers.buffers[i];
						bufferInfo.offset = 0;
						bufferInfo.range = sizeof(UniformBufferObject);

						VkDescriptorImageInfo imageInfo = {};
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						imageInfo.imageView = info.textures[itr].imageView;
						imageInfo.sampler = info.textures[itr].imageSampler;

						ARRAY<VkWriteDescriptorSet> descriptorWrites = {};

						VkWriteDescriptorSet _writes1;
						_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes1.dstSet = descriptorSets[itr][i];
						_writes1.dstBinding = 0;
						_writes1.dstArrayElement = 0;
						_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
						_writes1.descriptorCount = 1;
						_writes1.pBufferInfo = &bufferInfo;

						VkWriteDescriptorSet _writes2;
						_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes2.dstSet = descriptorSets[itr][i];
						_writes2.dstBinding = 1;
						_writes2.dstArrayElement = 0;
						_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						_writes2.descriptorCount = 1;
						_writes2.pImageInfo = &imageInfo;

						descriptorWrites.pushBack(_writes1);
						descriptorWrites.pushBack(_writes2);

						for (VkWriteDescriptorSet _write : info.additionalWrites)
							descriptorWrites.pushBack(_write);

						vkUpdateDescriptorSets(info.device.logicalDevice, static_cast<UI32>(descriptorWrites.size()),
							descriptorWrites.data(), 0, nullptr);
					} // make two descriptor layouts for each descriptor set
				}
			}

			void VulkanDescriptors::terminateDescriptorSets(VulkanDevice device)
			{
			}
		}
	}
}