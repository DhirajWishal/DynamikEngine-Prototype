#include "dmkafx.h"
#include "VulkanGraphicsTextureAttachmentCube.h"

#include "../VulkanGraphicsFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanGraphicsTextureAttachmentCube::VulkanGraphicsTextureAttachmentCube()
			{
				type = ADGRVulkanAttachmentType::ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_TEXTURE_CUBEMAP;
			}

			VulkanGraphicsTextureAttachmentCube::VulkanGraphicsTextureAttachmentCube(ADGRVulkanAttachmentInitInfo info) : 
				VulkanGraphicsTextureAttachment(info)
			{
				type = ADGRVulkanAttachmentType::ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_TEXTURE_CUBEMAP;
			}
			
			void VulkanGraphicsTextureAttachmentCube::initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos)
			{
				ADGRVulkanTextureContainer _container;
				if ((infos[0].path.find(".ktx") != std::string::npos) && (infos.size() < 6))
				{
					VkPhysicalDeviceFeatures _features;
					vkGetPhysicalDeviceFeatures(physicalDevice, &_features);

					bool found = false;

					for (const ADGRVulkanTextureInitInfo& info : infos)
					{
						if (found)
							break;

						gli::texture ktxTexture;

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

						ADGRVulkanCreateBufferInfo bufferInfo;
						bufferInfo.bufferSize = ktxTexture.size();
						bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
						bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
						bufferInfo.buffer = &stagingBuffer;
						bufferInfo.bufferMemory = &stagingBufferMemory;

						VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);

						void* data;
						if (vkMapMemory(logicalDevice, stagingBufferMemory, 0, static_cast<size_t>(ktxTexture.size()), 0, &data) != VK_SUCCESS)
							DMK_CORE_FATAL("Failed to map memory!");
						memcpy(data, ktxTexture.data(), static_cast<size_t>(ktxTexture.size()));
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
						cinfo.mipLevels = _container.mipLevels;
						cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
						cinfo.arrayLayers = ktxTexture.max_face() + 1;
						cinfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

						VulkanGraphicsFunctions::createImage(logicalDevice, physicalDevice, cinfo);

						std::vector<VkBufferImageCopy> bufferCopyRegions;
						for (UI32 face = 0; face < ktxTexture.max_face() + 1; face++)
						{
							for (UI32 level = 0; level < _container.mipLevels; level++)
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

						ADGRVulkanTransitionImageLayoutInfo transitionInfo;
						transitionInfo.image = _container.image;
						transitionInfo.format = _container.format;
						transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
						transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
						transitionInfo.mipLevels = info.mipLevels;
						transitionInfo.layerCount = ktxTexture.max_face() + 1;
						VulkanGraphicsFunctions::transitionImageLayout(logicalDevice, commandPool, processQueue, utilityQueue, transitionInfo);

						ADGRVulkanCopyBufferToImageInfo cpyInfo;
						cpyInfo.buffer = stagingBuffer;
						cpyInfo.image = _container.image;
						cpyInfo.destinationImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
						VulkanGraphicsFunctions::copyBufferToImageOverride(logicalDevice, commandPool, processQueue, utilityQueue, cpyInfo, bufferCopyRegions);

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
						_container.imageSampler = VulkanGraphicsFunctions::createImageSampler(logicalDevice, samplerInitInfo);

						ADGRVulkanCreateImageViewInfo cinfo2;
						cinfo2.image = _container.image;
						cinfo2.format = _container.format;
						cinfo2.mipLevels = _container.mipLevels;
						cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
						cinfo2.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
						cinfo2.layerCount = 6;
						cinfo2.component = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
						_container.imageView = VulkanGraphicsFunctions::createImageView(logicalDevice, cinfo2);

						vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
						vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
					}
				}
				else if (infos.size() == 6)
				{
					StaggingBufferContainer bufferContainer;
					UI32 width = 0;
					UI32 height = 0;

					ARRAY<std::string> paths;
					for (const ADGRVulkanTextureInitInfo& info : infos)
						paths.pushBack(info.path);

					if (paths.size() < 6)
						DMK_CORE_FATAL("Invalid skybox texture!");

					resource::TextureData texData;
					unsigned char* images[6];

					for (UI32 _itr = 0; _itr < 6; _itr++)
						images[_itr] = texData.loadTexture(paths[_itr], resource::TEXTURE_TYPE_RGBA);

					width = texData.texWidth;
					height = texData.texHeight;

					VkDeviceSize totalSize = texData.texWidth * texData.texHeight * 4 * 6;
					UI32 layerSize = totalSize / 6;

					ADGRVulkanCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = totalSize;
					bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &bufferContainer.buffer;
					bufferInfo.bufferMemory = &bufferContainer.bufferMemory;
					VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);

					_container.mipLevels = 1;
					_container.format = VK_FORMAT_R8G8B8A8_UNORM;

					void* data = nullptr;
					if (vkMapMemory(logicalDevice, bufferContainer.bufferMemory, 0, static_cast<size_t>(totalSize), 0, &data) != VK_SUCCESS)
						DMK_CORE_FATAL("Failed to map memory!");
					for (UI32 i = 0; i < 6; i++)
						memcpy((void*)((UI64(data)) + (layerSize * i)), images[i], static_cast<size_t>(layerSize));
					vkUnmapMemory(logicalDevice, bufferContainer.bufferMemory);

					ADGRVulkanCreateImageInfo cinfo;
					cinfo.width = width;
					cinfo.height = height;
					cinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
					cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
					cinfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
					cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
					cinfo.image = &_container.image;
					cinfo.imageMemory = &_container.imageMemory;
					cinfo.mipLevels = _container.mipLevels;
					cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
					cinfo.arrayLayers = 6;
					cinfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
					VulkanGraphicsFunctions::createImage(logicalDevice, physicalDevice, cinfo);

					ADGRVulkanTransitionImageLayoutInfo transitionInfo;
					transitionInfo.image = _container.image;
					transitionInfo.format = _container.format;
					transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					transitionInfo.mipLevels = 1;
					transitionInfo.layerCount = 6;
					VulkanGraphicsFunctions::transitionImageLayout(logicalDevice, commandPool, processQueue, utilityQueue, transitionInfo);

					ADGRVulkanCopyBufferToImageInfo cpyInfo;
					cpyInfo.buffer = bufferContainer.buffer;
					cpyInfo.image = _container.image;
					cpyInfo.width = static_cast<UI32>(texData.texWidth);
					cpyInfo.height = static_cast<UI32>(texData.texHeight);
					cpyInfo.baseArrayCount = 0;
					cpyInfo.layerCount = 6;
					VulkanGraphicsFunctions::copyBufferToImage(logicalDevice, commandPool, processQueue, utilityQueue, cpyInfo);

					transitionInfo.image = _container.image;
					transitionInfo.format = _container.format;
					transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					transitionInfo.mipLevels = 1;
					transitionInfo.layerCount = 6;
					VulkanGraphicsFunctions::transitionImageLayout(logicalDevice, commandPool, processQueue, utilityQueue, transitionInfo);

					ADGRVulkanTextureSamplerInitInfo samplerInitInfo;
					samplerInitInfo.magFilter = VK_FILTER_LINEAR;
					samplerInitInfo.minFilter = VK_FILTER_LINEAR;
					samplerInitInfo.mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
					samplerInitInfo.maxMipLevels = infos[0].maxMipLevels;
					samplerInitInfo.minMipLevels = infos[0].minMipLevels;
					samplerInitInfo.modeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
					samplerInitInfo.modeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
					samplerInitInfo.modeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
					samplerInitInfo.mipLoadBias = 0.0f;
					samplerInitInfo.compareOp = VK_COMPARE_OP_NEVER;
					samplerInitInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
					samplerInitInfo.maxAnisotrophy = 1.0f;
					_container.imageSampler = VulkanGraphicsFunctions::createImageSampler(logicalDevice, samplerInitInfo);

					ADGRVulkanCreateImageViewInfo cinfo2;
					cinfo2.image = _container.image;
					cinfo2.format = _container.format;
					cinfo2.mipLevels = _container.mipLevels;
					cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					cinfo2.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
					cinfo2.layerCount = 6;
					cinfo2.component = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
					_container.imageView = VulkanGraphicsFunctions::createImageView(logicalDevice, cinfo2);

					/*
					 For cube and cube array image views, the layers of the image view starting
					 at baseArrayLayer correspond to faces in the order +X, -X, +Y, -Y, +Z, -Z.
					 For cube arrays, each set of six sequential layers is a single cube, so the
					 number of cube maps in a cube map array view is layerCount / 6, and image array
					 layer (baseArrayLayer + i) is face index (i mod 6) of cube i / 6. If the number
					 of layers in the view, whether set explicitly in layerCount or implied by
					 VK_REMAINING_ARRAY_LAYERS, is not a multiple of 6, the last cube map in the
					 array must not be accessed.

					 (https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkImageSubresourceRange.html)
					*/

					vkDestroyBuffer(logicalDevice, bufferContainer.buffer, nullptr);
					vkFreeMemory(logicalDevice, bufferContainer.bufferMemory, nullptr);

					for (UI32 i = 0; i < 6; i++)
						texData.freeData(images[i]);
				}
				else if (infos.size() == 1)
				{
					ADGRVulkanTextureInitInfo info = infos[0];
					std::string _path = info.path;
					if (_path.find(".hdr") != std::string::npos)
					{
						_container.format = info.format;

						resource::TextureData texData;
						unsigned char* pixels = nullptr;

						if (_container.format == VK_FORMAT_R8G8B8A8_UNORM)
							pixels = texData.loadTexture((_path), resource::TEXTURE_TYPE_RGBA);
						else if (_container.format == VK_FORMAT_R8G8B8_UNORM)
							pixels = texData.loadTexture((_path), resource::TEXTURE_TYPE_RGB);
						else
							DMK_CORE_FATAL("Invalid texture format!");

						_container.width = texData.texWidth;
						_container.height = texData.texHeight;

						VkDeviceSize imageSize = texData.size;

						if (!pixels)
							DMK_CORE_FATAL("failed to load texture image!");

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
						memcpy(data, pixels, static_cast<size_t>(imageSize));
						vkUnmapMemory(logicalDevice, stagingBufferMemory);

						texData.freeData(pixels);

						ADGRVulkanCreateImageInfo cinfo;
						cinfo.width = texData.texWidth;
						cinfo.height = texData.texHeight;
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
						VulkanGraphicsFunctions::transitionImageLayout(logicalDevice, commandPool, processQueue, utilityQueue, transitionInfo);

						ADGRVulkanCopyBufferToImageInfo cpyInfo;
						cpyInfo.buffer = stagingBuffer;
						cpyInfo.image = _container.image;
						cpyInfo.width = static_cast<UI32>(texData.texWidth);
						cpyInfo.height = static_cast<UI32>(texData.texHeight);
						VulkanGraphicsFunctions::copyBufferToImage(logicalDevice, commandPool, processQueue, utilityQueue, cpyInfo);

						transitionInfo.image = _container.image;
						transitionInfo.format = _container.format;
						transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
						transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						transitionInfo.mipLevels = info.mipLevels;
						transitionInfo.layerCount = 1;
						VulkanGraphicsFunctions::transitionImageLayout(logicalDevice, commandPool, processQueue, utilityQueue, transitionInfo);

						vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
						vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

						//generateMipMaps(&_container);

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
					}
				}

				textureContainers.pushBack(_container);
			}
		}
	}
}