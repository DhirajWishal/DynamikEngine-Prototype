#include "adgrafx.h"
#include "VulkanComputeDescriptor.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanComputeDescriptor::initializeDescriptorSetLayout(VkDevice device, ADGRVulkanDescriptorSetLayoutInitInfo info)
			{
				ARRAY<VkDescriptorSetLayoutBinding> bindings;
				if (!info.overrideBindings)
				{
					VkDescriptorSetLayoutBinding uboLayoutBinding = {};
					uboLayoutBinding.binding = 0; // info.bindIndex;
					uboLayoutBinding.descriptorCount = 1;
					uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
					uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
					uboLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
					bindings.push_back(uboLayoutBinding);

					VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
					samplerLayoutBinding.binding = 1; // info.bindIndex;
					samplerLayoutBinding.descriptorCount = 1;
					samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
					samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
					samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
					bindings.push_back(samplerLayoutBinding);
				}

				for (VkDescriptorSetLayoutBinding _binding : info.additionalBindings)
					bindings.push_back(_binding);

				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = static_cast<UI32>(bindings.size());
				layoutInfo.pBindings = bindings.data();

				if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &container.layout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create compute descriptor set layout!");
			}

			void VulkanComputeDescriptor::initializeDescriptorPool(VkDevice device, ADGRVulkanDescriptorPoolInitInfo info)
			{
				ARRAY<VkDescriptorPoolSize> poolSizes = {};

				VkDescriptorPoolSize _poolSize1;
				_poolSize1.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				_poolSize1.descriptorCount = 1;
				poolSizes.push_back(_poolSize1);

				for (VkDescriptorPoolSize _size : info.additionalSizes)
					poolSizes.push_back(_size);

				VkDescriptorPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				poolInfo.pNext = VK_NULL_HANDLE;
				poolInfo.flags = VK_NULL_HANDLE;
				poolInfo.poolSizeCount = poolSizes.size();
				poolInfo.pPoolSizes = poolSizes.data();
				poolInfo.maxSets = 1;

				if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &container.pool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create compute descriptor pool!");
			}

			void VulkanComputeDescriptor::initializeDescriptorSet(VkDevice device, ADGRVulkanDescriptorSetsInitInfo info)
			{
				VkDescriptorSetAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = container.pool;
				allocInfo.descriptorSetCount = 1;
				allocInfo.pSetLayouts = &container.layout;

				if (vkAllocateDescriptorSets(device, &allocInfo, &container.descriptorSet) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate descriptor sets!");

				ARRAY<VkWriteDescriptorSet> descriptorWrites = {};
				//ARRAY<VkDescriptorBufferInfo> bufferInfos;
				//
				//VkDescriptorBufferInfo bufferInfo = {};
				//bufferInfo.buffer = VK_NULL_HANDLE;
				//bufferInfo.offset = 0;
				//bufferInfo.range = VK_WHOLE_SIZE;
				//bufferInfos.pushBack(bufferInfo);
				//
				//bufferInfo.buffer = VK_NULL_HANDLE;
				//bufferInfo.offset = 0;
				//bufferInfo.range = VK_WHOLE_SIZE;
				//bufferInfos.pushBack(bufferInfo);
				//
				//VkWriteDescriptorSet _writes1;
				//_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				//_writes1.dstSet = container.descriptorSet;
				//_writes1.dstBinding = 0;
				//_writes1.dstArrayElement = 0;
				//_writes1.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				//_writes1.descriptorCount = 1;
				//_writes1.pBufferInfo = bufferInfos.data();
				//_writes1.pNext = VK_NULL_HANDLE;
				//_writes1.pImageInfo = VK_NULL_HANDLE;
				//_writes1.pTexelBufferView = VK_NULL_HANDLE;
				//descriptorWrites.push_back(_writes1);

				for (VkWriteDescriptorSet _write : info.additionalWrites)
					descriptorWrites.push_back(_write);

				vkUpdateDescriptorSets(device, static_cast<UI32>(descriptorWrites.size()),
					descriptorWrites.data(), 0, nullptr);
			}
		}
	}
}