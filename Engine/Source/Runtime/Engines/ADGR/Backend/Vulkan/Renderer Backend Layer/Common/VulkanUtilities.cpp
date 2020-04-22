#include "dmkafx.h"
#include "VulkanUtilities.h"

#include "GameObjectDescriptors.h"

#include "VulkanOneTimeCommandBuffer.h"
#include "../Graphics/VulkanGraphicsFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			ARRAY<VkVertexInputBindingDescription> VulkanUtilities::getBindingDescription(ARRAY<DMKVertexAttribute> attributes, UI32 bindCount)
			{
				ARRAY<VkVertexInputBindingDescription> bindingDescription(bindCount);

				for (int i = 0; i < bindCount; i++) {
					bindingDescription[i].binding = i;
					bindingDescription[i].stride = DMKVertexBufferObjectDescriptor::vertexByteSize(attributes);
					bindingDescription[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				}

				return bindingDescription;
			}

			ARRAY<VkVertexInputAttributeDescription> VulkanUtilities::getAttributeDescriptions(ARRAY<DMKVertexAttribute> attributes, UI32 binding)
			{
				ARRAY<VkVertexInputAttributeDescription> attributeDescriptions;
				UI32 _previousTypeSize = 0;

				for (UI32 _index = 0; _index < attributes.size(); _index++)
				{
					VkVertexInputAttributeDescription _description = {};
					_description.binding = binding;
					_description.location = _index;
					_description.format = vertexAttributeTypeToVkFormat(attributes[_index].type);
					_description.offset = _previousTypeSize;
					attributeDescriptions.pushBack(_description);

					_previousTypeSize += (UI32)attributes[_index].type;
				}

				return attributeDescriptions;
			}

			void VulkanUtilities::updateUniformBuffer(VkDevice device, ARRAY<MAT4> uniformData, VkDeviceMemory uniformBufferMemory)
			{
				UI32 _bufferSize = uniformData.typeSize() * uniformData.size();

				VPTR data = nullptr;
				vkMapMemory(device, uniformBufferMemory, 0, _bufferSize, 0, &data);
				memcpy(data, uniformData.data(), _bufferSize);
				vkUnmapMemory(device, uniformBufferMemory);
			}

			VkFormat VulkanUtilities::vertexAttributeTypeToVkFormat(DMKDataType type)
			{
				const UI32 _baseByteSize = sizeof(F32);

				switch ((UI32)type)
				{
				case (_baseByteSize * 1):
					return VkFormat::VK_FORMAT_R32_SFLOAT;
					break;

				case (_baseByteSize * 2):
					return VkFormat::VK_FORMAT_R32G32_SFLOAT;
					break;

				case (_baseByteSize * 3):
					return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
					break;

				case (_baseByteSize * 4):
					return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
					break;

				case (_baseByteSize * 9):
					return VkFormat::VK_FORMAT_R32_SFLOAT;
					break;

				case (_baseByteSize * 16):
					return VkFormat::VK_FORMAT_R32_SFLOAT;
					break;
				}

				return VkFormat::VK_FORMAT_UNDEFINED;
			}

			ARRAY<VulkanGraphicsShader> VulkanUtilities::getGraphicsShaders(VkDevice logicalDevice, POINTER<InternalFormat> internalFormat)
			{
				ARRAY<VulkanGraphicsShader> _shaders;

				if (internalFormat->shaderPaths.vertexShader.size() && internalFormat->shaderPaths.vertexShader != "NONE")
				{
					ADGRVulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = internalFormat->shaderPaths.vertexShader;
					_initInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (internalFormat->shaderPaths.tessellationShader.size() && internalFormat->shaderPaths.tessellationShader != "NONE")
				{
					ADGRVulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = internalFormat->shaderPaths.tessellationShader;
					_initInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (internalFormat->shaderPaths.geometryShader.size() && internalFormat->shaderPaths.geometryShader != "NONE")
				{
					ADGRVulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = internalFormat->shaderPaths.geometryShader;
					_initInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (internalFormat->shaderPaths.fragmentShader.size() && internalFormat->shaderPaths.fragmentShader != "NONE")
				{
					ADGRVulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = internalFormat->shaderPaths.fragmentShader;
					_initInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}

				return _shaders;
			}

			void VulkanUtilities::terminateGraphicsShaders(VkDevice logicalDevice, ARRAY<VulkanGraphicsShader> shaders)
			{
				for (auto shader : shaders)
					shader.terminate(logicalDevice);
			}

			void VulkanUtilities::generateMipMaps(
				VkDevice logicalDevice,
				VkPhysicalDevice physicalDevice,
				VkCommandPool commandPool,
				VkQueue processQueue,
				VkQueue utilityQueue,
				POINTER<ADGRVulkanTextureContainer> container)
			{
				UI32 _width = container->width, _height = container->height;

				VkFormatProperties formatProperties;
				vkGetPhysicalDeviceFormatProperties(physicalDevice, container->format, &formatProperties);

				if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
					DMK_CORE_FATAL("texture image format does not support linear blitting!");

				VulkanOneTimeCommandBuffer oneTimeCommandBuffer(logicalDevice, commandPool, processQueue, utilityQueue);
				VkCommandBuffer _commandBuffer = oneTimeCommandBuffer.buffer;

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

			ADGRVulkanTextureContainer VulkanUtilities::createTexture(ADGRVulkanUtilitiesTextureInitInfo info, Texture texture)
			{
				ADGRVulkanTextureContainer _container;

				_container.mipLevels = info.mipLevels;

				if (texture.format == DMKFormat::DMK_FORMAT_RGBA_8_UNIFORM)
					_container.format = VK_FORMAT_R8G8B8A8_UNORM;
				else if (texture.format == DMKFormat::DMK_FORMAT_RGB_8_UNIFORM)
					_container.format = VK_FORMAT_R8G8B8_UNORM;

				_container.width = texture.width;
				_container.height = texture.height;

				/* Changes for cubemap */
				VkImageCreateFlags createImageFlags = NULL;
				VkImageViewType imageViewType = VK_IMAGE_VIEW_TYPE_2D;
				UI32 arrayLayers = 1;
				VkDeviceSize layerSize = texture.size() / 6;	/* For cubemap */

				if (texture.type == DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP)
				{
					createImageFlags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
					imageViewType = VK_IMAGE_VIEW_TYPE_CUBE;
					arrayLayers = 6;
				}

				VkDeviceSize imageSize = texture.size();

				VkBuffer stagingBuffer = VK_NULL_HANDLE;
				VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

				ADGRVulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = imageSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanGraphicsFunctions::createBuffer(info.logicalDevice, info.physicalDevice, bufferInfo);

				void* data;
				if (vkMapMemory(info.logicalDevice, stagingBufferMemory, 0, static_cast<size_t>(imageSize), 0, &data) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to map memory!");

				if (texture.type == DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP)
				{
					for (UI32 i = 0; i < 6; i++)
						memcpy((void*)((UI64(data)) + (layerSize * i)), texture.textureData[i], static_cast<size_t>(layerSize));
				}
				else
					memcpy(data, texture.textureData, static_cast<size_t>(imageSize));

				vkUnmapMemory(info.logicalDevice, stagingBufferMemory);

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
				cinfo.flags = createImageFlags;

				VulkanGraphicsFunctions::createImage(info.logicalDevice, info.physicalDevice, cinfo);

				ADGRVulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = _container.image;
				transitionInfo.format = _container.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.mipLevels = info.mipLevels;
				transitionInfo.layerCount = arrayLayers;
				VulkanGraphicsFunctions::transitionImageLayout(info.logicalDevice, info.commandPool, info.processQueue, info.utilityQueue, transitionInfo);

				ADGRVulkanCopyBufferToImageInfo cpyInfo;
				cpyInfo.buffer = stagingBuffer;
				cpyInfo.image = _container.image;
				cpyInfo.width = _container.width;
				cpyInfo.height = _container.height;
				VulkanGraphicsFunctions::copyBufferToImage(info.logicalDevice, info.commandPool, info.processQueue, info.utilityQueue, cpyInfo);

				transitionInfo.image = _container.image;
				transitionInfo.format = _container.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				transitionInfo.mipLevels = info.mipLevels;
				transitionInfo.layerCount = arrayLayers;
				VulkanGraphicsFunctions::transitionImageLayout(info.logicalDevice, info.commandPool, info.processQueue, info.utilityQueue, transitionInfo);

				vkDestroyBuffer(info.logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(info.logicalDevice, stagingBufferMemory, nullptr);

				if (texture.type != DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP)
					generateMipMaps(
						info.logicalDevice,
						info.physicalDevice,
						info.commandPool,
						info.processQueue,
						info.utilityQueue,
						&_container);

				ADGRVulkanTextureSamplerInitInfo samplerInitInfo;
				samplerInitInfo.magFilter = info.magFilter;
				samplerInitInfo.minFilter = info.minFilter;
				samplerInitInfo.maxMipLevels = info.maxMipLevels;
				samplerInitInfo.minMipLevels = info.minMipLevels;
				samplerInitInfo.modeU = info.modeU;
				samplerInitInfo.modeV = info.modeV;
				samplerInitInfo.modeW = info.modeW;
				_container.imageSampler = VulkanGraphicsFunctions::createImageSampler(info.logicalDevice, samplerInitInfo);

				ADGRVulkanCreateImageViewInfo cinfo2;
				cinfo2.image = _container.image;
				cinfo2.format = _container.format;
				cinfo2.mipLevels = _container.mipLevels;
				cinfo2.aspectFlags = info.aspectFlags;
				cinfo2.viewType = imageViewType;
				_container.imageView = VulkanGraphicsFunctions::createImageView(info.logicalDevice, cinfo2);

				return _container;
			}
		}
	}
}