#include "adgrafx.h"
#include "VulkanSkyBox.h"

#include "Objects/VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanSkyBox::VulkanSkyBox(ADGRVulkanRenderableObjectInitInfo info)
				: VulkanRenderableObject(info)
			{
				myRenderData.type = DMKObjectType::DMK_OBJECT_TYPE_SKYBOX;
			}

			void VulkanSkyBox::initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos)
			{
				VkPhysicalDeviceFeatures _features;
				vkGetPhysicalDeviceFeatures(physicalDevice, &_features);

				bool found = false;

				for (const ADGRVulkanTextureInitInfo& info : infos)
				{
					if (found)
						break;

					gli::texture ktxTexture;

					ADGRVulkanTextureContainer _container;

					if (info.path.find("astc") != std::string::npos)
					{
						if (!_features.textureCompressionASTC_LDR)
							continue;

						_container.format = VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
					}
					else if (info.path.find("bc3") != std::string::npos)
					{
						if (!_features.textureCompressionBC)
							continue;

						_container.format = VK_FORMAT_BC2_UNORM_BLOCK;
					}
					else if (info.path.find("etx2") != std::string::npos)
					{
						if (!_features.textureCompressionETC2)
							continue;

						_container.format = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
					}
					found = true;

					ktxTexture = gli::load_ktx(info.path.c_str());

					if (ktxTexture.empty())
						DMK_CORE_FATAL("Invalid texture format!");

					_container.mipLevels = 1;
					_container.width = ktxTexture.extent().x;
					_container.height = ktxTexture.extent().y;

					VkBuffer stagingBuffer = VK_NULL_HANDLE;
					VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

					ADGRCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = ktxTexture.size();
					bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &stagingBuffer;
					bufferInfo.bufferMemory = &stagingBufferMemory;

					VulkanFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);

					void* data;
					if (vkMapMemory(logicalDevice, stagingBufferMemory, 0, static_cast<size_t>(ktxTexture.size()), 0, &data) != VK_SUCCESS)
						DMK_CORE_FATAL("Failed to map memory!");
					memcpy(data, ktxTexture.data(), static_cast<size_t>(ktxTexture.size()));
					vkUnmapMemory(logicalDevice, stagingBufferMemory);

					ADGRCreateImageInfo cinfo;
					cinfo.width = _container.width;
					cinfo.height = _container.height;
					cinfo.format = _container.format;
					cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
					cinfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
					cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
					cinfo.image = &_container.image;
					cinfo.imageMemory = &_container.imageMemory;
					cinfo.mipLevels = _container.mipLevels;
					cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
					cinfo.arrayLayers = ktxTexture.max_face() + 1;
					cinfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

					VulkanFunctions::createImage(logicalDevice, physicalDevice, cinfo);

					std::vector<VkBufferImageCopy> bufferCopyRegions;
					for (uint32_t face = 0; face < ktxTexture.max_face() + 1; face++)
					{
						for (uint32_t level = 0; level < _container.mipLevels; level++)
						{
							// Calculate offset into staging buffer for the current mip level and face
							VkBufferImageCopy bufferCopyRegion = {};
							bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
							bufferCopyRegion.imageSubresource.mipLevel = level;
							bufferCopyRegion.imageSubresource.baseArrayLayer = face;
							bufferCopyRegion.imageSubresource.layerCount = 1;
							bufferCopyRegion.imageExtent.width = ktxTexture.extent().x >> level;
							bufferCopyRegion.imageExtent.height = ktxTexture.extent().y >> level;
							bufferCopyRegion.imageExtent.depth = 1;
							bufferCopyRegion.bufferOffset = 0;
							bufferCopyRegions.push_back(bufferCopyRegion);
						}
					}

					ADGRTransitionImageLayoutInfo transitionInfo;
					transitionInfo.image = _container.image;
					transitionInfo.format = _container.format;
					transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					transitionInfo.mipLevels = info.mipLevels;
					transitionInfo.layerCount = ktxTexture.max_face() + 1;
					VulkanFunctions::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

					ADGRCopyBufferToImageInfo cpyInfo;
					cpyInfo.buffer = stagingBuffer;
					cpyInfo.image = _container.image;
					cpyInfo.destinationImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					VulkanFunctions::copyBufferToImageOverride(logicalDevice, commandPool, graphicsQueue, presentQueue, cpyInfo, bufferCopyRegions);

					//generateMipMaps(&_container);

					ADGRVulkanTextureSamplerInitInfo samplerInitInfo;
					samplerInitInfo.magFilter = info.magFilter;
					samplerInitInfo.minFilter = info.minFilter;
					samplerInitInfo.maxMipLevels = info.maxMipLevels;
					samplerInitInfo.minMipLevels = info.minMipLevels;
					samplerInitInfo.modeU = info.modeU;
					samplerInitInfo.modeV = info.modeV;
					samplerInitInfo.modeW = info.modeW;
					samplerInitInfo.mipLoadBias = 0.0f;
					samplerInitInfo.compareOp = VK_COMPARE_OP_NEVER;
					samplerInitInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
					initializeTextureSampler(samplerInitInfo, &_container.imageSampler);

					ADGRCreateImageViewInfo cinfo2;
					cinfo2.image = _container.image;
					cinfo2.format = _container.format;
					cinfo2.mipLevels = _container.mipLevels;
					cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					cinfo2.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
					cinfo2.layerCount = 6;
					cinfo2.component = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
					_container.imageView = VulkanFunctions::createImageView(logicalDevice, cinfo2);

					vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
					vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

					myRenderData.textures.pushBack(_container);
				}
			}

			struct StaggingBufferContainer {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			ADGRVulkanTextureContainer VulkanSkyBox::loadCubeMapImage(ARRAY<std::string> paths, VkFormat format)
			{
				ARRAY<StaggingBufferContainer> bufferContainers;
				UI32 width = 0;
				UI32 height = 0;

				for (const std::string& path : paths)
				{
					resource::TextureData texData;
					unsigned char* pixels = nullptr;

					if (format == VK_FORMAT_R8G8B8A8_UNORM)
						pixels = texData.loadTexture((path), resource::TEXTURE_TYPE_RGBA);
					else if (format == VK_FORMAT_R8G8B8_UNORM)
						pixels = texData.loadTexture((path), resource::TEXTURE_TYPE_RGB);
					else
						DMK_CORE_FATAL("Invalid texture format!");

					VkDeviceSize imageSize = texData.size;

					if (!pixels)
						DMK_CORE_FATAL("failed to load texture image!");

					StaggingBufferContainer _container;

					ADGRCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = imageSize;
					bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &_container.buffer;
					bufferInfo.bufferMemory = &_container.bufferMemory;

					VulkanFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);

					void* data;
					if (vkMapMemory(logicalDevice, _container.bufferMemory, 0, static_cast<size_t>(imageSize), 0, &data) != VK_SUCCESS)
						DMK_CORE_FATAL("Failed to map memory!");
					memcpy(data, pixels, static_cast<size_t>(imageSize));
					vkUnmapMemory(logicalDevice, _container.bufferMemory);

					texData.freeData(pixels);
					bufferContainers.pushBack(_container);
				}

				ADGRVulkanTextureContainer _container;

				ADGRCreateImageInfo cinfo;
				cinfo.width = width;
				cinfo.height = height;
				cinfo.format = format;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &_container.image;
				cinfo.imageMemory = &_container.imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
				cinfo.flags = NULL;

				VulkanFunctions::createImage(logicalDevice, physicalDevice, cinfo);

				ADGRTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = _container.image;
				transitionInfo.format = _container.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.mipLevels = 1;
				transitionInfo.layerCount = 6;
				VulkanFunctions::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

				for (StaggingBufferContainer _bufferContainer : bufferContainers)
				{
					ADGRCopyBufferToImageInfo cpyInfo;
					cpyInfo.buffer = _bufferContainer.buffer;
					cpyInfo.image = _container.image;
					cpyInfo.width = width;
					cpyInfo.height = height;
					VulkanFunctions::copyBufferToImage(logicalDevice, commandPool, graphicsQueue, presentQueue, cpyInfo);

					vkDestroyBuffer(logicalDevice, _bufferContainer.buffer, nullptr);
					vkFreeMemory(logicalDevice, _bufferContainer.bufferMemory, nullptr);
				}
				return _container;
			}

			void VulkanSkyBox::initializeUniformBuffer()
			{
				VkDeviceSize bufferSize = sizeof(UBO_MP);
				UI32 count = myRenderData.swapChainPointer->getSwapChainImages().size();

				myRenderData.uniformBuffers.resize(count);
				myRenderData.uniformBufferMemories.resize(count);

				for (size_t i = 0; i < count; i++)
				{
					ADGRCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = bufferSize;
					bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &myRenderData.uniformBuffers[i];
					bufferInfo.bufferMemory = &myRenderData.uniformBufferMemories[i];

					VulkanFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);
				}
			}

			void VulkanSkyBox::updateUniformBuffer(UBO_MP uniformBufferObject, UI32 currentImage)
			{
				void* data = nullptr;
				vkMapMemory(logicalDevice, myRenderData.uniformBufferMemories[currentImage], 0, sizeof(uniformBufferObject), 0, &data);
				memcpy(data, &uniformBufferObject, sizeof(uniformBufferObject));
				vkUnmapMemory(logicalDevice, myRenderData.uniformBufferMemories[currentImage]);
			}

			void VulkanSkyBox::initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info)
			{
				myRenderData.descriptors.descriptorSets.resize(myRenderData.uniformBuffers.size());

				VkDescriptorSetLayout _layout = VK_NULL_HANDLE;
				if (myRenderData.textures.size())
					_layout = myRenderData.swapChainPointer->getDescriptorSetLayout();
				else
					_layout = noTextureDescriptorSetLayout;

				for (UI32 itr = 0; itr < myRenderData.uniformBuffers.size(); itr++) {
					VkDescriptorSetAllocateInfo allocInfo = {};
					allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
					allocInfo.descriptorPool = myRenderData.descriptors.descriptorPools[itr];
					allocInfo.descriptorSetCount = 1;
					allocInfo.pSetLayouts = &_layout;

					VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
					if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, &_descriptorSet) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to allocate descriptor sets!");

					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = myRenderData.uniformBuffers[itr];
					bufferInfo.offset = 0;
					bufferInfo.range = sizeof(UBO_MP);

					ARRAY<VkWriteDescriptorSet> descriptorWrites = {};

					VkWriteDescriptorSet _writes1;
					_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes1.dstSet = _descriptorSet;
					_writes1.dstBinding = 0;
					_writes1.dstArrayElement = 0;
					_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					_writes1.descriptorCount = 1;
					_writes1.pBufferInfo = &bufferInfo;
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
							_writes2.dstSet = _descriptorSet;
							_writes2.dstBinding = 1;
							_writes2.dstArrayElement = 0;
							_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
							_writes2.descriptorCount = 1;
							_writes2.pImageInfo = &imageInfo;
							_writes2.pNext = VK_NULL_HANDLE;
							_writes2.pTexelBufferView = VK_NULL_HANDLE;
							descriptorWrites.push_back(_writes2);
						}
					}

					for (VkWriteDescriptorSet _write : info.additionalWrites)
						descriptorWrites.push_back(_write);

					vkUpdateDescriptorSets(logicalDevice, static_cast<UI32>(descriptorWrites.size()),
						descriptorWrites.data(), 0, nullptr);

					myRenderData.descriptors.descriptorSets.pushBack(_descriptorSet);
				} // make two descriptor layouts for each descriptor set
			}
		}
	}
}