#include "adgrafx.h"
#include "VulkanTextureManager.h"

#include "VulkanFunctions.h"
#include "VulkanOneTimeCommandBufferManager.h"

#include "Platform/Windows/resource/imageLoader.h"
using namespace Dynamik::ADGR::core;

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanTextureManager::initialize(
				POINTER<VulkanCoreObject> core,
				ADGRVulkanTextureInitInfo info,
				POINTER<VulkanRenderObject> object)
			{
				ADGRVulkanTextureContainer _container;
				_container.format = info.format;
				_container.mipLevels = info.mipLevels;

				resource::TextureData texData;
				unsigned char* pixels = nullptr;

				if (_container.format == VK_FORMAT_R8G8B8A8_UNORM)
					pixels = texData.loadTexture((info.path), resource::TEXTURE_TYPE_RGBA);
				else if (_container.format == VK_FORMAT_R8G8B8_UNORM)
					pixels = texData.loadTexture((info.path), resource::TEXTURE_TYPE_RGB);
				else
					DMK_CORE_FATAL("Invalid texture format!");

				_container.width = texData.texWidth;
				_container.height = texData.texHeight;

				VkDeviceSize imageSize = texData.size;

				if (!pixels)
					DMK_CORE_FATAL("failed to load texture image!");

				VkBuffer stagingBuffer = VK_NULL_HANDLE;
				VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

				ADGRCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = imageSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanFunctions::createBuffer(core, bufferInfo);

				void* data;
				if (vkMapMemory(core->logicalDevice, stagingBufferMemory, 0, static_cast<size_t>(imageSize), 0, &data) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to map memory!");
				memcpy(data, pixels, static_cast<size_t>(imageSize));
				vkUnmapMemory(core->logicalDevice, stagingBufferMemory);

				texData.freeData(pixels);

				ADGRCreateImageInfo cinfo;
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

				VulkanFunctions::createImage(core, cinfo);

				ADGRTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = _container.image;
				transitionInfo.format = _container.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.mipLevels = info.mipLevels;
				transitionInfo.layerCount = 1;

				VulkanFunctions::transitionImageLayout(core, transitionInfo);

				ADGRCopyBufferToImageInfo cpyInfo;
				cpyInfo.buffer = stagingBuffer;
				cpyInfo.image = _container.image;
				cpyInfo.width = static_cast<UI32>(texData.texWidth);
				cpyInfo.height = static_cast<UI32>(texData.texHeight);

				VulkanFunctions::copyBufferToImage(core, cpyInfo);

				vkDestroyBuffer(core->logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(core->logicalDevice, stagingBufferMemory, nullptr);

				generateMipMaps(core, &_container);

				VkSamplerCreateInfo samplerInfo = {};
				samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				samplerInfo.magFilter = info.magFilter;
				samplerInfo.minFilter = info.minFilter;
				samplerInfo.addressModeU = info.modeU;
				samplerInfo.addressModeV = info.modeV;
				samplerInfo.addressModeW = info.modeW;
				samplerInfo.anisotropyEnable = VK_TRUE;
				samplerInfo.maxAnisotropy = 16;
				samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
				samplerInfo.unnormalizedCoordinates = VK_FALSE;
				samplerInfo.compareEnable = VK_FALSE;
				samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
				samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				samplerInfo.minLod = core->minMipLevel; // change this for varying mip levels
				samplerInfo.maxLod = static_cast<F32>(core->maxMipLevel);
				samplerInfo.mipLodBias = 0; // Optional

				if (vkCreateSampler(core->logicalDevice, &samplerInfo, nullptr, &_container.imageSampler) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create texture sampler!");

				ADGRCreateImageViewInfo cinfo2;
				cinfo2.image = _container.image;
				cinfo2.format = _container.format;
				cinfo2.mipLevels = _container.mipLevels;
				cinfo2.aspectFlags = info.aspectFlags;

				_container.imageView = VulkanFunctions::createImageView(core, cinfo2);

				object->textures.pushBack(_container);
			}

			void VulkanTextureManager::terminate(POINTER<VulkanCoreObject> core, ADGRVulkanTextureContainer container)
			{
				vkDestroySampler(core->logicalDevice, container.imageSampler, nullptr);
				vkDestroyImageView(core->logicalDevice, container.imageView, nullptr);

				vkDestroyImage(core->logicalDevice, container.image, nullptr);
				vkFreeMemory(core->logicalDevice, container.imageMemory, nullptr);
			}

			void VulkanTextureManager::generateMipMaps(POINTER<VulkanCoreObject> core, POINTER<ADGRVulkanTextureContainer> container)
			{
				UI32 _width = container->width, _height = container->height;

				VkFormatProperties formatProperties;
				vkGetPhysicalDeviceFormatProperties(core->physicalDevice, container->format, &formatProperties);

				if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
					DMK_CORE_FATAL("texture image format does not support linear blitting!");

				VulkanOneTimeCommandBufferManager oneTimeCommandBuffer(*core);
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
		}
	}
}