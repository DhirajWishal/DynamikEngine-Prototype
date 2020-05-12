#include "dmkafx.h"
#include "VulkanComputeBufferObject.h"

#include "../Common/VulkanUtilities.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			void VulkanComputeBufferObject::initialize()
			{
				const int32_t bufferLength = 16384;

				const UI32 bufferSize = sizeof(int32_t) * bufferLength;

				// we are going to need two buffers from this one memory
				const VkDeviceSize memorySize = bufferSize * 2;

				VulkanCreateBufferInfo info;
				info.buffer = &inputBuffer;
				info.bufferMemory = &inputBufferMemory;
				info.bufferMemoryPropertyflags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
				info.bufferSize = bufferSize;
				VulkanUtilities::createBuffer(logicalDevice, physicalDevice, info);

				info.buffer = &outputBuffer;
				info.bufferMemory = &outputBufferMemory;
				info.bufferMemoryPropertyflags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
				info.bufferSize = bufferSize;
				VulkanUtilities::createBuffer(logicalDevice, physicalDevice, info);

				_initializeDescriptorSetLayout();
				_initializePipelineLayout();
				_initializePipeline();
				_initializeDescriptorPool();
				_initializeDescriptorSets();
			}

			void VulkanComputeBufferObject::_initializeDescriptorSetLayout()
			{
				VulkanDescriptorSetLayoutInitInfo descriptorSetLayoutInitInfo;

				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
				descriptorSetLayoutInitInfo.bindings.push_back(uboLayoutBinding);

				VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
				samplerLayoutBinding.binding = 1; // info.bindIndex;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
				descriptorSetLayoutInitInfo.bindings.push_back(samplerLayoutBinding);
				myComputeData.computeDescriptor.initializeLayout(logicalDevice, descriptorSetLayoutInitInfo);
			}

			void VulkanComputeBufferObject::_initializePipelineLayout()
			{
				VulkanGraphicsPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.layouts = { myComputeData.computeDescriptor.layout };
				myComputeData.computePipeline.initializePipelineLayout(logicalDevice, pipelineLayoutInitInfo);
			}

			void VulkanComputeBufferObject::_initializePipeline()
			{
				VulkanComputePipelineInitInfo pipelineInitInfo;
				myComputeData.computePipeline.initializePipeline(logicalDevice, pipelineInitInfo);
			}

			void VulkanComputeBufferObject::_initializeDescriptorPool()
			{
				VkDescriptorPoolSize _poolSize1;
				_poolSize1.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				_poolSize1.descriptorCount = 1;

				VulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
				descriptorPoolInitInfo.poolSizes.push_back(_poolSize1);
				myComputeData.computeDescriptor.initializePool(logicalDevice, descriptorPoolInitInfo);
			}

			void VulkanComputeBufferObject::_initializeDescriptorSets()
			{
				std::vector<VkWriteDescriptorSet> descriptorWrites = {};
				std::vector<VkDescriptorBufferInfo> bufferInfos;

				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = inputBuffer;
				bufferInfo.offset = 0;
				bufferInfo.range = VK_WHOLE_SIZE;
				bufferInfos.push_back(bufferInfo);

				bufferInfo.buffer = outputBuffer;
				bufferInfo.offset = 0;
				bufferInfo.range = VK_WHOLE_SIZE;
				bufferInfos.push_back(bufferInfo);

				VkWriteDescriptorSet _writes1;
				_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes1.dstSet = VK_NULL_HANDLE;
				_writes1.dstBinding = 0;
				_writes1.dstArrayElement = 0;
				_writes1.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				_writes1.descriptorCount = 1;
				_writes1.pBufferInfo = bufferInfos.data();
				_writes1.pNext = VK_NULL_HANDLE;
				_writes1.pImageInfo = VK_NULL_HANDLE;
				_writes1.pTexelBufferView = VK_NULL_HANDLE;
				descriptorWrites.push_back(_writes1);

				VulkanDescriptorSetsInitInfo descriptorSetInitInfo;
				descriptorSetInitInfo.descriptorWrites = descriptorWrites;
				myComputeData.computeDescriptor.initializeSets(logicalDevice, descriptorSetInitInfo);
			}
		}
	}
}