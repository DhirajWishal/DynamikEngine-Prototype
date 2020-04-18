#include "adgrafx.h"
#include "VulkanGraphicsTextureAttachment.h"

#include "../VulkanGraphicsOneTimeCommandBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanGraphicsTextureAttachment::terminateTextures()
			{
				for (ADGRVulkanTextureContainer _container : textureContainers)
				{
					vkDestroySampler(logicalDevice, _container.imageSampler, nullptr);
					vkDestroyImageView(logicalDevice, _container.imageView, nullptr);

					vkDestroyImage(logicalDevice, _container.image, nullptr);
					vkFreeMemory(logicalDevice, _container.imageMemory, nullptr);
				}
			}

			void VulkanGraphicsTextureAttachment::generateMipMaps(POINTER<ADGRVulkanTextureContainer> container)
			{
				UI32 _width = container->width, _height = container->height;

				VkFormatProperties formatProperties;
				vkGetPhysicalDeviceFormatProperties(physicalDevice, container->format, &formatProperties);

				if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
					DMK_CORE_FATAL("texture image format does not support linear blitting!");

				VulkanGraphicsOneTimeCommandBuffer oneTimeCommandBuffer(logicalDevice, commandPool, processQueue, utilityQueue);
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

			void VulkanGraphicsTextureAttachment::initializeDescriptor(UI32 destination, VkShaderStageFlags stageFlag)
			{
				/* Initialize Descriptor Layout */
				_initializeDescriptorLayout(destination, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, stageFlag);

				/* Initialize Descriptor Pool */
				_initializeDescriptorPool();

				/* Initialize Descriptor Sets */
				_initializeDescriptorSets(destination);
			}
			
			void VulkanGraphicsTextureAttachment::_initializeDescriptorPool()
			{
				VkDescriptorPoolSize _poolSize1;
				_poolSize1.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				_poolSize1.descriptorCount = 1;

				ADGRVulkanDescriptorPoolInitInfo poolInitInfo;
				poolInitInfo.maxDescriptorSets = 1;
				poolInitInfo.poolSizes = { _poolSize1 };
				descriptor.initializePool(logicalDevice, poolInitInfo);
			}

			void VulkanGraphicsTextureAttachment::_initializeDescriptorSets(UI32 destination)
			{
				ARRAY<VkDescriptorImageInfo> infos;
				for (ADGRVulkanTextureContainer _container : textureContainers)
				{
					VkDescriptorImageInfo imageInfo = {};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = _container.imageView;
					imageInfo.sampler = _container.imageSampler;
					infos.pushBack(imageInfo);
				}

				VkWriteDescriptorSet _writes2;
				_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes2.dstBinding = destination;
				_writes2.dstArrayElement = 0;
				_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				_writes2.descriptorCount = 1;
				_writes2.pImageInfo = infos.data();
				_writes2.pNext = VK_NULL_HANDLE;
				_writes2.pTexelBufferView = VK_NULL_HANDLE;
				_writes2.pBufferInfo = VK_NULL_HANDLE;

				ADGRVulkanDescriptorSetsInitInfo setInitInfo;
				setInitInfo.descriptorWrites = { _writes2 };
				setInitInfo.setCount = 1;
				descriptor.initializeSets(logicalDevice, setInitInfo);
			}
		}
	}
}