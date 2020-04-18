#include "adgrafx.h"
#include "VulkanComputeDescriptor.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanComputeDescriptor::initializeDescriptorSetLayout(VkDevice device, ADGRVulkanDescriptorSetLayoutInitInfo info)
			{
				ARRAY<VkDescriptorSetLayoutBinding> bindings;
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

				VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
				if (vkAllocateDescriptorSets(device, &allocInfo, &_descriptorSet) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate descriptor sets!");

				ARRAY<VkWriteDescriptorSet> descriptorWrites = {};

				for (VkWriteDescriptorSet _write : info.additionalWrites)
				{
					_write.dstSet = _descriptorSet;
					descriptorWrites.push_back(_write);
				}

				vkUpdateDescriptorSets(device, static_cast<UI32>(descriptorWrites.size()),
					descriptorWrites.data(), 0, nullptr);

				container.descriptorSets.pushBack(_descriptorSet);
			}
		}
	}
}