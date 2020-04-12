#include "adgrafx.h"
#include "VulkanSkeletalAnimation.h"

#include "../Objects/VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void ADGRVulkanSkeletalBoneData::add(UI32 boneID, F32 weight)
			{
				for (UI32 _itr = 0; _itr < MAX_BONES_PER_VERTEX; _itr++)
				{
					if (weights[_itr] == 0.0f)
					{
						IDs[_itr] = boneID;
						weights[_itr] = weight;
						return;
					}
				}
			}

			ADGRVulkanSkeletalBoneInfo::ADGRVulkanSkeletalBoneInfo()
			{
				offset = aiMatrix4x4();
				finalTransformation = aiMatrix4x4();
			}

			VulkanSkeletalAnimation::VulkanSkeletalAnimation(ADGRVulkanRenderableObjectInitInfo info) : VulkanRenderableObject(info)
			{
				myRenderData.type = DMKObjectType::DMK_OBJECT_TYPE_SKELETAL_ANIMATION;
			}

			ADGRVulkanRenderData VulkanSkeletalAnimation::initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples)
			{
				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
				myRenderData.descriptors.layout = VulkanRenderLayout::createDescriptorSetLayout(logicalDevice, layoutInitInfo);

				ADGRVulkanPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.layouts = { myRenderData.descriptors.layout };
				myRenderData.pipelineLayout = VulkanRenderLayout::createPipelineLayout(logicalDevice, pipelineLayoutInitInfo);

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
				pipelineInitInfo.vertexBindingDescription = SkeletalVertex::getBindingDescription(1);
				pipelineInitInfo.vertexAttributeDescription = SkeletalVertex::getAttributeDescriptions();
				pipelineInitInfo.isTexturesAvailable = _object.texturePaths.size();
				pipelineInitInfo.rasterizerFrontFace = VK_FRONT_FACE_CLOCKWISE;
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

			void VulkanSkeletalAnimation::setAnimation(UI32 animationIndex)
			{
				if (animationIndex >= myAnimationData.scene->mNumAnimations)
					DMK_CORE_FATAL("Invalid animation index!");

				myAnimationData.pAnimation = myAnimationData.scene->mAnimations[animationIndex];
			}

			void VulkanSkeletalAnimation::loadBones(const aiMesh* pMesh, UI32 vertexOffset, ARRAY<ADGRVulkanSkeletalBoneData>& bones)
			{
				for (UI32 _itr = 0; _itr < pMesh->mNumBones; _itr++)
				{
					UI32 index = 0;

					if (pMesh->mNumBones > MAX_BONES)
						DMK_CORE_FATAL("Invalid number of bones!");

					std::string name(pMesh->mBones[_itr]->mName.data);

					if (myAnimationData.boneMapping.find(name) == myAnimationData.boneMapping.end())
					{
						// Bone not present, add new one
						index = myAnimationData.numBones;
						myAnimationData.numBones++;
						ADGRVulkanSkeletalBoneInfo bone;
						myAnimationData.boneInfo.push_back(bone);
						myAnimationData.boneInfo[index].offset = pMesh->mBones[_itr]->mOffsetMatrix;
						myAnimationData.boneMapping[name] = index;
					}
					else
					{
						index = myAnimationData.boneMapping[name];
					}

					for (UI32 j = 0; j < pMesh->mBones[_itr]->mNumWeights; j++)
					{
						UI32 vertexID = vertexOffset + pMesh->mBones[_itr]->mWeights[j].mVertexId;
						bones[vertexID].add(index, pMesh->mBones[_itr]->mWeights[j].mWeight);
					}

				}
				myAnimationData.boneTransforms.resize(myAnimationData.numBones);
			}

			void VulkanSkeletalAnimation::update(F32 time)
			{
				float TicksPerSecond = (float)(myAnimationData.scene->mAnimations[0]->mTicksPerSecond != 0 ? myAnimationData.scene->mAnimations[0]->mTicksPerSecond : 25.0f);
				float TimeInTicks = time * TicksPerSecond;
				float AnimationTime = fmod(TimeInTicks, (float)myAnimationData.scene->mAnimations[0]->mDuration);

				aiMatrix4x4 identity = aiMatrix4x4();
				readNodeHierarchy(AnimationTime, myAnimationData.scene->mRootNode, identity);

				for (UI32 i = 0; i < myAnimationData.boneTransforms.size(); i++)
					myAnimationData.boneTransforms[i] = myAnimationData.boneInfo[i].finalTransformation;
			}

			void VulkanSkeletalAnimation::draw(F32 time)
			{
			}

			void VulkanSkeletalAnimation::initializeVertexBuffer(ARRAY<SkeletalVertex>* vertexBufferObject)
			{
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.vertexCount = vertexBufferObject->size();
				VkDeviceSize bufferSize = myRenderData.vertexCount * vertexBufferObject->typeSize();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);

				void* data = nullptr;
				vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, vertexBufferObject->data(), (size_t)bufferSize);
				vkUnmapMemory(logicalDevice, stagingBufferMemory);

				ADGRCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &_buffer;
				vertBufferInfo.bufferMemory = &_bufferMemory;

				VulkanFunctions::createBuffer(logicalDevice, physicalDevice, vertBufferInfo);

				VulkanFunctions::copyBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue, stagingBuffer, _buffer, bufferSize);

				vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

				myRenderData.vertexBuffers.pushBack(_buffer);
				myRenderData.vertexBufferMemories.pushBack(_bufferMemory);
			}

			void VulkanSkeletalAnimation::initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos)
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
						cinfo.flags = 0;

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

						transitionInfo.image = _container.image;
						transitionInfo.format = _container.format;
						transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
						transitionInfo.newLayout = VK_IMAGE_LAYOUT_GENERAL;
						transitionInfo.mipLevels = 1;
						transitionInfo.layerCount = ktxTexture.max_face() + 1;
						VulkanFunctions::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

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
						cinfo2.layerCount = ktxTexture.max_face() + 1;
						cinfo2.component = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
						_container.imageView = VulkanFunctions::createImageView(logicalDevice, cinfo2);

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
					transitionInfo.newLayout = VK_IMAGE_LAYOUT_GENERAL;
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
				else
				{
					ADGRVulkanTextureInitInfo info = infos[0];
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

					VulkanFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);

					void* data;
					if (vkMapMemory(logicalDevice, stagingBufferMemory, 0, static_cast<size_t>(imageSize), 0, &data) != VK_SUCCESS)
						DMK_CORE_FATAL("Failed to map memory!");
					memcpy(data, pixels, static_cast<size_t>(imageSize));
					vkUnmapMemory(logicalDevice, stagingBufferMemory);

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

					VulkanFunctions::createImage(logicalDevice, physicalDevice, cinfo);

					ADGRTransitionImageLayoutInfo transitionInfo;
					transitionInfo.image = _container.image;
					transitionInfo.format = _container.format;
					transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					transitionInfo.mipLevels = info.mipLevels;
					transitionInfo.layerCount = 1;
					VulkanFunctions::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

					ADGRCopyBufferToImageInfo cpyInfo;
					cpyInfo.buffer = stagingBuffer;
					cpyInfo.image = _container.image;
					cpyInfo.width = static_cast<UI32>(texData.texWidth);
					cpyInfo.height = static_cast<UI32>(texData.texHeight);
					VulkanFunctions::copyBufferToImage(logicalDevice, commandPool, graphicsQueue, presentQueue, cpyInfo);

					transitionInfo.image = _container.image;
					transitionInfo.format = _container.format;
					transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					transitionInfo.mipLevels = info.mipLevels;
					transitionInfo.layerCount = 1;
					VulkanFunctions::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);

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
					_container.imageSampler = VulkanFunctions::createImageSampler(logicalDevice, samplerInitInfo);

					ADGRCreateImageViewInfo cinfo2;
					cinfo2.image = _container.image;
					cinfo2.format = _container.format;
					cinfo2.mipLevels = _container.mipLevels;
					cinfo2.aspectFlags = info.aspectFlags;
					_container.imageView = VulkanFunctions::createImageView(logicalDevice, cinfo2);
				}

				myRenderData.textures.pushBack(_container);
			}

			void VulkanSkeletalAnimation::initializeUniformBuffer()
			{
				myRenderData.uniformBufferContainers.pushBack(
					VulkanFunctions::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UBO_SKELETAL),
						myRenderData.swapChainPointer->getSwapChainImages().size()
						)
					);
			}

			void VulkanSkeletalAnimation::updateUniformBuffer(UBO_SKELETAL uniformBufferObject, UI32 currentImage, F32 time)
			{
				update(time);

				for (UI32 _itr = 0; _itr < myAnimationData.boneTransforms.size(); _itr++)
					uniformBufferObject.bones[_itr] = glm::transpose(glm::make_mat4(&myAnimationData.boneTransforms[_itr].a1));

				for (ADGRVulkanUnformBufferContainer _container : myRenderData.uniformBufferContainers)
				{
					void* data = nullptr;
					vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uniformBufferObject), 0, &data);
					memcpy(data, &uniformBufferObject, sizeof(uniformBufferObject));
					vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);
				}
			}

			void VulkanSkeletalAnimation::initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info)
			{
				VkDescriptorSetLayout _layout = VK_NULL_HANDLE;
				if (myRenderData.textures.size())
					_layout = myRenderData.descriptors.layout;
				else
					_layout = noTextureDescriptorSetLayout;

				VkDescriptorSetAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = myRenderData.descriptors.pool;
				allocInfo.descriptorSetCount = 1;
				allocInfo.pSetLayouts = &_layout;

				if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, &myRenderData.descriptors.descriptorSet) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate descriptor sets!");

				ARRAY<VkWriteDescriptorSet> descriptorWrites = {};
				ARRAY<VkDescriptorBufferInfo> bufferInfos;

				for (UI32 itr = 0; itr < myRenderData.uniformBufferContainers.size(); itr++)
				{
					for (UI32 i = 0; i < myRenderData.uniformBufferContainers[itr].buffers.size(); i++)
					{
						VkDescriptorBufferInfo bufferInfo = {};
						bufferInfo.buffer = myRenderData.uniformBufferContainers[itr].buffers[i];
						bufferInfo.offset = 0;
						bufferInfo.range = sizeof(UBO_SKELETAL);
						bufferInfos.pushBack(bufferInfo);
					}
				}

				VkWriteDescriptorSet _writes1;
				_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes1.dstSet = myRenderData.descriptors.descriptorSet;
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
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
						imageInfo.imageView = myRenderData.textures[_texIndex].imageView;
						imageInfo.sampler = myRenderData.textures[_texIndex].imageSampler;

						VkWriteDescriptorSet _writes2;
						_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes2.dstSet = myRenderData.descriptors.descriptorSet;
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

				for (VkWriteDescriptorSet _write : info.additionalWrites)
					descriptorWrites.push_back(_write);

				vkUpdateDescriptorSets(logicalDevice, static_cast<UI32>(descriptorWrites.size()),
					descriptorWrites.data(), 0, nullptr);
			}

			const aiNodeAnim* VulkanSkeletalAnimation::findNodeAnim(const aiAnimation* animation, const std::string nodeName)
			{
				for (UI32 i = 0; i < animation->mNumChannels; i++)
				{
					const aiNodeAnim* nodeAnim = animation->mChannels[i];
					if (std::string(nodeAnim->mNodeName.data) == nodeName)
						return nodeAnim;
				}
				return nullptr;
			}

			aiMatrix4x4 VulkanSkeletalAnimation::interpolateTranslation(float time, const aiNodeAnim* pNodeAnim)
			{
				aiVector3D translation;

				if (pNodeAnim->mNumPositionKeys == 1)
				{
					translation = pNodeAnim->mPositionKeys[0].mValue;
				}
				else
				{
					UI32 frameIndex = 0;
					for (UI32 i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
					{
						if (time < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
						{
							frameIndex = i;
							break;
						}
					}

					aiVectorKey currentFrame = pNodeAnim->mPositionKeys[frameIndex];
					aiVectorKey nextFrame = pNodeAnim->mPositionKeys[(frameIndex + 1) % pNodeAnim->mNumPositionKeys];

					float delta = (time - (float)currentFrame.mTime) / (float)(nextFrame.mTime - currentFrame.mTime);

					const aiVector3D& start = currentFrame.mValue;
					const aiVector3D& end = nextFrame.mValue;

					translation = (start + delta * (end - start));
				}

				aiMatrix4x4 mat;
				aiMatrix4x4::Translation(translation, mat);
				return mat;
			}

			aiMatrix4x4 VulkanSkeletalAnimation::interpolateRotation(float time, const aiNodeAnim* pNodeAnim)
			{
				aiQuaternion rotation;

				if (pNodeAnim->mNumRotationKeys == 1)
				{
					rotation = pNodeAnim->mRotationKeys[0].mValue;
				}
				else
				{
					UI32 frameIndex = 0;
					for (UI32 i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
					{
						if (time < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
						{
							frameIndex = i;
							break;
						}
					}

					aiQuatKey currentFrame = pNodeAnim->mRotationKeys[frameIndex];
					aiQuatKey nextFrame = pNodeAnim->mRotationKeys[(frameIndex + 1) % pNodeAnim->mNumRotationKeys];

					float delta = (time - (float)currentFrame.mTime) / (float)(nextFrame.mTime - currentFrame.mTime);

					const aiQuaternion& start = currentFrame.mValue;
					const aiQuaternion& end = nextFrame.mValue;

					aiQuaternion::Interpolate(rotation, start, end, delta);
					rotation.Normalize();
				}

				aiMatrix4x4 mat(rotation.GetMatrix());
				return mat;
			}

			aiMatrix4x4 VulkanSkeletalAnimation::interpolateScale(float time, const aiNodeAnim* pNodeAnim)
			{
				aiVector3D scale;

				if (pNodeAnim->mNumScalingKeys == 1)
				{
					scale = pNodeAnim->mScalingKeys[0].mValue;
				}
				else
				{
					UI32 frameIndex = 0;
					for (UI32 i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
					{
						if (time < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
						{
							frameIndex = i;
							break;
						}
					}

					aiVectorKey currentFrame = pNodeAnim->mScalingKeys[frameIndex];
					aiVectorKey nextFrame = pNodeAnim->mScalingKeys[(frameIndex + 1) % pNodeAnim->mNumScalingKeys];

					float delta = (time - (float)currentFrame.mTime) / (float)(nextFrame.mTime - currentFrame.mTime);

					const aiVector3D& start = currentFrame.mValue;
					const aiVector3D& end = nextFrame.mValue;

					scale = (start + delta * (end - start));
				}

				aiMatrix4x4 mat;
				aiMatrix4x4::Scaling(scale, mat);
				return mat;
			}

			void VulkanSkeletalAnimation::readNodeHierarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform)
			{
				std::string NodeName(pNode->mName.data);

				aiMatrix4x4 NodeTransformation(pNode->mTransformation);

				const aiNodeAnim* pNodeAnim = findNodeAnim(myAnimationData.pAnimation, NodeName);

				if (pNodeAnim)
				{
					// Get interpolated matrices between current and next frame
					aiMatrix4x4 matScale = interpolateScale(AnimationTime, pNodeAnim);
					aiMatrix4x4 matRotation = interpolateRotation(AnimationTime, pNodeAnim);
					aiMatrix4x4 matTranslation = interpolateTranslation(AnimationTime, pNodeAnim);

					NodeTransformation = matTranslation * matRotation * matScale;
				}

				aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;

				if (myAnimationData.boneMapping.find(NodeName) != myAnimationData.boneMapping.end())
				{
					UI32 BoneIndex = myAnimationData.boneMapping[NodeName];
					myAnimationData.boneInfo[BoneIndex].finalTransformation = myAnimationData.globalInverseTransform * GlobalTransformation * myAnimationData.boneInfo[BoneIndex].offset;
				}

				for (UI32 i = 0; i < pNode->mNumChildren; i++)
					readNodeHierarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
			}
		}
	}
}