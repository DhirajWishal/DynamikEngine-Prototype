#include "adgrafx.h"
#include "VulkanPBRObject.h"

#include "Objects/VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanPBRObject::VulkanPBRObject(ADGRVulkanRenderableObjectInitInfo initInfo) : VulkanRenderableObject(initInfo)
			{
				myRenderData.enableMaterials = true;
			}

			ADGRVulkanRenderData VulkanPBRObject::initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples)
			{
				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
				layoutInitInfo.overrideBindings = true;

				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.additionalBindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 1; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.additionalBindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 2; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.additionalBindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 3; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.additionalBindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 4; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.additionalBindings.pushBack(uboLayoutBinding);
				myRenderData.descriptors.layout = VulkanRenderLayout::createDescriptorSetLayout(logicalDevice, layoutInitInfo);

				ADGRVulkanPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.layouts = { myRenderData.descriptors.layout };

				VkPushConstantRange pushConstantRange;
				pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				pushConstantRange.size = sizeof(glm::vec3);
				pushConstantRange.offset = 0;
				pipelineLayoutInitInfo.pushConstantRanges.pushBack(pushConstantRange);

				pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				pushConstantRange.size = sizeof(ADGRVulkanMaterialDescriptor::PushBlock);
				pushConstantRange.offset = sizeof(glm::vec3);
				pipelineLayoutInitInfo.pushConstantRanges.pushBack(pushConstantRange);
				initializePipelineLayout(pipelineLayoutInitInfo);

				myRenderData.pushConstants.pushBack(vertPushConstant);
				myRenderData.pushConstants.pushBack(fragPushConstant);

				ARRAY<VulkanShader> _shaders;

				if (_object.vertexShaderPath.size() && _object.vertexShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.vertexShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

					VulkanShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.tessellationShaderPath.size() && _object.tessellationShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.tessellationShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

					VulkanShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.geometryShaderPath.size() && _object.geometryShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.geometryShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

					VulkanShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.fragmentShaderPath.size() && _object.fragmentShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.fragmentShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

					VulkanShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}

				// initialize pipeline
				ADGRVulkanPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = _shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
				pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
				pipelineInitInfo.isTexturesAvailable = _object.texturePaths.size();
				pipelineInitInfo.depthStencilTestEnable = VK_TRUE;
				pipelineInitInfo.depthStencilWriteEnable = VK_TRUE;
				pipelineInitInfo.rasterizerCullMode = VK_CULL_MODE_FRONT_BIT;
				initializePipeline(pipelineInitInfo);

				for (VulkanShader _shader : _shaders)
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
				ADGRVulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
				initializeDescriptorPool(descriptorPoolInitInfo);

				// initialize descriptor sets
				ADGRVulkanDescriptorSetsInitInfo descriptorSetsInitInfo;
				initializeDescriptorSets(descriptorSetsInitInfo);

				return myRenderData;
			}

			void VulkanPBRObject::initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos)
			{
				ADGRVulkanTextureContainer _container;
				if ((infos[0].path.find(".ktx") != std::string::npos) || (infos.size() < 6))
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
						_container.imageSampler = VulkanFunctions::createImageSampler(logicalDevice, samplerInitInfo);

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
					}
				}
				else
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

					ADGRCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = totalSize;
					bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &bufferContainer.buffer;
					bufferInfo.bufferMemory = &bufferContainer.bufferMemory;
					VulkanFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);

					_container.mipLevels = 1;
					_container.format = VK_FORMAT_R8G8B8A8_UNORM;

					void* data = nullptr;
					if (vkMapMemory(logicalDevice, bufferContainer.bufferMemory, 0, static_cast<size_t>(totalSize), 0, &data) != VK_SUCCESS)
						DMK_CORE_FATAL("Failed to map memory!");
					for (UI32 i = 0; i < 6; i++)
						memcpy((void*)((UI64(data)) + (layerSize * i)), images[i], static_cast<size_t>(layerSize));
					vkUnmapMemory(logicalDevice, bufferContainer.bufferMemory);

					ADGRCreateImageInfo cinfo;
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
					VulkanFunctions::createImage(logicalDevice, physicalDevice, cinfo);

					ADGRTransitionImageLayoutInfo transitionInfo;
					transitionInfo.image = _container.image;
					transitionInfo.format = _container.format;
					transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					transitionInfo.mipLevels = 1;
					transitionInfo.layerCount = 6;
					VulkanFunctions::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

					ADGRCopyBufferToImageInfo cpyInfo;
					cpyInfo.buffer = bufferContainer.buffer;
					cpyInfo.image = _container.image;
					cpyInfo.width = static_cast<UI32>(texData.texWidth);
					cpyInfo.height = static_cast<UI32>(texData.texHeight);
					cpyInfo.baseArrayCount = 0;
					cpyInfo.layerCount = 6;
					VulkanFunctions::copyBufferToImage(logicalDevice, commandPool, graphicsQueue, presentQueue, cpyInfo);

					transitionInfo.image = _container.image;
					transitionInfo.format = _container.format;
					transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					transitionInfo.mipLevels = 1;
					transitionInfo.layerCount = 6;
					VulkanFunctions::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

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
					_container.imageSampler = VulkanFunctions::createImageSampler(logicalDevice, samplerInitInfo);

					ADGRCreateImageViewInfo cinfo2;
					cinfo2.image = _container.image;
					cinfo2.format = _container.format;
					cinfo2.mipLevels = _container.mipLevels;
					cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					cinfo2.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
					cinfo2.layerCount = 6;
					cinfo2.component = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
					_container.imageView = VulkanFunctions::createImageView(logicalDevice, cinfo2);

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

				myRenderData.textures.pushBack(_container);
			}

			void VulkanPBRObject::initializeUniformBuffer()
			{
				myRenderData.uniformBufferContainers.pushBack(
					VulkanFunctions::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UBO_MVPC),
						myRenderData.swapChainPointer->swapChainImages.size()
						)
					);

				myRenderData.uniformBufferContainers.pushBack(
					VulkanFunctions::createUniformBuffers(
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

			void VulkanPBRObject::initializeDescriptorPool(ADGRVulkanDescriptorPoolInitInfo info)
			{
				ARRAY<VkDescriptorPoolSize> poolSizes = {};

				VkDescriptorPoolSize _poolSize1;
				_poolSize1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_poolSize1.descriptorCount = myRenderData.uniformBufferContainers.size();
				poolSizes.push_back(_poolSize1);

				if (myRenderData.textures.size())
				{
					VkDescriptorPoolSize _poolSize2;
					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = myRenderData.textures.size();

					poolSizes.push_back(_poolSize2);
				}

				VkDescriptorPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				poolInfo.poolSizeCount = poolSizes.size();
				poolInfo.pPoolSizes = poolSizes.data();
				poolInfo.maxSets = 1;

				VkDescriptorPool _localDescriptorPool = VK_NULL_HANDLE;
				if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &_localDescriptorPool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor pool!");

				myRenderData.descriptors.pool = _localDescriptorPool;
			}

			void VulkanPBRObject::initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info)
			{
				VkDescriptorSetLayout _layout = myRenderData.descriptors.layout;

				VkDescriptorSetAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = myRenderData.descriptors.pool;
				allocInfo.descriptorSetCount = 1;
				allocInfo.pSetLayouts = &_layout;

				if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, &myRenderData.descriptors.descriptorSet) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate descriptor sets!");

				ARRAY<VkWriteDescriptorSet> descriptorWrites = {};
				ARRAY<VkDescriptorBufferInfo> bufferInfos1;
				ARRAY<VkDescriptorBufferInfo> bufferInfos2;

				ADGRVulkanUnformBufferContainer _container1 = myRenderData.uniformBufferContainers[0];

				for (UI32 itr = 0; itr < _container1.buffers.size(); itr++)
				{
					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = _container1.buffers[itr];
					bufferInfo.offset = 0;
					bufferInfo.range = sizeof(UBO_MVPC);
					bufferInfos1.pushBack(bufferInfo);
				}

				ADGRVulkanUnformBufferContainer _container2 = myRenderData.uniformBufferContainers[1];

				for (UI32 itr = 0; itr < _container2.buffers.size(); itr++)
				{
					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = _container2.buffers[itr];
					bufferInfo.offset = 0;
					bufferInfo.range = sizeof(UBO_L4EG);
					bufferInfos2.pushBack(bufferInfo);
				}

				VkWriteDescriptorSet _writes1;
				_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes1.dstSet = myRenderData.descriptors.descriptorSet;
				_writes1.dstBinding = 0;
				_writes1.dstArrayElement = 0;
				_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_writes1.descriptorCount = 1;
				_writes1.pBufferInfo = bufferInfos1.data();
				_writes1.pNext = VK_NULL_HANDLE;
				_writes1.pImageInfo = VK_NULL_HANDLE;
				_writes1.pTexelBufferView = VK_NULL_HANDLE;
				descriptorWrites.push_back(_writes1);

				_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes1.dstSet = myRenderData.descriptors.descriptorSet;
				_writes1.dstBinding = 1;
				_writes1.dstArrayElement = 0;
				_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_writes1.descriptorCount = 1;
				_writes1.pBufferInfo = bufferInfos2.data();
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
						_writes2.dstSet = myRenderData.descriptors.descriptorSet;
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
				}

				for (VkWriteDescriptorSet _write : info.additionalWrites)
					descriptorWrites.push_back(_write);

				vkUpdateDescriptorSets(logicalDevice, static_cast<UI32>(descriptorWrites.size()),
					descriptorWrites.data(), 0, nullptr);
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