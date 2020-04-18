#include "adgrafx.h"
#include "VulkanDescriptor.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanDescriptor::initializeLayout(VkDevice logicalDevice, ADGRVulkanDescriptorSetLayoutInitInfo info)
			{
				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = info.bindings.size();
				layoutInfo.pBindings = info.bindings.data();

				if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &layout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor set layout!");
			}

			void VulkanDescriptor::initializePool(VkDevice logicalDevice, ADGRVulkanDescriptorPoolInitInfo info)
			{
				VkDescriptorPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				poolInfo.poolSizeCount = info.poolSizes.size();
				poolInfo.pPoolSizes = info.poolSizes.data();
				poolInfo.maxSets = info.maxDescriptorSets;

				if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &pool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor pool!");
			}

			void VulkanDescriptor::initializeSets(VkDevice logicalDevice, ADGRVulkanDescriptorSetsInitInfo info)
			{
				VkDescriptorSetAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = pool;
				allocInfo.descriptorSetCount = 1;
				allocInfo.pSetLayouts = &layout;

				if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, &descriptorSet) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate descriptor sets!");

				for (UI32 _index = 0; _index < info.descriptorWrites.size(); _index++)
					info.descriptorWrites[_index].dstSet = descriptorSet;

				vkUpdateDescriptorSets(logicalDevice, info.descriptorWrites.size(), info.descriptorWrites.data(), 0, nullptr);
			}

			void VulkanDescriptor::terminate(VkDevice logicalDevice)
			{
			}
		}
	}
}