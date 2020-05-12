#include "dmkafx.h"
#include "VulkanDescriptor.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			void VulkanDescriptor::initializeLayout(VkDevice logicalDevice, VulkanDescriptorSetLayoutInitInfo info)
			{
				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = info.bindings.size();
				layoutInfo.pBindings = info.bindings.data();

				if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &layout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor set layout!");
			}

			void VulkanDescriptor::initializePool(VkDevice logicalDevice, VulkanDescriptorPoolInitInfo info)
			{
				VkDescriptorPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				poolInfo.poolSizeCount = info.poolSizes.size();
				poolInfo.pPoolSizes = info.poolSizes.data();
				poolInfo.maxSets = info.maxDescriptorSets;

				if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &pool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor pool!");
			}

			void VulkanDescriptor::allocateSets(VkDevice logicalDevice, UI32 setCount)
			{
				for (UI32 _i = 0; _i < setCount; _i++)
				{
					VkDescriptorSetAllocateInfo allocInfo = {};
					allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
					allocInfo.descriptorPool = pool;
					allocInfo.descriptorSetCount = 1;
					allocInfo.pSetLayouts = &layout;

					VkDescriptorSet _set = VK_NULL_HANDLE;
					if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, &_set) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to allocate descriptor sets!");

					descriptorSets.push_back(_set);
				}
			}

			void VulkanDescriptor::initializeSets(VkDevice logicalDevice, VulkanDescriptorSetsInitInfo info)
			{
				allocateSets(logicalDevice, info.setCount);

				for (UI32 i = 0; i < info.setCount; i++)
				{
					for (UI32 _index = 0; _index < info.descriptorWrites.size(); _index++)
						info.descriptorWrites[_index].dstSet = descriptorSets[i];

					updateSet(logicalDevice, info.descriptorWrites);
				}
			}

			void VulkanDescriptor::updateSet(VkDevice logicalDevice, std::vector<VkWriteDescriptorSet> writes)
			{
				vkUpdateDescriptorSets(logicalDevice, writes.size(), writes.data(), 0, nullptr);
			}

			void VulkanDescriptor::terminate(VkDevice logicalDevice)
			{
				vkDestroyDescriptorSetLayout(logicalDevice, layout, nullptr);
				vkDestroyDescriptorPool(logicalDevice, pool, nullptr);
			}
		}
	}
}