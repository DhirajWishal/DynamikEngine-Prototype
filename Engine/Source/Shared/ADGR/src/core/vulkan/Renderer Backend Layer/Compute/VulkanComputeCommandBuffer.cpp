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
				buffers.resize(info.computeDatas.size());

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = pool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = static_cast<UI32>(info.computeDatas.size());

				if (vkAllocateCommandBuffers(logicalDevice, &allocInfo, buffers.data()) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate compute command buffers!");

				for (size_t i = 0; i < info.computeDatas.size(); i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					if (buffers.size() == 1)
						beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
					else
						beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

					if (vkBeginCommandBuffer(buffers[i], &beginInfo) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to begin recording compute command commandBuffers[i]!");

					vkCmdBindPipeline(buffers[i], VK_PIPELINE_BIND_POINT_COMPUTE, info.computeDatas[i].computePipeline.pipeline);

						vkCmdBindDescriptorSets(buffers[i], VK_PIPELINE_BIND_POINT_COMPUTE,
							info.computeDatas[i].computePipeline.layout, 0, 1, &info.computeDatas[i].computeDescriptor.descriptorSet, 0, 0);

					vkCmdDispatch(buffers[i], info.computeDatas[i].groupCountX, info.computeDatas[i].groupCountY, info.computeDatas[i].groupCountZ);

					if (vkEndCommandBuffer(buffers[i]) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to record compute command commandBuffers[i]!");
				}
			}
		}
	}
}