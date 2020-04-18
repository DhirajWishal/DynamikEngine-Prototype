#include "dmkafx.h"
#include "VulkanGraphicsUniformBufferAttachment.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanGraphicsUniformBufferAttachment::VulkanGraphicsUniformBufferAttachment()
			{
				type = ADGRVulkanAttachmentType::ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_UNIFORM_BUFFER;
			}

			VulkanGraphicsUniformBufferAttachment::VulkanGraphicsUniformBufferAttachment(ADGRVulkanAttachmentInitInfo info) :
				VulkanGraphicsAttachment(info)
			{
				type = ADGRVulkanAttachmentType::ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_UNIFORM_BUFFER;
			}

			void VulkanGraphicsUniformBufferAttachment::initializeDescriptor(UI32 destination, VkShaderStageFlags shaderStage)
			{
				/* Initialize Descriptor Set Layout */
				_initializeDescriptorLayout(destination, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, shaderStage);

				/* Initialize Descriptor Pool */
				VkDescriptorPoolSize _poolSize1;
				_poolSize1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_poolSize1.descriptorCount = 1;

				ADGRVulkanDescriptorPoolInitInfo poolInitInfo;
				poolInitInfo.maxDescriptorSets = 1;
				poolInitInfo.poolSizes = { _poolSize1 };
				descriptor.initializePool(logicalDevice, poolInitInfo);

				/* Initialize Descriptor Sets */
				ARRAY<VkDescriptorBufferInfo> bufferInfos;
				for (UI32 itr = 0; itr < bufferContainer.buffers.size(); itr++)
				{
					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = bufferContainer.buffers[itr];
					bufferInfo.offset = bufferOffset;
					bufferInfo.range = bufferSize;
					bufferInfos.pushBack(bufferInfo);
				}

				VkWriteDescriptorSet _writes1;
				_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes1.dstBinding = destination;
				_writes1.dstArrayElement = 0;
				_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_writes1.descriptorCount = 1;
				_writes1.pBufferInfo = bufferInfos.data();
				_writes1.pNext = VK_NULL_HANDLE;
				_writes1.pImageInfo = VK_NULL_HANDLE;
				_writes1.pTexelBufferView = VK_NULL_HANDLE;

				ADGRVulkanDescriptorSetsInitInfo setInitInfo;
				setInitInfo.descriptorWrites = { _writes1 };
				setInitInfo.setCount = 1;
				descriptor.initializeSets(logicalDevice, setInitInfo);
			}

			void VulkanGraphicsUniformBufferAttachment::initializeUniformBuffer(VkDeviceSize size, UI32 count, UI32 offset)
			{
				bufferSize = size;
				bufferOffset = offset;
				bufferContainer = VulkanGraphicsFunctions::createUniformBuffers(logicalDevice, physicalDevice, bufferSize, count);
			}

			void VulkanGraphicsUniformBufferAttachment::updateUniformBuffer(VPTR data, UI32 imageIndex)
			{
				VPTR _data = nullptr;
				vkMapMemory(logicalDevice, bufferContainer.bufferMemories[imageIndex], 0, bufferSize, 0, &_data);
				memcpy(_data, data, bufferSize);
				vkUnmapMemory(logicalDevice, bufferContainer.bufferMemories[imageIndex]);
			}

			void VulkanGraphicsUniformBufferAttachment::terminate()
			{
				for (UI32 itr = 0; itr < bufferContainer.buffers.size(); itr++)
				{
					vkDestroyBuffer(logicalDevice, bufferContainer.buffers[itr], nullptr);
					vkFreeMemory(logicalDevice, bufferContainer.bufferMemories[itr], nullptr);
				}
			}
		}
	}
}