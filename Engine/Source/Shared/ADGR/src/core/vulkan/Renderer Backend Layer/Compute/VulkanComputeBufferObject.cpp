#include "adgrafx.h"
#include "VulkanComputeBufferObject.h"

#include "../Graphics/VulkanGraphicsFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanComputeBufferObject::initialize()
			{
				const int32_t bufferLength = 16384;

				const UI32 bufferSize = sizeof(int32_t) * bufferLength;

				// we are going to need two buffers from this one memory
				const VkDeviceSize memorySize = bufferSize * 2;

				ADGRVulkanCreateBufferInfo info;
				info.buffer = &inputBuffer;
				info.bufferMemory = &inputBufferMemory;
				info.bufferMemoryPropertyflags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
				info.bufferSize = bufferSize;
				VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, info);

				info.buffer = &outputBuffer;
				info.bufferMemory = &outputBufferMemory;
				info.bufferMemoryPropertyflags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
				info.bufferSize = bufferSize;
				VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, info);

				_initializeDescriptorSetLayout();
				_initializePipelineLayout();
				_initializePipeline();
				_initializeDescriptorPool();
				_initializeDescriptorSets();
			}

			void VulkanComputeBufferObject::_initializeDescriptorSetLayout()
			{
				ADGRVulkanDescriptorSetLayoutInitInfo descriptorSetLayoutInitInfo;

				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
				descriptorSetLayoutInitInfo.additionalBindings.push_back(uboLayoutBinding);

				VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
				samplerLayoutBinding.binding = 1; // info.bindIndex;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
				descriptorSetLayoutInitInfo.additionalBindings.push_back(samplerLayoutBinding);

				myComputeData.computeDescriptor.initializeDescriptorSetLayout(logicalDevice, descriptorSetLayoutInitInfo);
			}

			void VulkanComputeBufferObject::_initializePipelineLayout()
			{
				ADGRVulkanGraphicsPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.layouts = { myComputeData.computeDescriptor.container.layout };
				myComputeData.computePipeline.initializePipelineLayout(logicalDevice, pipelineLayoutInitInfo);
			}

			void VulkanComputeBufferObject::_initializePipeline()
			{
				ADGRVulkanComputePipelineInitInfo pipelineInitInfo;
				myComputeData.computePipeline.initializePipeline(logicalDevice, pipelineInitInfo);
			}

			void VulkanComputeBufferObject::_initializeDescriptorPool()
			{
				VkDescriptorPoolSize _poolSize1;
				_poolSize1.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				_poolSize1.descriptorCount = 1;

				ADGRVulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
				descriptorPoolInitInfo.additionalSizes.pushBack(_poolSize1);
				myComputeData.computeDescriptor.initializeDescriptorPool(logicalDevice, descriptorPoolInitInfo);
			}

			void VulkanComputeBufferObject::_initializeDescriptorSets()
			{
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

				ADGRVulkanDescriptorSetsInitInfo descriptorSetInitInfo;
				descriptorSetInitInfo.additionalWrites = descriptorWrites;
				myComputeData.computeDescriptor.initializeDescriptorSet(logicalDevice, descriptorSetInitInfo);
			}
		}
	}
}