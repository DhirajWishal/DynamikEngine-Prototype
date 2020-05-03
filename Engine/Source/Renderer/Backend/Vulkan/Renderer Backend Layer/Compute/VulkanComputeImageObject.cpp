#include "dmkafx.h"
#include "VulkanComputeImageObject.h"

#include "VulkanComputeFunctions.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			void VulkanComputeImageObject::initialize(VulkanComputeImageInputContainer container)
			{
				myInputTextureContainer = container.inputTextureData;
				_initializeTexture(container);

				_initializeDescriptorSetLayout();
				_initializePipelineLayout();
				_initializePipeline();
				_initializeDescriptorPool();
				_initializeDescriptorSets();
			}

			void VulkanComputeImageObject::_initializeTexture(VulkanComputeImageInputContainer container)
			{
				VkFormatProperties _formatProperties;
				vkGetPhysicalDeviceFormatProperties(physicalDevice, container.inputTextureData.format, &_formatProperties);

				if (!(_formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT))
					DMK_CORE_FATAL("Unable to create compute image object!");

				myOutputTextureContainer.format = container.inputTextureData.format;
				myOutputTextureContainer.width = container.inputTextureData.width;
				myOutputTextureContainer.height = container.inputTextureData.height;
				myOutputTextureContainer.mipLevels = 1;

				VulkanCreateImageInfo cinfo;
				cinfo.width = container.inputTextureData.width;
				cinfo.height = container.inputTextureData.height;
				cinfo.format = container.inputTextureData.format;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &myOutputTextureContainer.image;
				cinfo.imageMemory = &myOutputTextureContainer.imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
				cinfo.flags = NULL;
				VulkanComputeFunctions::createImage(logicalDevice, physicalDevice, cinfo);

				VulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = myOutputTextureContainer.image;
				transitionInfo.format = myOutputTextureContainer.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.mipLevels = 1;
				transitionInfo.layerCount = 1;
				VulkanComputeFunctions::transitionImageLayout(logicalDevice, commandPool, computeQueue, transitionInfo);

				VulkanCopyBufferToImageInfo cpyInfo;
				cpyInfo.buffer = container.dataBuffer;
				cpyInfo.image = myOutputTextureContainer.image;
				cpyInfo.width = static_cast<UI32>(container.inputTextureData.width);
				cpyInfo.height = static_cast<UI32>(container.inputTextureData.height);
				VulkanComputeFunctions::copyBufferToImage(logicalDevice, commandPool, computeQueue, cpyInfo);

				VulkanTextureSamplerInitInfo samplerInitInfo;
				samplerInitInfo.magFilter = VK_FILTER_LINEAR;
				samplerInitInfo.minFilter = VK_FILTER_LINEAR;
				samplerInitInfo.maxMipLevels = container.inputTextureData.mipLevels;
				samplerInitInfo.minMipLevels = 0;
				samplerInitInfo.modeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
				samplerInitInfo.modeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
				samplerInitInfo.modeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
				myOutputTextureContainer.imageSampler = VulkanUtilities::createImageSampler(logicalDevice, samplerInitInfo);

				VulkanCreateImageViewInfo cinfo2;
				cinfo2.image = myOutputTextureContainer.image;
				cinfo2.format = myOutputTextureContainer.format;
				cinfo2.mipLevels = container.inputTextureData.mipLevels;
				cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				myOutputTextureContainer.imageView = VulkanComputeFunctions::createImageView(logicalDevice, cinfo2);
			}

			void VulkanComputeImageObject::_initializeDescriptorSetLayout()
			{
				VulkanDescriptorSetLayoutInitInfo descriptorSetLayoutInitInfo;

				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
				descriptorSetLayoutInitInfo.bindings.push_back(uboLayoutBinding);

				VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
				samplerLayoutBinding.binding = 1; // info.bindIndex;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
				descriptorSetLayoutInitInfo.bindings.push_back(samplerLayoutBinding);

				myComputeData.computeDescriptor.initializeLayout(logicalDevice, descriptorSetLayoutInitInfo);
			}

			void VulkanComputeImageObject::_initializePipelineLayout()
			{
				VulkanGraphicsPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.layouts = { myComputeData.computeDescriptor.layout };
				myComputeData.computePipeline.initializePipelineLayout(logicalDevice, pipelineLayoutInitInfo);
			}

			void VulkanComputeImageObject::_initializePipeline()
			{
				VulkanComputePipelineInitInfo pipelineInitInfo;
				myComputeData.computePipeline.initializePipeline(logicalDevice, pipelineInitInfo);
			}

			void VulkanComputeImageObject::_initializeDescriptorPool()
			{
				VulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
				VkDescriptorPoolSize _poolSize2;
				_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				_poolSize2.descriptorCount = 1;
				descriptorPoolInitInfo.poolSizes.pushBack(_poolSize2);

				myComputeData.computeDescriptor.initializePool(logicalDevice, descriptorPoolInitInfo);
			}

			void VulkanComputeImageObject::_initializeDescriptorSets()
			{
				ARRAY<VkWriteDescriptorSet> descriptorWrites = {};
				ARRAY<VkDescriptorImageInfo> imageInfos;

				VkDescriptorImageInfo imageInfo = {};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = myOutputTextureContainer.imageView;
				imageInfo.sampler = myOutputTextureContainer.imageSampler;
				imageInfos.pushBack(imageInfo);

				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = myInputTextureContainer.imageView;
				imageInfo.sampler = myInputTextureContainer.imageSampler;
				imageInfos.pushBack(imageInfo);

				VkWriteDescriptorSet _writes1;
				_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes1.dstSet = VK_NULL_HANDLE;
				_writes1.dstBinding = 0;
				_writes1.dstArrayElement = 0;
				_writes1.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				_writes1.descriptorCount = 1;
				_writes1.pBufferInfo = VK_NULL_HANDLE;
				_writes1.pNext = VK_NULL_HANDLE;
				_writes1.pImageInfo = imageInfos.data();
				_writes1.pTexelBufferView = VK_NULL_HANDLE;
				descriptorWrites.push_back(_writes1);

				VulkanDescriptorSetsInitInfo descriptorSetInitInfo;
				descriptorSetInitInfo.descriptorWrites = descriptorWrites;
				myComputeData.computeDescriptor.initializeSets(logicalDevice, descriptorSetInitInfo);
			}
		}
	}
}