#include "adgrafx.h"
#include "VulkanComputeObject.h"

#include "../Objects/VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanComputeObject::initialize()
			{
				const int32_t bufferLength = 16384;

				const uint32_t bufferSize = sizeof(int32_t) * bufferLength;

				// we are going to need two buffers from this one memory
				const VkDeviceSize memorySize = bufferSize * 2;

				ADGRCreateBufferInfo info;
				info.buffer = &inputBuffer;
				info.bufferMemory = &inputBufferMemory;
				info.bufferMemoryPropertyflags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
				info.bufferSize = bufferSize;
				VulkanFunctions::createBuffer(logicalDevice, physicalDevice, info);

				info.buffer = &outputBuffer;
				info.bufferMemory = &outputBufferMemory;
				info.bufferMemoryPropertyflags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
				info.bufferSize = bufferSize;
				VulkanFunctions::createBuffer(logicalDevice, physicalDevice, info);

				ADGRVulkanDescriptorSetLayoutInitInfo descriptorSetLayoutInitInfo;
				myComputeData.computeDescriptor.initializeDescriptorSetLayout(logicalDevice, descriptorSetLayoutInitInfo);

				ADGRVulkanPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.layouts = { myComputeData.computeDescriptor.container.layout };
				myComputeData.computePipeline.initializePipelineLayout(logicalDevice, pipelineLayoutInitInfo);

				ADGRVulkanComputePipelineInitInfo pipelineInitInfo;
				myComputeData.computePipeline.initializePipeline(logicalDevice, pipelineInitInfo);

				ADGRVulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
				myComputeData.computeDescriptor.initializeDescriptorPool(logicalDevice, descriptorPoolInitInfo);

				ARRAY<VkWriteDescriptorSet> descriptorWrites = {};
				ARRAY<VkDescriptorBufferInfo> bufferInfos;
				
				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = inputBuffer;
				bufferInfo.offset = 0;
				bufferInfo.range = VK_WHOLE_SIZE;
				bufferInfos.pushBack(bufferInfo);
				
				bufferInfo.buffer = outputBuffer;
				bufferInfo.offset = 0;
				bufferInfo.range = VK_WHOLE_SIZE;
				bufferInfos.pushBack(bufferInfo);
				
				VkWriteDescriptorSet _writes1;
				_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes1.dstSet = myComputeData.computeDescriptor.container.descriptorSet;
				_writes1.dstBinding = 0;
				_writes1.dstArrayElement = 0;
				_writes1.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				_writes1.descriptorCount = 1;
				_writes1.pBufferInfo = bufferInfos.data();
				_writes1.pNext = VK_NULL_HANDLE;
				_writes1.pImageInfo = VK_NULL_HANDLE;
				_writes1.pTexelBufferView = VK_NULL_HANDLE;
				descriptorWrites.push_back(_writes1);

				ADGRVulkanDescriptorSetsInitInfo descriptorSetInitInfo;
				descriptorSetInitInfo.additionalWrites = descriptorWrites;
				myComputeData.computeDescriptor.initializeDescriptorSet(logicalDevice, descriptorSetInitInfo);
			}
		}
	}
}