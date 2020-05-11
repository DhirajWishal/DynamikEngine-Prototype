#include "dmkafx.h"
#include "VulkanUtilities.h"

#include "GameObjectDescriptors.h"
#include "VulkanOneTimeCommandBuffer.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			VkFormat VulkanUtilities::getFormat(DMKFormat format)
			{
				VkFormat _format = VK_FORMAT_UNDEFINED;

				switch (format)
				{
				case Dynamik::DMKFormat::DMK_FORMAT_UNDEFINED:
					_format = VK_FORMAT_UNDEFINED;
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_8_INT:
					_format = VK_FORMAT_R8_UINT;
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_8_UINT:
					_format = VK_FORMAT_R8_UINT;
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_8_SINT:
					_format = VK_FORMAT_R8_SINT;
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_8_F32:
					_format = VK_FORMAT_R8_UNORM;
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_8_UF32:
					_format = VK_FORMAT_R8_UNORM;
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_8_SF32:
					_format = VK_FORMAT_R8_UNORM;
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_8_UNIFORM:
					_format = VK_FORMAT_R8_UNORM;
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_16_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_16_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_16_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_16_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_16_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_16_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_16_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_32_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_32_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_32_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_32_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_32_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_32_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_R_32_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_8_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_8_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_8_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_8_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_8_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_8_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_8_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_16_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_16_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_16_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_16_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_16_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_16_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_16_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_32_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_32_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_32_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_32_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_32_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_32_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RG_32_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_8_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_8_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_8_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_8_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_8_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_8_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_8_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_16_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_16_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_16_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_16_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_16_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_16_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_16_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_32_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_32_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_32_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_32_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_32_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_32_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGB_32_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_8_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_8_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_8_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_8_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_8_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_8_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_8_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_16_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_16_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_16_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_16_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_16_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_16_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_16_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_32_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_32_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_32_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_32_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_32_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_32_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGR_32_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_8_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_8_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_8_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_8_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_8_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_8_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_8_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_16_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_16_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_16_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_16_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_16_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_16_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_16_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_32_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_32_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_32_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_32_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_32_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_32_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_RGBA_32_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_8_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_8_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_8_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_8_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_8_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_8_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_8_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_16_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_16_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_16_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_16_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_16_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_16_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_16_UNIFORM:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_32_INT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_32_UINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_32_SINT:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_32_F32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_32_UF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_32_SF32:
					break;
				case Dynamik::DMKFormat::DMK_FORMAT_BGRA_32_UNIFORM:
					break;
				}

				return _format;
			}

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
					_description.format = vertexAttributeTypeToVkFormat(attributes[_index].dataType);
					_description.offset = _previousTypeSize;
					attributeDescriptions.pushBack(_description);

					_previousTypeSize += (UI32)attributes[_index].dataType;
				}

				return attributeDescriptions;
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

			VkShaderStageFlagBits VulkanUtilities::getShaderStage(DMKShaderLocation location)
			{
				switch (location)
				{
				case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX:
					return VK_SHADER_STAGE_VERTEX_BIT;
					break;
				case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_TESSELLATION:
					return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
					break;
				case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_GEOMETRY:
					return VK_SHADER_STAGE_GEOMETRY_BIT;
					break;
				case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT:
					return VK_SHADER_STAGE_FRAGMENT_BIT;
					break;
				case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_COMPUTE:
					return VK_SHADER_STAGE_FRAGMENT_BIT;
					break;
				default:
					DMK_CORE_FATAL("Invalid shader location!");
					break;
				}

				return VK_SHADER_STAGE_ALL;
			}

			ARRAY<VulkanGraphicsShader> VulkanUtilities::getGraphicsShaders(VkDevice logicalDevice, ShaderPaths shaderPaths)
			{
				DMK_BEGIN_PROFILE_TIMER();

				ARRAY<VulkanGraphicsShader> _shaders;

				if (shaderPaths.vertexShader.size() && shaderPaths.vertexShader != "NONE")
				{
					VulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = shaderPaths.vertexShader;
					_initInfo.type = VulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (shaderPaths.tessellationShader.size() && shaderPaths.tessellationShader != "NONE")
				{
					VulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = shaderPaths.tessellationShader;
					_initInfo.type = VulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (shaderPaths.geometryShader.size() && shaderPaths.geometryShader != "NONE")
				{
					VulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = shaderPaths.geometryShader;
					_initInfo.type = VulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (shaderPaths.fragmentShader.size() && shaderPaths.fragmentShader != "NONE")
				{
					VulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = shaderPaths.fragmentShader;
					_initInfo.type = VulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

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
				POINTER<VulkanTextureContainer> container)
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

			VulkanTextureContainer VulkanUtilities::createTexture(VulkanUtilitiesTextureInitInfo info, Texture texture)
			{
				DMK_BEGIN_PROFILE_TIMER();

				VulkanTextureContainer _container;

				_container.mipLevels = info.mipLevels;

				if (texture.format == DMKFormat::DMK_FORMAT_RGBA_8_UNIFORM)
					_container.format = VK_FORMAT_R8G8B8A8_UNORM;
				else if (texture.format == DMKFormat::DMK_FORMAT_RGB_8_UNIFORM)
					_container.format = VK_FORMAT_R8G8B8A8_UNORM;

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

				VulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = imageSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanUtilities::createBuffer(info.logicalDevice, info.physicalDevice, bufferInfo);

				void* data;
				if (vkMapMemory(info.logicalDevice, stagingBufferMemory, 0, static_cast<size_t>(imageSize), 0, &data) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to map memory!");
				memcpy(data, texture.textureData.get(), static_cast<size_t>(imageSize));
				vkUnmapMemory(info.logicalDevice, stagingBufferMemory);

				VulkanCreateImageInfo cinfo;
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
				cinfo.arrayLayers = arrayLayers;
				VulkanUtilities::createImage(info.logicalDevice, info.physicalDevice, cinfo);

				VulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = _container.image;
				transitionInfo.format = _container.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.mipLevels = info.mipLevels;
				transitionInfo.layerCount = arrayLayers;
				VulkanUtilities::transitionImageLayout(info.logicalDevice, info.commandPool, info.processQueue, info.utilityQueue, transitionInfo);

				VulkanCopyBufferToImageInfo cpyInfo;
				cpyInfo.buffer = stagingBuffer;
				cpyInfo.image = _container.image;
				cpyInfo.width = _container.width;
				cpyInfo.height = _container.height;
				cpyInfo.baseArrayCount = 0;
				cpyInfo.layerCount = arrayLayers;
				VulkanUtilities::copyBufferToImage(info.logicalDevice, info.commandPool, info.processQueue, info.utilityQueue, cpyInfo);

				transitionInfo.image = _container.image;
				transitionInfo.format = _container.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				transitionInfo.mipLevels = info.mipLevels;
				transitionInfo.layerCount = arrayLayers;
				VulkanUtilities::transitionImageLayout(info.logicalDevice, info.commandPool, info.processQueue, info.utilityQueue, transitionInfo);

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

				VulkanTextureSamplerInitInfo samplerInitInfo;
				samplerInitInfo.magFilter = info.magFilter;
				samplerInitInfo.minFilter = info.minFilter;
				samplerInitInfo.maxMipLevels = info.maxMipLevels;
				samplerInitInfo.minMipLevels = info.minMipLevels;
				samplerInitInfo.modeU = info.modeU;
				samplerInitInfo.modeV = info.modeV;
				samplerInitInfo.modeW = info.modeW;
				samplerInitInfo.mipLoadBias = 0.0f;
				samplerInitInfo.mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				samplerInitInfo.compareOp = VK_COMPARE_OP_NEVER;
				samplerInitInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
				samplerInitInfo.maxAnisotrophy = 1.0f;
				_container.imageSampler = VulkanUtilities::createImageSampler(info.logicalDevice, samplerInitInfo);

				VulkanCreateImageViewInfo cinfo2;
				cinfo2.image = _container.image;
				cinfo2.format = _container.format;
				cinfo2.mipLevels = _container.mipLevels;
				cinfo2.aspectFlags = info.aspectFlags;
				cinfo2.viewType = imageViewType;
				cinfo2.layerCount = arrayLayers;
				if (texture.type == DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP)
					cinfo2.component = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
				_container.imageView = VulkanUtilities::createImageView(info.logicalDevice, cinfo2);

				return _container;
			}

			void VulkanUtilities::createBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VulkanCreateBufferInfo info)
			{
				VkBufferCreateInfo bufferInfo = {};
				bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferInfo.size = info.bufferSize;
				bufferInfo.usage = info.usage;
				bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, info.buffer) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create buffer!");

				VkMemoryRequirements memRequirements;
				vkGetBufferMemoryRequirements(logicalDevice, *info.buffer, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, info.bufferMemoryPropertyflags,
					physicalDevice);

				if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, info.bufferMemory) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate buffer memory!");

				vkBindBufferMemory(logicalDevice, *info.buffer, *info.bufferMemory, 0);
			}

			UI32 VulkanUtilities::findMemoryType(UI32 typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice)
			{
				VkPhysicalDeviceMemoryProperties memProperties;
				vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

				for (UI32 i = 0; i < memProperties.memoryTypeCount; i++)
					if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags
						& properties) == properties)
						return i;

				DMK_CORE_FATAL("failed to find suitable memory type!");
				return 0;
			}

			VkSampleCountFlagBits VulkanUtilities::getMaxUsableSampleCount(VkPhysicalDevice physicalDevice)
			{
				VkPhysicalDeviceProperties physicalDeviceProperties;
				vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

				VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts,
					physicalDeviceProperties.limits.framebufferDepthSampleCounts);
				if (counts & VK_SAMPLE_COUNT_64_BIT)  return VK_SAMPLE_COUNT_64_BIT;
				if (counts & VK_SAMPLE_COUNT_32_BIT)  return VK_SAMPLE_COUNT_32_BIT;
				if (counts & VK_SAMPLE_COUNT_16_BIT)  return VK_SAMPLE_COUNT_16_BIT;
				if (counts & VK_SAMPLE_COUNT_8_BIT)  return VK_SAMPLE_COUNT_8_BIT;
				if (counts & VK_SAMPLE_COUNT_4_BIT)  return VK_SAMPLE_COUNT_4_BIT;
				if (counts & VK_SAMPLE_COUNT_2_BIT)  return VK_SAMPLE_COUNT_2_BIT;

				return VK_SAMPLE_COUNT_1_BIT;
			}

			B1 VulkanUtilities::hasStencilComponent(VkFormat format)
			{
				return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
			}

			VkFormat VulkanUtilities::findSupportedFormat(const ARRAY<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice)
			{
				for (VkFormat format : candidates) {
					VkFormatProperties props;

					vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

					if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
						return format;

					else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
						return format;
				}

				DMK_CORE_FATAL("failed to find supported format!");
				return VkFormat();
			}

			VkFormat VulkanUtilities::findDepthFormat(VkPhysicalDevice physicalDevice)
			{
				return findSupportedFormat(
					{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
					VK_IMAGE_TILING_OPTIMAL,
					VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
					physicalDevice
				);
			}

			void VulkanUtilities::copyBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
			{
				VulkanOneTimeCommandBuffer oneTimeCommandBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue);
				VkCommandBuffer commandBuffer = oneTimeCommandBuffer.buffer;

				VkBufferCopy copyRegion = {};
				copyRegion.size = size;
				vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
			}

			void VulkanUtilities::createImage(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VulkanCreateImageInfo info)
			{
				VkImageCreateInfo imageInfo = {};
				imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				imageInfo.imageType = VK_IMAGE_TYPE_2D;
				imageInfo.extent.width = info.width;
				imageInfo.extent.height = info.height;
				imageInfo.extent.depth = 1;
				imageInfo.mipLevels = info.mipLevels;
				imageInfo.arrayLayers = info.arrayLayers;
				imageInfo.format = info.format;
				imageInfo.initialLayout = info.initialLayout;
				imageInfo.usage = info.usage;
				imageInfo.samples = info.numSamples;
				imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				if (info.flags != NULL)
					imageInfo.flags = info.flags;

				if (vkCreateImage(logicalDevice, &imageInfo, nullptr, info.image) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create image!");

				VkMemoryRequirements memRequirements;
				vkGetImageMemoryRequirements(logicalDevice, *info.image, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, physicalDevice);

				if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, info.imageMemory) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate image memory!");

				vkBindImageMemory(logicalDevice, *info.image, *info.imageMemory, 0);
			}

			void VulkanUtilities::transitionImageLayout(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue, VulkanTransitionImageLayoutInfo info)
			{
				VulkanOneTimeCommandBuffer oneTimeCommandBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue);
				VkCommandBuffer commandBuffer = oneTimeCommandBuffer.buffer;

				VkImageMemoryBarrier barrier = {};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.oldLayout = info.oldLayout;
				barrier.newLayout = info.newLayout;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.image = info.image;

				if (info.newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
					barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

					if (hasStencilComponent(info.format)) {
						barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
					}
				}
				else {
					barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				}

				barrier.subresourceRange.baseMipLevel = 0;
				barrier.subresourceRange.levelCount = info.mipLevels;
				barrier.subresourceRange.baseArrayLayer = info.arrayLayers;
				barrier.subresourceRange.layerCount = info.layerCount;
				barrier.srcAccessMask = 0; // TODO
				barrier.dstAccessMask = 0; // TODO

				VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
				VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

				switch (info.oldLayout)
				{
				case VK_IMAGE_LAYOUT_UNDEFINED:
					sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
					barrier.srcAccessMask = 0;
					break;

				case VK_IMAGE_LAYOUT_PREINITIALIZED:
					barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
					barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
					barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					break;

				case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
					sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
					destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
					barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
					break;
				default:
					DMK_CORE_FATAL("unsupported layout transition!");
					break;
				}

				switch (info.newLayout)
				{
				case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
					destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					break;

				case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
					destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
					destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
					barrier.dstAccessMask = barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
					if (barrier.srcAccessMask == 0)
					{
						barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
					}
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
					destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
					break;

				case VK_IMAGE_LAYOUT_GENERAL:
					destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
					break;
				default:
					DMK_CORE_FATAL("unsupported layout transition!");
					break;
				}

				vkCmdPipelineBarrier(
					commandBuffer,
					sourceStage, destinationStage,
					0,
					0, nullptr,
					0, nullptr,
					1, &barrier
				);
			}

			void VulkanUtilities::transitionImageLayout(VkDevice logicalDevice, VkCommandBuffer commandBuffer, VulkanTransitionImageLayoutInfo info)
			{
				VkImageMemoryBarrier barrier = {};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.oldLayout = info.oldLayout;
				barrier.newLayout = info.newLayout;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.image = info.image;

				if (info.newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
					barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

					if (hasStencilComponent(info.format)) {
						barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
					}
				}
				else {
					barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				}

				barrier.subresourceRange.baseMipLevel = 0;
				barrier.subresourceRange.levelCount = info.mipLevels;
				barrier.subresourceRange.baseArrayLayer = info.arrayLayers;
				barrier.subresourceRange.layerCount = info.layerCount;
				barrier.srcAccessMask = 0; // TODO
				barrier.dstAccessMask = 0; // TODO

				VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
				VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

				switch (info.oldLayout)
				{
				case VK_IMAGE_LAYOUT_UNDEFINED:
					sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
					barrier.srcAccessMask = 0;
					break;

				case VK_IMAGE_LAYOUT_PREINITIALIZED:
					barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
					barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
					barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					break;

				case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
					sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
					destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
					barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
					break;
				default:
					DMK_CORE_FATAL("unsupported layout transition!");
					break;
				}

				switch (info.newLayout)
				{
				case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
					destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					break;

				case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
					destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
					destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
					barrier.dstAccessMask = barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
					if (barrier.srcAccessMask == 0)
					{
						barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
					}
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
					destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
					break;

				case VK_IMAGE_LAYOUT_GENERAL:
					destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
					break;
				default:
					DMK_CORE_FATAL("unsupported layout transition!");
					break;
				}

				vkCmdPipelineBarrier(
					commandBuffer,
					sourceStage, destinationStage,
					0,
					0, nullptr,
					0, nullptr,
					1, &barrier
				);
			}

			VkImageView VulkanUtilities::createImageView(VkDevice device, VulkanCreateImageViewInfo info)
			{
				VkImageViewCreateInfo viewInfo = {};
				viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				viewInfo.image = info.image;
				viewInfo.viewType = info.viewType;
				viewInfo.format = info.format;
				viewInfo.subresourceRange.aspectMask = info.aspectFlags;
				viewInfo.subresourceRange.baseMipLevel = 0;
				viewInfo.subresourceRange.levelCount = info.mipLevels;
				viewInfo.subresourceRange.baseArrayLayer = 0;
				viewInfo.subresourceRange.layerCount = info.layerCount;
				viewInfo.components = info.component;

				VkImageView imageView;
				if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create texture image view!");

				return imageView;
			}

			void VulkanUtilities::copyBufferToImage(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue, VulkanCopyBufferToImageInfo info)
			{
				VulkanOneTimeCommandBuffer oneTimeCommandBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue);
				VkCommandBuffer commandBuffer = oneTimeCommandBuffer.buffer;

				copyBufferToImage(logicalDevice, commandBuffer, info);
			}

			void VulkanUtilities::copyBufferToImage(VkDevice logicalDevice, VkCommandBuffer commandBuffer, VulkanCopyBufferToImageInfo info)
			{
				VkBufferImageCopy region = {};
				region.bufferOffset = 0;
				region.bufferRowLength = 0;
				region.bufferImageHeight = 0;

				region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				region.imageSubresource.mipLevel = 0;
				region.imageSubresource.baseArrayLayer = info.baseArrayCount;
				region.imageSubresource.layerCount = info.layerCount;

				region.imageOffset = { 0, 0, 0 };
				region.imageExtent = {
					info.width,
					info.height,
					1
				};

				vkCmdCopyBufferToImage(
					commandBuffer,
					info.buffer,
					info.image,
					info.destinationImageLayout,
					1,
					&region
				);
			}

			void VulkanUtilities::copyBufferToImageOverride(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue, VulkanCopyBufferToImageInfo info, ARRAY<VkBufferImageCopy> copyRegions)
			{
				VulkanOneTimeCommandBuffer oneTimeCommandBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue);
				VkCommandBuffer commandBuffer = oneTimeCommandBuffer.buffer;

				copyBufferToImageOverride(logicalDevice, commandBuffer, info, copyRegions);
			}

			void VulkanUtilities::copyBufferToImageOverride(VkDevice logicalDevice, VkCommandBuffer commandBuffer, VulkanCopyBufferToImageInfo info, ARRAY<VkBufferImageCopy> copyRegions)
			{
				vkCmdCopyBufferToImage(
					commandBuffer,
					info.buffer,
					info.image,
					info.destinationImageLayout,
					copyRegions.size(),
					copyRegions.data());
			}

			VkSampler VulkanUtilities::createImageSampler(VkDevice logicalDevice, VulkanTextureSamplerInitInfo info)
			{
				VkSamplerCreateInfo samplerInfo = {};
				samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				samplerInfo.magFilter = info.magFilter;
				samplerInfo.minFilter = info.minFilter;
				samplerInfo.addressModeU = info.modeU;
				samplerInfo.addressModeV = info.modeV;
				samplerInfo.addressModeW = info.modeW;
				samplerInfo.anisotropyEnable = info.anisotrophyEnable;
				samplerInfo.maxAnisotropy = info.maxAnisotrophy;
				samplerInfo.borderColor = info.borderColor;
				samplerInfo.unnormalizedCoordinates = info.unnormalizedCoordinates;
				samplerInfo.compareEnable = info.compareEnable;
				samplerInfo.compareOp = info.compareOp;
				samplerInfo.mipmapMode = info.mipMapMode;
				samplerInfo.minLod = info.minMipLevels; // change this for varying mip levels
				samplerInfo.maxLod = info.maxMipLevels;
				samplerInfo.mipLodBias = info.mipLoadBias; // Optional

				VkSampler _sampler = VK_NULL_HANDLE;
				if (vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &_sampler) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create texture sampler!");

				return _sampler;
			}

			VulkanUnformBufferContainer VulkanUtilities::createUniformBuffers(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, UI32 count)
			{
				VulkanUnformBufferContainer _container;

				_container.buffers.resize(count);
				_container.bufferMemories.resize(count);

				for (size_t i = 0; i < count; i++)
				{
					VulkanCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = bufferSize;
					bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &_container.buffers[i];
					bufferInfo.bufferMemory = &_container.bufferMemories[i];

					VulkanUtilities::createBuffer(logicalDevice, physicalDevice, bufferInfo);
				}

				return _container;
			}

			struct UBO_MVP {
				MAT4 model = glm::mat4(1.0f);
				MAT4 view = glm::mat4(1.0f);
				MAT4 proj = glm::mat4(1.0f);
			};

			void VulkanUtilities::updateUniformBuffer(VkDevice device, DMKUniformBufferData uniformData, VkDeviceMemory uniformBufferMemory)
			{
				DMK_BEGIN_PROFILE_TIMER();

				VPTR data = nullptr;
				vkMapMemory(device, uniformBufferMemory, 0, uniformData.size(), 0, &data);
				memcpy(data, uniformData.data(), uniformData.size());
				vkUnmapMemory(device, uniformBufferMemory);
			}

			VkDescriptorType VulkanUtilities::getDescriptorType(DMKUniformType type)
			{
				VkDescriptorType _descriptorType;

				switch (type)
				{
				case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT:
					_descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					break;

				case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_2D:
					_descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					break;

				case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_3D:
					_descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					break;

				case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_CUBEMAP:
					_descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					break;

				case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_CONSTANT:
					break;
				}

				return _descriptorType;
			}

			VkShaderStageFlagBits VulkanUtilities::getDescriptorFlag(DMKShaderLocation location)
			{
				VkShaderStageFlagBits _flag;

				switch (location)
				{
				case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX:
					_flag = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
					break;

				case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_TESSELLATION:
					_flag = VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
					break;

				case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_GEOMETRY:
					_flag = VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
					break;

				case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT:
					_flag = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
					break;

				case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_COMPUTE:
					_flag = VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
					break;
				}

				return _flag;
			}

			ARRAY<VkDescriptorSetLayoutBinding> VulkanUtilities::getDescriptorSetBindings(ARRAY<DMKUniformBufferObjectDescriptor> descriptors)
			{
				ARRAY<VkDescriptorSetLayoutBinding> bindings;

				for (UI32 binding = 0; binding < descriptors.size(); binding++)
				{
					VkDescriptorSetLayoutBinding uboLayoutBinding = {};
					uboLayoutBinding.binding = descriptors[binding].binding; // info.bindIndex;
					uboLayoutBinding.descriptorCount = 1;
					uboLayoutBinding.descriptorType = getDescriptorType(descriptors[binding].type);
					uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
					uboLayoutBinding.stageFlags = getDescriptorFlag(descriptors[binding].location);
					bindings.pushBack(uboLayoutBinding);
				}

				return bindings;
			}

			ARRAY<VkDescriptorPoolSize> VulkanUtilities::getPoolSizes(ARRAY<DMKUniformBufferObjectDescriptor> descriptors, UI32 uniformBufferCount, UI32 textureImageCount)
			{
				ARRAY<VkDescriptorPoolSize> poolSizes = {};

				for (auto _description : descriptors)
				{
					VkDescriptorPoolSize pool;
					switch (_description.type)
					{
					case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT:
						pool.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
						pool.descriptorCount = uniformBufferCount;
						poolSizes.pushBack(pool);
						break;

					case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_2D:
						pool.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						pool.descriptorCount = textureImageCount;
						poolSizes.pushBack(pool);
						break;

					case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_3D:
						pool.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						pool.descriptorCount = textureImageCount;
						poolSizes.pushBack(pool);
						break;

					case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_CUBEMAP:
						pool.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						pool.descriptorCount = textureImageCount;
						poolSizes.pushBack(pool);
						break;

					case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_CONSTANT:
						break;
					}
				}

				return poolSizes;
			}
		}
	}
}