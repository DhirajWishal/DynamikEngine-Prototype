#include "dmkafx.h"
#include "VulkanGraphicsRenderableObject.h"

#include "../Common/VulkanValidator.h"
#include "../Common/VulkanExtensionsManager.h"
#include "VulkanGraphicsFunctions.h"
#include "VulkanGraphicsOneTimeCommandBuffer.h"
#include "Engines/ADGR/defines.h"

#include "../Common/VulkanUtilities.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanGraphicsRenderableObject::initializeResources(ADGRVulkanGraphicsRenderableObjectInitInfo info)
			{
				logicalDevice = info.logicalDevice;
				physicalDevice = info.physicalDevice;
				graphicsQueue = info.graphicsQueue;
				presentQueue = info.presentQueue;
				commandPool = info.commandPool;
			}

			ADGRVulkanRenderData VulkanGraphicsRenderableObject::initializeObject(VkDevice logicalDevice, POINTER<InternalFormat> format, VkSampleCountFlagBits msaaSamples)
			{
				ARRAY<VkDescriptorSetLayoutBinding> bindings;

				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				bindings.push_back(uboLayoutBinding);

				VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
				samplerLayoutBinding.binding = 1; // info.bindIndex;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				bindings.push_back(samplerLayoutBinding);

				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
				layoutInitInfo.bindings = bindings;
				myRenderData.descriptors.initializeLayout(logicalDevice, layoutInitInfo);

				ADGRVulkanGraphicsPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.layouts = { myRenderData.descriptors.layout };
				initializePipelineLayout(pipelineLayoutInitInfo);

				ARRAY<VulkanGraphicsShader> _shaders = VulkanUtilities::getGraphicsShaders(logicalDevice, format);

				// initialize pipeline
				ADGRVulkanGraphicsPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = _shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				pipelineInitInfo.vertexBindingDescription = VulkanUtilities::getBindingDescription(1);
				pipelineInitInfo.vertexAttributeDescription = VulkanUtilities::getAttributeDescriptions(descriptor.vertexBufferObjectDescription.attributes, 1);
				pipelineInitInfo.isTexturesAvailable = format->texturePaths.size();
				initializePipeline(pipelineInitInfo);

				VulkanUtilities::terminateGraphicsShaders(logicalDevice, _shaders);

				// initialize texture
				ADGRVulkanTextureInitInfo initInfo;
				initInfo.mipLevels = 1;
				initInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				initInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				initInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				initInfo.magFilter = VK_FILTER_LINEAR;
				initInfo.minFilter = VK_FILTER_LINEAR;
				initInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				initializeTextures(initInfo);

				// initialize vertex buffers
				initializeVertexBuffer();

				// initialize index buffers
				initializeIndexBuffer();

				// initialize uniform buffers
				initializeUniformBuffer();

				// initialize descriptor pool
				initializeDescriptorPool();

				// initialize descriptor sets
				initializeDescriptorSets();

				return myRenderData;
			}

			void VulkanGraphicsRenderableObject::initializePipelineLayout(ADGRVulkanGraphicsPipelineLayoutInitInfo info)
			{
				VulkanGraphicsPipeline _pipeline;
				_pipeline.initializePipelineLayout(logicalDevice, info);
				myRenderData.pipelineContainers.pushBack(_pipeline);
			}

			void VulkanGraphicsRenderableObject::initializePipeline(ADGRVulkanGraphicsPipelineInitInfo info)
			{
				if (!myRenderData.pipelineContainers.size())
					DMK_CORE_FATAL("Pipeline layout was not initialized!");

				info.renderPass = myRenderData.frameBufferPointer->renderPass;
				info.swapChainExtent = myRenderData.swapChainPointer->swapChainExtent;

				UI32 _index = myRenderData.pipelineContainers.size() - 1;
				myRenderData.pipelineContainers[_index].initializePipeline(logicalDevice, info);
			}

			void VulkanGraphicsRenderableObject::terminatePipeline()
			{
				for (VulkanGraphicsPipeline _container : myRenderData.pipelineContainers)
					_container.terminate(logicalDevice);
			}

			void VulkanGraphicsRenderableObject::initializeTextures(ADGRVulkanTextureInitInfo info)
			{
				for (auto mesh : meshDatas)
				{
					ADGRVulkanTextureContainer _container;

					_container.mipLevels = info.mipLevels;

					if (mesh.textureData.format == DMKFormat::DMK_FORMAT_RGBA_8_UNIFORM)
						_container.format = VK_FORMAT_R8G8B8A8_UNORM;
					else if (mesh.textureData.format == DMKFormat::DMK_FORMAT_RGB_8_UNIFORM)
						_container.format = VK_FORMAT_R8G8B8_UNORM;

					_container.width = mesh.textureData.width;
					_container.height = mesh.textureData.height;

					VkDeviceSize imageSize = mesh.textureData.size();

					VkBuffer stagingBuffer = VK_NULL_HANDLE;
					VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

					ADGRVulkanCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = imageSize;
					bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &stagingBuffer;
					bufferInfo.bufferMemory = &stagingBufferMemory;

					VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);

					void* data;
					if (vkMapMemory(logicalDevice, stagingBufferMemory, 0, static_cast<size_t>(imageSize), 0, &data) != VK_SUCCESS)
						DMK_CORE_FATAL("Failed to map memory!");
					memcpy(data, mesh.textureData.textureData, static_cast<size_t>(imageSize));
					vkUnmapMemory(logicalDevice, stagingBufferMemory);

					ADGRVulkanCreateImageInfo cinfo;
					cinfo.width = _container.width;
					cinfo.height = _container.height;
					cinfo.format = _container.format;
					cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
					cinfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
					cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
					cinfo.image = &_container.image;
					cinfo.imageMemory = &_container.imageMemory;
					cinfo.mipLevels = info.mipLevels;
					cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
					cinfo.flags = NULL;

					VulkanGraphicsFunctions::createImage(logicalDevice, physicalDevice, cinfo);

					ADGRVulkanTransitionImageLayoutInfo transitionInfo;
					transitionInfo.image = _container.image;
					transitionInfo.format = _container.format;
					transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					transitionInfo.mipLevels = info.mipLevels;
					transitionInfo.layerCount = 1;
					VulkanGraphicsFunctions::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

					ADGRVulkanCopyBufferToImageInfo cpyInfo;
					cpyInfo.buffer = stagingBuffer;
					cpyInfo.image = _container.image;
					cpyInfo.width = _container.width;
					cpyInfo.height = _container.height;
					VulkanGraphicsFunctions::copyBufferToImage(logicalDevice, commandPool, graphicsQueue, presentQueue, cpyInfo);

					transitionInfo.image = _container.image;
					transitionInfo.format = _container.format;
					transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					transitionInfo.mipLevels = info.mipLevels;
					transitionInfo.layerCount = 1;
					VulkanGraphicsFunctions::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

					vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
					vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

					generateMipMaps(&_container);

					ADGRVulkanTextureSamplerInitInfo samplerInitInfo;
					samplerInitInfo.magFilter = info.magFilter;
					samplerInitInfo.minFilter = info.minFilter;
					samplerInitInfo.maxMipLevels = info.maxMipLevels;
					samplerInitInfo.minMipLevels = info.minMipLevels;
					samplerInitInfo.modeU = info.modeU;
					samplerInitInfo.modeV = info.modeV;
					samplerInitInfo.modeW = info.modeW;
					_container.imageSampler = VulkanGraphicsFunctions::createImageSampler(logicalDevice, samplerInitInfo);

					ADGRVulkanCreateImageViewInfo cinfo2;
					cinfo2.image = _container.image;
					cinfo2.format = _container.format;
					cinfo2.mipLevels = _container.mipLevels;
					cinfo2.aspectFlags = info.aspectFlags;
					_container.imageView = VulkanGraphicsFunctions::createImageView(logicalDevice, cinfo2);

					myRenderData.textures.pushBack(_container);
				}
			}

			void  VulkanGraphicsRenderableObject::generateMipMaps(POINTER<ADGRVulkanTextureContainer> container)
			{
				UI32 _width = container->width, _height = container->height;

				VkFormatProperties formatProperties;
				vkGetPhysicalDeviceFormatProperties(physicalDevice, container->format, &formatProperties);

				if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
					DMK_CORE_FATAL("texture image format does not support linear blitting!");

				VulkanGraphicsOneTimeCommandBuffer oneTimeCommandBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue);
				VkCommandBuffer _commandBuffer = oneTimeCommandBuffer.myCommandBuffers[0];

				VkImageMemoryBarrier barrier = {};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.image = container->image;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.baseArrayLayer = 0;
				barrier.subresourceRange.layerCount = 1;
				barrier.subresourceRange.levelCount = 1;

				for (UI32 i = 1; i < container->mipLevels; i++) {
					barrier.subresourceRange.baseMipLevel = i - 1;
					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

					vkCmdPipelineBarrier(_commandBuffer,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
						0, nullptr,
						0, nullptr,
						1, &barrier);

					VkImageBlit blit = {};
					blit.srcOffsets[0] = { 0, 0, 0 };
					blit.srcOffsets[1] = { (I32)_width, (I32)_height, 1 };
					blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.srcSubresource.mipLevel = i - 1;
					blit.srcSubresource.baseArrayLayer = 0;
					blit.srcSubresource.layerCount = 1;
					blit.dstOffsets[0] = { 0, 0, 0 };
					blit.dstOffsets[1] = { (I32)_width > 1 ? (I32)_width / 2 : 1, (I32)_height > 1 ? (I32)_height / 2 : 1, 1 };
					blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.dstSubresource.mipLevel = i;
					blit.dstSubresource.baseArrayLayer = 0;
					blit.dstSubresource.layerCount = 1;

					vkCmdBlitImage(_commandBuffer,
						container->image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						container->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						1, &blit,
						VK_FILTER_LINEAR);

					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					vkCmdPipelineBarrier(_commandBuffer,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
						0, nullptr,
						0, nullptr,
						1, &barrier);

					if (_width > 1) _width /= 2;
					if (_height > 1) _height /= 2;
				}
			}

			void VulkanGraphicsRenderableObject::terminateTextures()
			{
				for (ADGRVulkanTextureContainer container : myRenderData.textures) {
					vkDestroySampler(logicalDevice, container.imageSampler, nullptr);
					vkDestroyImageView(logicalDevice, container.imageView, nullptr);

					vkDestroyImage(logicalDevice, container.image, nullptr);
					vkFreeMemory(logicalDevice, container.imageMemory, nullptr);
				}
			}

			void VulkanGraphicsRenderableObject::initializeVertexBuffer()
			{
				for (auto mesh : meshDatas)
				{
					VkBuffer _buffer = VK_NULL_HANDLE;
					VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

					myRenderData.vertexCount = mesh.vertexDataStore.size();
					VkDeviceSize bufferSize = myRenderData.vertexCount * DMKVertexBufferObjectDescriptor::vertexByteSize(descriptor.vertexBufferObjectDescription.attributes);

					VkBuffer stagingBuffer;
					VkDeviceMemory stagingBufferMemory;

					ADGRVulkanCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = bufferSize;
					bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &stagingBuffer;
					bufferInfo.bufferMemory = &stagingBufferMemory;

					VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);

					void* data = nullptr;
					vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
					mesh.packData(descriptor.vertexBufferObjectDescription.attributes, data);
					vkUnmapMemory(logicalDevice, stagingBufferMemory);

					ADGRVulkanCreateBufferInfo vertBufferInfo;
					vertBufferInfo.bufferSize = bufferSize;
					vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
					vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
					vertBufferInfo.buffer = &_buffer;
					vertBufferInfo.bufferMemory = &_bufferMemory;

					VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, vertBufferInfo);

					VulkanGraphicsFunctions::copyBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue, stagingBuffer, _buffer, bufferSize);

					vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
					vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

					myRenderData.vertexBuffers.pushBack(_buffer);
					myRenderData.vertexBufferMemories.pushBack(_bufferMemory);
				}
			}

			void VulkanGraphicsRenderableObject::terminateVertexBuffer()
			{
				for (UI32 _itr = 0; _itr < myRenderData.vertexBuffers.size(); _itr++)
				{
					vkDestroyBuffer(logicalDevice, myRenderData.vertexBuffers[_itr], nullptr);
					vkFreeMemory(logicalDevice, myRenderData.vertexBufferMemories[_itr], nullptr);
				}
			}

			void VulkanGraphicsRenderableObject::initializeIndexBuffer()
			{
				for (auto mesh : meshDatas)
				{
					myRenderData.indexbufferObjectTypeSize = (UI32)descriptor.indexBufferType;
					VkBuffer _buffer = VK_NULL_HANDLE;
					VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

					myRenderData.indexCount = mesh.indexes.size();
					VkDeviceSize bufferSize = myRenderData.indexCount * myRenderData.indexbufferObjectTypeSize;

					VkBuffer stagingBuffer;
					VkDeviceMemory stagingBufferMemory;

					ADGRVulkanCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = bufferSize;
					bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &stagingBuffer;
					bufferInfo.bufferMemory = &stagingBufferMemory;

					VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);

					void* data = nullptr;
					vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
					memcpy(data, mesh.indexes.data(), (size_t)bufferSize);
					vkUnmapMemory(logicalDevice, stagingBufferMemory);

					ADGRVulkanCreateBufferInfo indexBufferInfo;
					indexBufferInfo.bufferSize = bufferSize;
					indexBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
					indexBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
					indexBufferInfo.buffer = &_buffer;
					indexBufferInfo.bufferMemory = &_bufferMemory;

					VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, indexBufferInfo);

					VulkanGraphicsFunctions::copyBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue, stagingBuffer, _buffer, bufferSize);

					vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
					vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

					myRenderData.indexBuffers.pushBack(_buffer);
					myRenderData.indexBufferMemories.pushBack(_bufferMemory);
				}
			}

			void VulkanGraphicsRenderableObject::terminateIndexBuffer()
			{
				for (UI32 _itr = 0; _itr < myRenderData.indexBuffers.size(); _itr++)
				{
					vkDestroyBuffer(logicalDevice, myRenderData.indexBuffers[_itr], nullptr);
					vkFreeMemory(logicalDevice, myRenderData.indexBufferMemories[_itr], nullptr);
				}
			}

			void VulkanGraphicsRenderableObject::initializeDescriptorPool()
			{
				ARRAY<VkDescriptorPoolSize> poolSizes = {};

				VkDescriptorPoolSize _poolSize1;
				_poolSize1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_poolSize1.descriptorCount = 1;
				poolSizes.push_back(_poolSize1);

				if (myRenderData.textures.size())
				{
					VkDescriptorPoolSize _poolSize2;
					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = myRenderData.textures.size();

					poolSizes.push_back(_poolSize2);
				}

				ADGRVulkanDescriptorPoolInitInfo initInfo;
				initInfo.poolSizes = poolSizes;
				myRenderData.descriptors.initializePool(logicalDevice, initInfo);
			}

			void VulkanGraphicsRenderableObject::terminateDescriptorPool()
			{
				vkDestroyDescriptorPool(logicalDevice, myRenderData.descriptors.pool, nullptr);
			}

			void VulkanGraphicsRenderableObject::initializeDescriptorSets()
			{
				ARRAY<VkWriteDescriptorSet> descriptorWrites = {};
				ARRAY<VkDescriptorBufferInfo> bufferInfos;

				for (UI32 itr = 0; itr < myRenderData.uniformBufferContainers.size(); itr++)
				{
					for (UI32 i = 0; i < myRenderData.uniformBufferContainers[itr].buffers.size(); i++)
					{
						VkDescriptorBufferInfo bufferInfo = {};
						bufferInfo.buffer = myRenderData.uniformBufferContainers[itr].buffers[i];
						bufferInfo.offset = 0;
						bufferInfo.range = sizeof(UniformBufferObject);
						bufferInfos.pushBack(bufferInfo);
					}
				}

				VkWriteDescriptorSet _writes1;
				_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes1.dstBinding = 0;
				_writes1.dstArrayElement = 0;
				_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_writes1.descriptorCount = 1;
				_writes1.pBufferInfo = bufferInfos.data();
				_writes1.pNext = VK_NULL_HANDLE;
				_writes1.pImageInfo = VK_NULL_HANDLE;
				_writes1.pTexelBufferView = VK_NULL_HANDLE;
				descriptorWrites.push_back(_writes1);

				if (myRenderData.textures.size())
				{
					for (UI32 _texIndex = 0; _texIndex < myRenderData.textures.size(); _texIndex++)
					{
						VkDescriptorImageInfo imageInfo = {};
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						imageInfo.imageView = myRenderData.textures[_texIndex].imageView;
						imageInfo.sampler = myRenderData.textures[_texIndex].imageSampler;

						VkWriteDescriptorSet _writes2;
						_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes2.dstBinding = 1;
						_writes2.dstArrayElement = 0;
						_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						_writes2.descriptorCount = 1;
						_writes2.pImageInfo = &imageInfo;
						_writes2.pNext = VK_NULL_HANDLE;
						_writes2.pTexelBufferView = VK_NULL_HANDLE;
						_writes2.pBufferInfo = VK_NULL_HANDLE;
						descriptorWrites.push_back(_writes2);
					}
				}

				ADGRVulkanDescriptorSetsInitInfo initInfo;
				initInfo.descriptorWrites = descriptorWrites;
				myRenderData.descriptors.initializeSets(logicalDevice, initInfo);
			}

			void VulkanGraphicsRenderableObject::initializeUniformBuffer()
			{
				myRenderData.uniformBufferContainers.pushBack(
					VulkanGraphicsFunctions::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UniformBufferObject),
						myRenderData.swapChainPointer->swapChainImages.size()
						)
					);
			}

			void VulkanGraphicsRenderableObject::updateUniformBuffer(UniformBufferObject uniformBuferObject, UI32 currentImage)
			{
				for (ADGRVulkanUnformBufferContainer _container : myRenderData.uniformBufferContainers)
				{
					void* data = nullptr;
					vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uniformBuferObject), 0, &data);
					memcpy(data, &uniformBuferObject, sizeof(uniformBuferObject));
					vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);
				}
			}

			void VulkanGraphicsRenderableObject::terminateUniformBuffer()
			{
				for (ADGRVulkanUnformBufferContainer _container : myRenderData.uniformBufferContainers)
				{
					for (I32 x = 0; x < _container.buffers.size(); x++) {
						vkDestroyBuffer(logicalDevice, _container.buffers[x], nullptr);
						vkFreeMemory(logicalDevice, _container.bufferMemories[x], nullptr);
					}
				}
			}

			void VulkanGraphicsRenderableObject::initializeNoTextureDescriptorSetLayout()
			{
				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

				ARRAY<VkDescriptorSetLayoutBinding> bindings;
				bindings.push_back(uboLayoutBinding);

				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = static_cast<UI32>(bindings.size());
				layoutInfo.pBindings = bindings.data();

				if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &noTextureDescriptorSetLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor set layout!");
			}

			void VulkanGraphicsRenderableObject::initializeNoTexturePipelineLayout()
			{
				VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
				pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipelineLayoutInfo.setLayoutCount = 1;
				pipelineLayoutInfo.pSetLayouts = &noTextureDescriptorSetLayout;

				/*
				if (false) {
					ARRAY<VkPushConstantRange> pushConstantInfos;
					pushConstants.resize(info.pushConstantCount);

					// initialize push constants
					for (I32 i = 0; i <= info.pushConstantCount; i++) {
						VkPushConstantRange pushConsInfo = {};
						pushConsInfo.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
						pushConsInfo.size = pushConstants.typeSize() * info.pushConstantCount;
						pushConsInfo.offset = info.pushConstantOffset;
						//pushConsInfo.offset = pushConstants.typeSize() * i;

						pushConstantInfos.push_back(pushConsInfo);
					}
					pipelineLayoutInfo.pushConstantRangeCount = info.pushConstantCount;	// make support for multiple
					pipelineLayoutInfo.pPushConstantRanges = pushConstantInfos.data();
				}*/

				pipelineLayoutInfo.pushConstantRangeCount = 0;	// make support for multiple
				pipelineLayoutInfo.pPushConstantRanges = nullptr;

				// create the pipeline layout
				VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
				if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create pipeline layout!");
			}
		}
	}
}