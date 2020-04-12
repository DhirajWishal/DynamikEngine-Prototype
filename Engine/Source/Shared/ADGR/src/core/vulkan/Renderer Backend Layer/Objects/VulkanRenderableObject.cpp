#include "adgrafx.h"
#include "VulkanRenderableObject.h"

#include "VulkanValidator.h"
#include "VulkanExtensionsManager.h"
#include "VulkanFunctions.h"
#include "VulkanOneTimeCommandBufferManager.h"
#include "VulkanRenderLayout.h"
#include "defines.h"

#include "CentralDataHub.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			using namespace core;

			void VulkanRenderableObject::initializeResources(ADGRVulkanRenderableObjectInitInfo info)
			{
				logicalDevice = info.logicalDevice;
				physicalDevice = info.physicalDevice;
				graphicsQueue = info.graphicsQueue;
				presentQueue = info.presentQueue;
				commandPool = info.commandPool;
			}

			ADGRVulkanRenderData VulkanRenderableObject::initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples)
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
				pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
				pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
				pipelineInitInfo.isTexturesAvailable = _object.texturePaths.size();
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

			void VulkanRenderableObject::initializePipeline(ADGRVulkanPipelineInitInfo info)
			{
				VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
				vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
				vertexInputInfo.vertexBindingDescriptionCount = static_cast<UI32>(info.vertexBindingDescription.size());
				vertexInputInfo.vertexAttributeDescriptionCount = static_cast<UI32>(info.vertexAttributeDescription.size());
				vertexInputInfo.pVertexBindingDescriptions = info.vertexBindingDescription.data();
				vertexInputInfo.pVertexAttributeDescriptions = info.vertexAttributeDescription.data();

				// initialize the input assembler
				VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
				inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
				inputAssembly.topology = info.inputAssemblyTopology;
				inputAssembly.primitiveRestartEnable = info.inputAssemblyPrimitiveRestartEnable;

				// initialize the viewport(s)
				ARRAY<VkViewport> viewports;
				for (I32 i = 0; i < info.viweportCount; i++) {
					VkViewport viewport = {};
					viewport.x = 0.0f;
					viewport.y = 0.0f;
					viewport.width = (F32)myRenderData.swapChainPointer->getSwapChainExtent().width;
					viewport.height = (F32)myRenderData.swapChainPointer->getSwapChainExtent().height;
					viewport.minDepth = 0.0f;
					viewport.maxDepth = 1.0f;

					viewports.push_back(viewport);
				}

				// initialize the scissor(s)
				ARRAY<VkRect2D> scissors;
				for (I32 i = 0; i < info.scissorCount; i++) {
					VkRect2D scissor = {};
					scissor.offset = info.offsets[0];
					scissor.extent = myRenderData.swapChainPointer->getSwapChainExtent();

					scissors.push_back(scissor);
				}

				// initialize the viewport state
				VkPipelineViewportStateCreateInfo viewportState = {};
				viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
				viewportState.viewportCount = info.viweportCount;
				viewportState.pViewports = viewports.data();
				viewportState.scissorCount = info.scissorCount;
				viewportState.pScissors = scissors.data();

				// initialize the rasterizer
				VkPipelineRasterizationStateCreateInfo rasterizer = {};
				rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
				rasterizer.depthClampEnable = info.rasterizerDepthClampEnable;
				rasterizer.rasterizerDiscardEnable = info.rasterizerDiscardEnable;
				rasterizer.polygonMode = info.rasterizerPolygonMode;
				rasterizer.lineWidth = info.rasterizerLineWidth;
				rasterizer.cullMode = info.rasterizerCullMode;
				rasterizer.frontFace = info.rasterizerFrontFace;
				rasterizer.depthBiasEnable = info.rasterizerDepthBiasEnable;

				// initialize multisampling
				VkPipelineMultisampleStateCreateInfo multisampling = {};
				multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
				multisampling.rasterizationSamples = info.multisamplerMsaaSamples;
				multisampling.sampleShadingEnable = info.multisamplerSampleShadingEnable; // enable sample shading in the pipeline
				multisampling.minSampleShading = info.multisamplerMinSampleShading; // min fraction for sample shading; closer to one is smoother

				VkPipelineDepthStencilStateCreateInfo depthStencil = {};
				depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
				depthStencil.depthTestEnable = info.depthStencilEnable;
				depthStencil.depthWriteEnable = info.depthStencilWriteEnable;
				depthStencil.depthCompareOp = info.depthStencilCompareOp;
				depthStencil.depthBoundsTestEnable = info.depthStencilBoundsTestEnable;
				depthStencil.stencilTestEnable = info.depthStencilTestEnable;

				ARRAY<VkPipelineColorBlendAttachmentState> colorBlendAttachments;

				// initialize the color blender(s)
				for (I32 i = 0; i < info.colorBlendingColorBlendCount; i++) {
					VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
					colorBlendAttachment.colorWriteMask = info.colorBlenderColorWriteMasks[i];
					colorBlendAttachment.blendEnable = info.colorBlenderBlendEnables[i];

					colorBlendAttachments.push_back(colorBlendAttachment);
				}

				for (VkPipelineColorBlendAttachmentState _state : info.additionalColorBlendStates)
					colorBlendAttachments.pushBack(_state);

				// initialize the color blender state
				VkPipelineColorBlendStateCreateInfo colorBlending = {};
				colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
				colorBlending.logicOpEnable = info.colorBlendingLogicOpEnable;
				colorBlending.logicOp = info.colorBlendingLogicOp;
				colorBlending.attachmentCount = colorBlendAttachments.size();
				colorBlending.pAttachments = colorBlendAttachments.data();
				colorBlending.blendConstants[0] = info.colorBlendingBlendConstants[0];
				colorBlending.blendConstants[1] = info.colorBlendingBlendConstants[1];
				colorBlending.blendConstants[2] = info.colorBlendingBlendConstants[2];
				colorBlending.blendConstants[3] = info.colorBlendingBlendConstants[3];

				// initialize dynamic state
				VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
				if (info.dynamicStateEnable) {
					ARRAY<VkDynamicState> dynamicState = {
						VK_DYNAMIC_STATE_VIEWPORT,
						VK_DYNAMIC_STATE_SCISSOR
					};

					dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
					dynamicStateInfo.dynamicStateCount = static_cast<UI32>(dynamicState.size());
					dynamicStateInfo.pDynamicStates = dynamicState.data();
					dynamicStateInfo.flags = info.dynamicStateFlags;
				}

				ARRAY<VkPipelineShaderStageCreateInfo> shaderStages;
				for (VulkanShader shader : info.shaders)
					shaderStages.pushBack(shader.stageCreateInfo);

				// initialize the pipeline
				VkGraphicsPipelineCreateInfo pipelineInfo = {};
				pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
				pipelineInfo.pNext = nullptr;
				pipelineInfo.stageCount = shaderStages.size();
				pipelineInfo.pStages = shaderStages.data();
				pipelineInfo.pVertexInputState = &vertexInputInfo;
				pipelineInfo.pInputAssemblyState = &inputAssembly;
				pipelineInfo.pViewportState = &viewportState;
				pipelineInfo.pRasterizationState = &rasterizer;
				pipelineInfo.pMultisampleState = &multisampling;
				pipelineInfo.pDepthStencilState = &depthStencil;
				pipelineInfo.pColorBlendState = &colorBlending;
				pipelineInfo.subpass = info.pipelineSubPass;
				pipelineInfo.basePipelineHandle = info.pipelineBasePipelineHandle;
				pipelineInfo.basePipelineIndex = info.pipelineBasePipelineIndex;
				pipelineInfo.pTessellationState = nullptr;
				pipelineInfo.layout = info.pipelineLayout;
				pipelineInfo.renderPass = myRenderData.swapChainPointer->getRenderPass();

				if (info.renderPass != VK_NULL_HANDLE)
					pipelineInfo.renderPass = info.renderPass;

				if (!info.isTexturesAvailable)
				{
					initializeNoTextureDescriptorSetLayout();
					initializeNoTexturePipelineLayout();
				}
				else if (info.pipelineLayout == VK_NULL_HANDLE)
				{
					pipelineInfo.layout = myRenderData.pipelineLayout;
				}

				if (info.dynamicStateEnable)
					pipelineInfo.pDynamicState = &dynamicStateInfo;

				// create the pipeline
				VkPipeline _pipeline = VK_NULL_HANDLE;
				if (vkCreateGraphicsPipelines(logicalDevice, info.pipelineCache, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create graphics pipeline!");

				myRenderData.pipeline = _pipeline;
			}

			void VulkanRenderableObject::terminatePipeline()
			{
				vkDestroyPipeline(logicalDevice, myRenderData.pipeline, nullptr);
			}

			void VulkanRenderableObject::initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos)
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

					myRenderData.textures.pushBack(_container);
				}
			}

			void  VulkanRenderableObject::generateMipMaps(POINTER<ADGRVulkanTextureContainer> container)
			{
				UI32 _width = container->width, _height = container->height;

				VkFormatProperties formatProperties;
				vkGetPhysicalDeviceFormatProperties(physicalDevice, container->format, &formatProperties);

				if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
					DMK_CORE_FATAL("texture image format does not support linear blitting!");

				VulkanOneTimeCommandBufferManager oneTimeCommandBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue);
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

			void VulkanRenderableObject::terminateTextures()
			{
				for (ADGRVulkanTextureContainer container : myRenderData.textures) {
					vkDestroySampler(logicalDevice, container.imageSampler, nullptr);
					vkDestroyImageView(logicalDevice, container.imageView, nullptr);

					vkDestroyImage(logicalDevice, container.image, nullptr);
					vkFreeMemory(logicalDevice, container.imageMemory, nullptr);
				}
			}

			void VulkanRenderableObject::initializeVertexBuffer(ARRAY<Vertex>* vertexes)
			{
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.vertexCount = vertexes->size();
				VkDeviceSize bufferSize = myRenderData.vertexCount * vertexes->typeSize();

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
				memcpy(data, vertexes->data(), (size_t)bufferSize);
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

			void VulkanRenderableObject::initializeVertex2DBuffer(ARRAY<vertex2D>* vertexes)
			{
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.vertexCount = vertexes->size();
				VkDeviceSize bufferSize = myRenderData.vertexCount * vertexes->typeSize();

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
				memcpy(data, vertexes->data(), (size_t)bufferSize);
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

			void VulkanRenderableObject::initializeVertexBufferP(ARRAY<VertexP>* vertexes)
			{
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.vertexCount = vertexes->size();
				VkDeviceSize bufferSize = myRenderData.vertexCount * vertexes->typeSize();

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
				memcpy(data, vertexes->data(), (size_t)bufferSize);
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

			void VulkanRenderableObject::initializeVertexBufferPN(ARRAY<VertexPN>* vertexes)
			{
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.vertexCount = vertexes->size();
				VkDeviceSize bufferSize = myRenderData.vertexCount * vertexes->typeSize();

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
				memcpy(data, vertexes->data(), (size_t)bufferSize);
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

			void VulkanRenderableObject::terminateVertexBuffer()
			{
				for (UI32 _itr = 0; _itr < myRenderData.vertexBuffers.size(); _itr++)
				{
					vkDestroyBuffer(logicalDevice, myRenderData.vertexBuffers[_itr], nullptr);
					vkFreeMemory(logicalDevice, myRenderData.vertexBufferMemories[_itr], nullptr);
				}
			}

			void VulkanRenderableObject::initializeIndexBufferUI8(ARRAY<UI8>* indexes)
			{
				myRenderData.indexbufferObjectTypeSize = sizeof(UI8);
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.indexCount = indexes->size();
				VkDeviceSize bufferSize = myRenderData.indexCount * indexes->typeSize();

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
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(logicalDevice, stagingBufferMemory);

				ADGRCreateBufferInfo indexBufferInfo;
				indexBufferInfo.bufferSize = bufferSize;
				indexBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				indexBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				indexBufferInfo.buffer = &_buffer;
				indexBufferInfo.bufferMemory = &_bufferMemory;

				VulkanFunctions::createBuffer(logicalDevice, physicalDevice, indexBufferInfo);

				VulkanFunctions::copyBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue, stagingBuffer, _buffer, bufferSize);

				vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

				myRenderData.indexBuffers.pushBack(_buffer);
				myRenderData.indexBufferMemories.pushBack(_bufferMemory);
			}

			void VulkanRenderableObject::initializeIndexBufferUI16(ARRAY<UI16>* indexes)
			{
				myRenderData.indexbufferObjectTypeSize = sizeof(UI16);
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.indexCount = indexes->size();
				VkDeviceSize bufferSize = myRenderData.indexCount * indexes->typeSize();

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
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(logicalDevice, stagingBufferMemory);

				ADGRCreateBufferInfo indexBufferInfo;
				indexBufferInfo.bufferSize = bufferSize;
				indexBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				indexBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				indexBufferInfo.buffer = &_buffer;
				indexBufferInfo.bufferMemory = &_bufferMemory;

				VulkanFunctions::createBuffer(logicalDevice, physicalDevice, indexBufferInfo);

				VulkanFunctions::copyBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue, stagingBuffer, _buffer, bufferSize);

				vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

				myRenderData.indexBuffers.pushBack(_buffer);
				myRenderData.indexBufferMemories.pushBack(_bufferMemory);
			}

			void VulkanRenderableObject::initializeIndexBufferUI32(ARRAY<UI32>* indexes)
			{
				myRenderData.indexbufferObjectTypeSize = sizeof(UI32);
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.indexCount = indexes->size();
				VkDeviceSize bufferSize = myRenderData.indexCount * indexes->typeSize();

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
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(logicalDevice, stagingBufferMemory);

				ADGRCreateBufferInfo indexBufferInfo;
				indexBufferInfo.bufferSize = bufferSize;
				indexBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				indexBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				indexBufferInfo.buffer = &_buffer;
				indexBufferInfo.bufferMemory = &_bufferMemory;

				VulkanFunctions::createBuffer(logicalDevice, physicalDevice, indexBufferInfo);

				VulkanFunctions::copyBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue, stagingBuffer, _buffer, bufferSize);

				vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

				myRenderData.indexBuffers.pushBack(_buffer);
				myRenderData.indexBufferMemories.pushBack(_bufferMemory);
			}

			void VulkanRenderableObject::initializeIndexBufferUI64(ARRAY<UI64>* indexes)
			{
				myRenderData.indexbufferObjectTypeSize = sizeof(UI64);
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.indexCount = indexes->size();
				VkDeviceSize bufferSize = myRenderData.indexCount * indexes->typeSize();

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
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(logicalDevice, stagingBufferMemory);

				ADGRCreateBufferInfo indexBufferInfo;
				indexBufferInfo.bufferSize = bufferSize;
				indexBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				indexBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				indexBufferInfo.buffer = &_buffer;
				indexBufferInfo.bufferMemory = &_bufferMemory;

				VulkanFunctions::createBuffer(logicalDevice, physicalDevice, indexBufferInfo);

				VulkanFunctions::copyBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue, stagingBuffer, _buffer, bufferSize);

				vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

				myRenderData.indexBuffers.pushBack(_buffer);
				myRenderData.indexBufferMemories.pushBack(_bufferMemory);
			}

			void VulkanRenderableObject::terminateIndexBuffer()
			{
				for (UI32 _itr = 0; _itr < myRenderData.indexBuffers.size(); _itr++)
				{
					vkDestroyBuffer(logicalDevice, myRenderData.indexBuffers[_itr], nullptr);
					vkFreeMemory(logicalDevice, myRenderData.indexBufferMemories[_itr], nullptr);
				}
			}

			void VulkanRenderableObject::initializeDescriptorPool(ADGRVulkanDescriptorPoolInitInfo info)
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

				for (VkDescriptorPoolSize _size : info.additionalSizes)
					poolSizes.push_back(_size);

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

			void VulkanRenderableObject::terminateDescriptorPool()
			{
				vkDestroyDescriptorPool(logicalDevice, myRenderData.descriptors.pool, nullptr);
			}

			void VulkanRenderableObject::initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info)
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
						bufferInfo.range = sizeof(UniformBufferObject);
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
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
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

			void VulkanRenderableObject::initializeUniformBuffer()
			{
				myRenderData.uniformBufferContainers.pushBack(
					VulkanFunctions::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UniformBufferObject),
						myRenderData.swapChainPointer->getSwapChainImages().size()
						)
					);
			}

			void VulkanRenderableObject::updateUniformBuffer(UniformBufferObject uniformBuferObject, UI32 currentImage)
			{
				for (ADGRVulkanUnformBufferContainer _container : myRenderData.uniformBufferContainers)
				{
					void* data = nullptr;
					vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uniformBuferObject), 0, &data);
					memcpy(data, &uniformBuferObject, sizeof(uniformBuferObject));
					vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);
				}
			}

			void VulkanRenderableObject::terminateUniformBuffer()
			{
				for (ADGRVulkanUnformBufferContainer _container : myRenderData.uniformBufferContainers)
				{
					for (I32 x = 0; x < _container.buffers.size(); x++) {
						vkDestroyBuffer(logicalDevice, _container.buffers[x], nullptr);
						vkFreeMemory(logicalDevice, _container.bufferMemories[x], nullptr);
					}
				}
			}

			void VulkanRenderableObject::initializeNoTextureDescriptorSetLayout()
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

			void VulkanRenderableObject::initializeNoTexturePipelineLayout()
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

				myRenderData.pipelineLayout = _pipelineLayout;
			}
		}
	}
}