#include "dmkafx.h"
#include "VulkanPBRObject.h"

#include "Graphics/VulkanGraphicsFunctions.h"

#include <gli.hpp>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanPBRObject::VulkanPBRObject(ADGRVulkanGraphicsRenderableObjectInitInfo initInfo) : VulkanGraphicsRenderableObject(initInfo)
			{
				myRenderData.enableMaterials = true;
			}

			ADGRVulkanRenderData VulkanPBRObject::initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples)
			{
				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;

				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 1; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 2; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 3; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 4; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 5; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 6; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 7; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 8; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 9; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);
				myRenderData.descriptors.initializeLayout(logicalDevice, layoutInitInfo);

				ADGRVulkanGraphicsPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.layouts = { myRenderData.descriptors.layout };

				//VkPushConstantRange pushConstantRange;
				//pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				//pushConstantRange.size = sizeof(glm::vec3);
				//pushConstantRange.offset = 0;
				//pipelineLayoutInitInfo.pushConstantRanges.pushBack(pushConstantRange);
				//
				//pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				//pushConstantRange.size = sizeof(ADGRVulkanMaterialDescriptor::PushBlock);
				//pushConstantRange.offset = sizeof(glm::vec3);
				//pipelineLayoutInitInfo.pushConstantRanges.pushBack(pushConstantRange);
				initializePipelineLayout(pipelineLayoutInitInfo);
				//
				//myRenderData.pushConstants.pushBack(vertPushConstant);
				//myRenderData.pushConstants.pushBack(fragPushConstant);

				ARRAY<VulkanGraphicsShader> _shaders;

				if (_object.vertexShaderPath.size() && _object.vertexShaderPath != "NONE")
				{
					ADGRVulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = _object.vertexShaderPath;
					_initInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.tessellationShaderPath.size() && _object.tessellationShaderPath != "NONE")
				{
					ADGRVulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = _object.tessellationShaderPath;
					_initInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.geometryShaderPath.size() && _object.geometryShaderPath != "NONE")
				{
					ADGRVulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = _object.geometryShaderPath;
					_initInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.fragmentShaderPath.size() && _object.fragmentShaderPath != "NONE")
				{
					ADGRVulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = _object.fragmentShaderPath;
					_initInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}

				// initialize pipeline
				ADGRVulkanGraphicsPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = _shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
				pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
				pipelineInitInfo.isTexturesAvailable = _object.texturePaths.size();
				pipelineInitInfo.depthStencilTestEnable = VK_TRUE;
				pipelineInitInfo.depthStencilWriteEnable = VK_TRUE;
				pipelineInitInfo.rasterizerCullMode = VK_CULL_MODE_FRONT_BIT;
				initializePipeline(pipelineInitInfo);

				for (VulkanGraphicsShader _shader : _shaders)
					_shader.terminate(logicalDevice);

				ARRAY<ADGRVulkanTextureInitInfo> textureInitInfos;

				// initialize textures
				for (UI32 _itr = 0; _itr < _object.texturePaths.size(); _itr++)
				{
					ADGRVulkanTextureInitInfo initInfo;
					initInfo.path = _object.texturePaths[_itr];
					initInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
					initInfo.mipLevels = 1;
					initInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					initInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					initInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					initInfo.magFilter = VK_FILTER_LINEAR;
					initInfo.minFilter = VK_FILTER_LINEAR;
					initInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					textureInitInfos.pushBack(initInfo);
				}

				initializeTextures(textureInitInfos);

				// initialize vertex buffers
				for (UI32 _itr = 0; _itr < _object.vertexBufferObjects->size(); _itr++)
					initializeVertexBuffer(&_object.vertexBufferObjects->at(_itr));

				// initialize index buffers
				for (UI32 _itr = 0; _itr < _object.indexBufferObjects->size(); _itr++)
					initializeIndexBufferUI32(&_object.indexBufferObjects->at(_itr));

				// initialize uniform buffers
				initializeUniformBuffer();

				// initialize descriptor pool
				initializeDescriptorPool();

				// initialize descriptor sets
				initializeDescriptorSets();

				return myRenderData;
			}

			void VulkanPBRObject::initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos)
			{
				if ((infos[0].path.find(".ktx") != std::string::npos) && (infos.size() < 6))
				{
					VkPhysicalDeviceFeatures _features;
					vkGetPhysicalDeviceFeatures(physicalDevice, &_features);

					bool found = false;

					for (const ADGRVulkanTextureInitInfo& info : infos)
					{
						ADGRVulkanTextureContainer _container;
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
						VulkanGraphicsFunctions::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

						ADGRVulkanCopyBufferToImageInfo cpyInfo;
						cpyInfo.buffer = stagingBuffer;
						cpyInfo.image = _container.image;
						cpyInfo.destinationImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
						VulkanGraphicsFunctions::copyBufferToImageOverride(logicalDevice, commandPool, graphicsQueue, presentQueue, cpyInfo, bufferCopyRegions);

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

						myRenderData.textures.pushBack(_container);
					}
				}
				else
				{
					for (const ADGRVulkanTextureInitInfo& info : infos)
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
						VulkanGraphicsFunctions::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

						ADGRVulkanCopyBufferToImageInfo cpyInfo;
						cpyInfo.buffer = stagingBuffer;
						cpyInfo.image = _container.image;
						cpyInfo.width = static_cast<UI32>(texData.texWidth);
						cpyInfo.height = static_cast<UI32>(texData.texHeight);
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
			}

			void VulkanPBRObject::initializeUniformBuffer()
			{
				myRenderData.uniformBufferContainers.pushBack(
					VulkanGraphicsFunctions::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UBO_MVPC),
						myRenderData.swapChainPointer->swapChainImages.size()
						)
					);

				myRenderData.uniformBufferContainers.pushBack(
					VulkanGraphicsFunctions::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UBO_L4),
						myRenderData.swapChainPointer->swapChainImages.size()
						)
					);
			}

			void VulkanPBRObject::updateUniformBuffer(UBO_MVPC uniformBufferObject, UI32 currentImage)
			{
				ADGRVulkanUnformBufferContainer _container = myRenderData.uniformBufferContainers[0];
				void* data = nullptr;
				vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uniformBufferObject), 0, &data);
				memcpy(data, &uniformBufferObject, sizeof(uniformBufferObject));
				vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);

				const float p = 15.0f;
				UBO_L4 uboParams;
				uboParams.lights[0] = glm::vec4(-p, -p * 0.5f, -p, 1.0f);
				uboParams.lights[1] = glm::vec4(-p, -p * 0.5f, p, 1.0f);
				uboParams.lights[2] = glm::vec4(p, -p * 0.5f, p, 1.0f);
				uboParams.lights[3] = glm::vec4(p, -p * 0.5f, -p, 1.0f);

				_container = myRenderData.uniformBufferContainers[1];
				data = nullptr;
				vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uboParams), 0, &data);
				memcpy(data, &uboParams, sizeof(uboParams));
				vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);
			}

			void VulkanPBRObject::initializeDescriptorPool()
			{
				ARRAY<VkDescriptorPoolSize> poolSizes = {};

				VkDescriptorPoolSize _poolSize1;
				_poolSize1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_poolSize1.descriptorCount = 1;
				poolSizes.push_back(_poolSize1);

				_poolSize1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_poolSize1.descriptorCount = 1;
				poolSizes.push_back(_poolSize1);

				if (myRenderData.textures.size())
				{
					VkDescriptorPoolSize _poolSize2;
					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);
				}

				ADGRVulkanDescriptorPoolInitInfo initInfo;
				initInfo.poolSizes = poolSizes;
				initInfo.maxDescriptorSets = 1;
				myRenderData.descriptors.initializePool(logicalDevice, initInfo);
			}

			void VulkanPBRObject::initializeDescriptorSets()
			{
				ARRAY<VkWriteDescriptorSet> descriptorWrites = {};
				ARRAY< VkDescriptorBufferInfo> bufferInfos1;
				ARRAY< VkDescriptorBufferInfo> bufferInfos2;

				for (UI32 index = 0; index < myRenderData.uniformBufferContainers[0].buffers.size(); index++)
				{
					VkDescriptorBufferInfo bufferInfo1 = {};
					bufferInfo1.buffer = myRenderData.uniformBufferContainers[0].buffers[index];
					bufferInfo1.offset = 0;
					bufferInfo1.range = sizeof(UBO_MVPC);
					bufferInfos1.pushBack(bufferInfo1);

					VkDescriptorBufferInfo bufferInfo2 = {};
					bufferInfo2.buffer = myRenderData.uniformBufferContainers[1].buffers[index];
					bufferInfo2.offset = 0;
					bufferInfo2.range = sizeof(UBO_L4);
					bufferInfos2.pushBack(bufferInfo2);
				}

				{
					VkWriteDescriptorSet _writes1;
					_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes1.dstBinding = 0;
					_writes1.dstArrayElement = 0;
					_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					_writes1.descriptorCount = 1;
					_writes1.pBufferInfo = bufferInfos1.data();
					_writes1.pNext = VK_NULL_HANDLE;
					_writes1.pImageInfo = VK_NULL_HANDLE;
					_writes1.pTexelBufferView = VK_NULL_HANDLE;
					descriptorWrites.push_back(_writes1);
				}

				{
					VkWriteDescriptorSet _writes1;
					_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes1.dstBinding = 1;
					_writes1.dstArrayElement = 0;
					_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					_writes1.descriptorCount = 1;
					_writes1.pBufferInfo = bufferInfos2.data();
					_writes1.pNext = VK_NULL_HANDLE;
					_writes1.pImageInfo = VK_NULL_HANDLE;
					_writes1.pTexelBufferView = VK_NULL_HANDLE;
					descriptorWrites.push_back(_writes1);
				}

				{
					VkDescriptorImageInfo imageInfo = {};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = myIrradianceCube.myTextureContainer.imageView;
					imageInfo.sampler = myIrradianceCube.myTextureContainer.imageSampler;

					VkWriteDescriptorSet _writes2;
					_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes2.dstBinding = 2;
					_writes2.dstArrayElement = 0;
					_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_writes2.descriptorCount = 1;
					_writes2.pImageInfo = &imageInfo;
					_writes2.pNext = VK_NULL_HANDLE;
					_writes2.pTexelBufferView = VK_NULL_HANDLE;
					_writes2.pBufferInfo = VK_NULL_HANDLE;
					descriptorWrites.push_back(_writes2);
				}

				{
					VkDescriptorImageInfo imageInfo = {};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = myBRDF.myTextureContainer.imageView;
					imageInfo.sampler = myBRDF.myTextureContainer.imageSampler;

					VkWriteDescriptorSet _writes2;
					_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes2.dstBinding = 3;
					_writes2.dstArrayElement = 0;
					_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_writes2.descriptorCount = 1;
					_writes2.pImageInfo = &imageInfo;
					_writes2.pNext = VK_NULL_HANDLE;
					_writes2.pTexelBufferView = VK_NULL_HANDLE;
					_writes2.pBufferInfo = VK_NULL_HANDLE;
					descriptorWrites.push_back(_writes2);
				}

				{
					VkDescriptorImageInfo imageInfo = {};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = myPreFilteredCube.myTextureContainer.imageView;
					imageInfo.sampler = myPreFilteredCube.myTextureContainer.imageSampler;

					VkWriteDescriptorSet _writes2;
					_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes2.dstBinding = 4;
					_writes2.dstArrayElement = 0;
					_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_writes2.descriptorCount = 1;
					_writes2.pImageInfo = &imageInfo;
					_writes2.pNext = VK_NULL_HANDLE;
					_writes2.pTexelBufferView = VK_NULL_HANDLE;
					_writes2.pBufferInfo = VK_NULL_HANDLE;
					descriptorWrites.push_back(_writes2);
				}

				for (UI32 _texIndex = 0; _texIndex < myRenderData.textures.size(); _texIndex++)
				{
					VkDescriptorImageInfo imageInfo = {};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = myRenderData.textures[_texIndex].imageView;
					imageInfo.sampler = myRenderData.textures[_texIndex].imageSampler;

					VkWriteDescriptorSet _writes2;
					_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes2.dstBinding = 5 + _texIndex;
					_writes2.dstArrayElement = 0;
					_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_writes2.descriptorCount = 1;
					_writes2.pImageInfo = &imageInfo;
					_writes2.pNext = VK_NULL_HANDLE;
					_writes2.pTexelBufferView = VK_NULL_HANDLE;
					_writes2.pBufferInfo = VK_NULL_HANDLE;
					descriptorWrites.push_back(_writes2);
				}

				ADGRVulkanDescriptorSetsInitInfo info;
				info.descriptorWrites = descriptorWrites;
				myRenderData.descriptors.initializeSets(logicalDevice, info);
			}

			VulkanPBRObject::_VertexPushConstant::_VertexPushConstant()
			{
				stageFlag = VK_SHADER_STAGE_VERTEX_BIT;
				offset = 0;
				byteSize = sizeof(glm::vec3);
			}

			VulkanPBRObject::_FragmentPushConstant::_FragmentPushConstant()
			{
				stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT;
				offset = sizeof(glm::vec3);
				byteSize = sizeof(ADGRVulkanMaterialDescriptor::PushBlock);
			}
		}
	}
}