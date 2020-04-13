#include "adgrafx.h"
#include "VulkanComputeCommandBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanComputeCommandBuffer::initializeCommandPool(VkDevice device, UI32 computeFamilyIndex)
			{
				VkCommandPoolCreateInfo createInfo;
				createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				createInfo.pNext = VK_NULL_HANDLE;
				createInfo.flags = VK_NULL_HANDLE;
				createInfo.queueFamilyIndex = computeFamilyIndex;
			}
			
			void VulkanComputeCommandBuffer::initializeCommandBuffers(VkDevice logicalDevice, ADGRVulkanComputeCommandBufferInitInfo info)
			{
				buffers.resize(info.count);

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = pool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = static_cast<UI32>(info.count);

				if (vkAllocateCommandBuffers(logicalDevice, &allocInfo, buffers.data()) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate compute command buffers!");

				for (size_t i = 0; i < info.count; i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

					if (vkBeginCommandBuffer(buffers[i], &beginInfo) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to begin recording compute command commandBuffers[i]!");

					for (UI32 _itr = 0; _itr < info.computeDatas.size(); _itr++)
					{
						vkCmdBindPipeline(buffers[i], VK_PIPELINE_BIND_POINT_COMPUTE, info.computeDatas[_itr].computePipeline.pipeline);
					
						vkCmdBindDescriptorSets(buffers[i], VK_PIPELINE_BIND_POINT_COMPUTE,
							info.computeDatas[_itr].computePipeline.layout, 0, 1,
							&info.computeDatas[_itr].computeDescriptor.container.descriptorSet, 0, 0);

						vkCmdDispatch(buffers[i], info.computeDatas[_itr].groupCountX, info.computeDatas[_itr].groupCountY, info.computeDatas[_itr].groupCountZ);
					}

					if (vkEndCommandBuffer(buffers[i]) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to record compute command commandBuffers[i]!");
				}
			}
		}
	}
}