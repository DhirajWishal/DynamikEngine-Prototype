#include "adgrafx.h"
#include "VulkanTextOverlay.h"

#define TEXTOVERLAY_MAX_CHAR_COUNT 2048

#include "Objects/VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanTextOverlay::VulkanTextOverlay(ADGRVulkanTextOverlayInitInfo info)
			{
				myCoreObject = info.mainCore;

				vertexShaderPath = info.vertexShaderPath;
				tessellationShaderPath = info.tessellationShaderPath;
				geometryShaderPath = info.geometryShaderPath;
				fragmentShaderPath = info.fragmentShaderPath;

				width = info.frameBufferWidth;
				height = info.frameBufferHeight;
			}

			void VulkanTextOverlay::setResources(ADGRVulkanTextOverlayInitInfo info)
			{
				myCoreObject = info.mainCore;

				vertexShaderPath = info.vertexShaderPath;
				tessellationShaderPath = info.tessellationShaderPath;
				geometryShaderPath = info.geometryShaderPath;
				fragmentShaderPath = info.fragmentShaderPath;

				width = info.frameBufferWidth;
				height = info.frameBufferHeight;
			}

			void VulkanTextOverlay::initialize()
			{
				_initializeSwapChain();
				_initializeCommandPool();

				colorBuffer.initialize(
					myCoreObject.logicalDevice,
					myCoreObject.physicalDevice,
					commandBufferManager.pool,
					myCoreObject.graphicsQueue,
					myCoreObject.presentQueue,
					mySwapChainObject.getSwapChainImageFormat(),
					mySwapChainObject.getSwapChainExtent(),
					VK_SAMPLE_COUNT_1_BIT);

				depthBuffer.initialize(
					myCoreObject.logicalDevice,
					myCoreObject.physicalDevice,
					commandBufferManager.pool,
					myCoreObject.graphicsQueue,
					myCoreObject.presentQueue,
					mySwapChainObject.getSwapChainExtent(),
					VK_SAMPLE_COUNT_1_BIT);

				ADGRVulkanFrameBufferInitInfo frameBufferInitInfo;
				frameBufferInitInfo.additionalAttachments = { colorBuffer.imageView, depthBuffer.imageView };
				frameBufferInitInfo.useSwapChainImageView = false;
				mySwapChainObject.initializeFrameBuffer(frameBufferInitInfo);

				_initializeDescriptorSetLayout();
				_initializePipelineLayout();
				_initializeVertexBuffer();
				_initializeTextureImage();
				_initializeDescriptorPool();
				_initializeDescriptorSets();
				_initializePipeline();
				isInitialized = true;
			}

			void VulkanTextOverlay::update(std::string text, F32 x, F32 y, DMKTextAlign align)
			{
				_beginUpdate();
				const UI32 firstChar = STB_FONT_consolas_24_latin1_FIRST_CHAR;

				assert(mapped != nullptr);

				const float charW = 1.5f / width;
				const float charH = 1.5f / height;

				float fbW = (float)width;
				float fbH = (float)height;
				x = (x / fbW * 2.0f) - 1.0f;
				y = (y / fbH * 2.0f) - 1.0f;

				// Calculate text width
				float textWidth = 0;
				for (auto letter : text)
				{
					stb_fontchar* charData = &stbFontData[(UI32)letter - firstChar];
					textWidth += charData->advance * charW;
				}

				switch (align)
				{
				case DMKTextAlign::DMK_TEXT_ALIGN_RIGHT:
					x -= textWidth;
					break;
				case DMKTextAlign::DMK_TEXT_ALIGN_CENTER:
					x -= textWidth / 2.0f;
					break;
				}

				// Generate a uv mapped quad per char in the new text
				for (auto letter : text)
				{
					stb_fontchar* charData = &stbFontData[(UI32)letter - firstChar];

					mapped->x = (x + (float)charData->x0 * charW);
					mapped->y = (y + (float)charData->y0 * charH);
					mapped->z = charData->s0;
					mapped->w = charData->t0;
					mapped++;

					mapped->x = (x + (float)charData->x1 * charW);
					mapped->y = (y + (float)charData->y0 * charH);
					mapped->z = charData->s1;
					mapped->w = charData->t0;
					mapped++;

					mapped->x = (x + (float)charData->x0 * charW);
					mapped->y = (y + (float)charData->y1 * charH);
					mapped->z = charData->s0;
					mapped->w = charData->t1;
					mapped++;

					mapped->x = (x + (float)charData->x1 * charW);
					mapped->y = (y + (float)charData->y1 * charH);
					mapped->z = charData->s1;
					mapped->w = charData->t1;
					mapped++;

					x += charData->advance * charW;

					numLetters++;
				}
				_endUpdate();
			}

			void VulkanTextOverlay::terminate()
			{
				// Free up all Vulkan resources requested by the text overlay
				_terminateTextureImage();
				_terminateVertexBuffer();
				//vkDestroyDescriptorSetLayout(vulkanDevice->logicalDevice, descriptorSetLayout, nullptr);
				//vkDestroyDescriptorPool(vulkanDevice->logicalDevice, descriptorPool, nullptr);
				//vkDestroyPipelineLayout(vulkanDevice->logicalDevice, pipelineLayout, nullptr);
				//vkDestroyPipelineCache(vulkanDevice->logicalDevice, pipelineCache, nullptr);
				//vkDestroyPipeline(vulkanDevice->logicalDevice, pipeline, nullptr);
				//vkDestroyRenderPass(vulkanDevice->logicalDevice, renderPass, nullptr);
				//vkDestroyCommandPool(vulkanDevice->logicalDevice, commandPool, nullptr);
			}

			VkCommandBuffer VulkanTextOverlay::draw(UI32 image)
			{
				return commandBufferManager.buffers[image];
			}

			VkSwapchainKHR VulkanTextOverlay::getSwapChain()
			{
				return mySwapChainObject.getSwapChain();
			}

			void VulkanTextOverlay::initializeCommandBuffers()
			{
				ADGRVulkanCommandBufferInitInfo info;
				info.count = mySwapChainObject.getSwapChainImages().size();
				info.objects = { myRenderData };
				info.swapChain = mySwapChainObject;
				commandBufferManager.vertexBuffer = vertexBuffer;
				commandBufferManager.numLetters = numLetters;
				commandBufferManager.initializeCommandBuffers(info);
			}

			VkRenderPass VulkanTextOverlay::getRenderPass()
			{
				return mySwapChainObject.getRenderPass();
			}

			void VulkanTextOverlay::_initializeCommandPool()
			{
				ADGRVulkanCommandBufferInitResources resourceInit;
				resourceInit.logicalDevice = myCoreObject.logicalDevice;
				resourceInit.physicalDevice = myCoreObject.physicalDevice;
				resourceInit.surface = myCoreObject.surface;
				commandBufferManager.initializeResources(resourceInit);
				commandBufferManager.initializeCommandPool();
			}

			void VulkanTextOverlay::_initializeSwapChain()
			{
				mySwapChainObject.setBasicData(
					myCoreObject.logicalDevice,
					myCoreObject.physicalDevice,
					myCoreObject.surface,
					myCoreObject.surfaceCapabilities);
				mySwapChainObject.initializeSwapChain(width, height);
				_initializeRenderPass();
			}

			void VulkanTextOverlay::_initializeRenderPass()
			{
				VkAttachmentDescription attachments[2] = {};

				// Color attachment
				attachments[0].format = mySwapChainObject.getSwapChainImageFormat();
				attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
				// Don't clear the framebuffer (like the renderpass from the example does)
				attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
				attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[0].initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

				// Depth attachment
				attachments[1].format = VulkanFunctions::findDepthFormat(myCoreObject.physicalDevice);
				attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
				attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				VkAttachmentReference colorReference = {};
				colorReference.attachment = 0;
				colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				VkAttachmentReference depthReference = {};
				depthReference.attachment = 1;
				depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				VkSubpassDescription subpass = {};
				subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpass.colorAttachmentCount = 1;
				subpass.pColorAttachments = &colorReference;
				subpass.pDepthStencilAttachment = &depthReference;

				// Use subpass dependencies for image layout transitions
				VkSubpassDependency subpassDependencies[2] = {};

				// Transition from final to initial (VK_SUBPASS_EXTERNAL refers to all commmands executed outside of the actual renderpass)
				subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependencies[0].dstSubpass = 0;
				subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				subpassDependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				// Transition from initial to final
				subpassDependencies[1].srcSubpass = 0;
				subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				subpassDependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				ARRAY<VkSubpassDescription> subPasses;
				subPasses.push_back(subpass);

				ADGRVulkanRenderPassInitInfo renderPassInitInfo;
				renderPassInitInfo.attachments = { attachments[0], attachments[1] };
				renderPassInitInfo.subPasses = subPasses;
				renderPassInitInfo.additionalSubPassDependencies = { subpassDependencies[0], subpassDependencies[1] };
				renderPassInitInfo.overrideDependencies = true;
				mySwapChainObject.initializeRenderPass(renderPassInitInfo);
			}

			void VulkanTextOverlay::_initializeVertexBuffer()
			{
				VkDeviceSize bufferSize = TEXTOVERLAY_MAX_CHAR_COUNT * sizeof(glm::vec4);

				ADGRCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &vertexBuffer;
				vertBufferInfo.bufferMemory = &vertexBufferMemory;

				VulkanFunctions::createBuffer(myCoreObject.logicalDevice, myCoreObject.physicalDevice, vertBufferInfo);
			}

			void VulkanTextOverlay::_initializeTextureImage()
			{
				VkBuffer stagingBuffer = VK_NULL_HANDLE;
				VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

				VkDeviceSize bufferSize = TEXTOVERLAY_MAX_CHAR_COUNT * sizeof(glm::vec4);
				UI32 size = width * height;

				const UI32 fontWidth = STB_FONT_consolas_24_latin1_BITMAP_WIDTH;
				const UI32 fontHeight = STB_FONT_consolas_24_latin1_BITMAP_WIDTH;
				static unsigned char font24pixels[fontWidth][fontHeight];
				stb_font_consolas_24_latin1(stbFontData, font24pixels, fontHeight);

				ADGRCreateImageInfo cinfo;
				cinfo.width = fontWidth;
				cinfo.height = fontHeight;
				cinfo.format = VK_FORMAT_R8_UNORM;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &textureContainer.image;
				cinfo.imageMemory = &textureContainer.imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
				cinfo.flags = NULL;

				VulkanFunctions::createImage(myCoreObject.logicalDevice, myCoreObject.physicalDevice, cinfo);

				VkMemoryRequirements memReq;
				vkGetImageMemoryRequirements(myCoreObject.logicalDevice, textureContainer.image, &memReq);

				ADGRCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = memReq.size;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;
				VulkanFunctions::createBuffer(myCoreObject.logicalDevice, myCoreObject.physicalDevice, bufferInfo);

				void* data;
				if (vkMapMemory(myCoreObject.logicalDevice, stagingBufferMemory, 0, static_cast<size_t>(bufferSize), 0, &data) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to map memory!");
				memcpy(data, &font24pixels[0][0], fontWidth * fontHeight);
				vkUnmapMemory(myCoreObject.logicalDevice, stagingBufferMemory);

				ADGRTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = textureContainer.image;
				transitionInfo.format = VK_FORMAT_R8_UNORM;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.mipLevels = 1;
				transitionInfo.layerCount = 1;
				VulkanFunctions::transitionImageLayout(myCoreObject.logicalDevice, commandBufferManager.pool, myCoreObject.graphicsQueue, myCoreObject.presentQueue, transitionInfo);

				ADGRCopyBufferToImageInfo cpyInfo;
				cpyInfo.buffer = stagingBuffer;
				cpyInfo.image = textureContainer.image;
				cpyInfo.width = fontWidth;
				cpyInfo.height = fontHeight;
				VulkanFunctions::copyBufferToImage(myCoreObject.logicalDevice, commandBufferManager.pool, myCoreObject.graphicsQueue, myCoreObject.presentQueue, cpyInfo);

				transitionInfo.image = textureContainer.image;
				transitionInfo.format = textureContainer.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				transitionInfo.mipLevels = 1;
				transitionInfo.layerCount = 1;
				VulkanFunctions::transitionImageLayout(myCoreObject.logicalDevice, commandBufferManager.pool, myCoreObject.graphicsQueue, myCoreObject.presentQueue, transitionInfo);

				vkDestroyBuffer(myCoreObject.logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(myCoreObject.logicalDevice, stagingBufferMemory, nullptr);

				ADGRVulkanTextureSamplerInitInfo samplerInitInfo;
				samplerInitInfo.magFilter = VK_FILTER_LINEAR;
				samplerInitInfo.minFilter = VK_FILTER_LINEAR;
				samplerInitInfo.maxMipLevels = 1;
				samplerInitInfo.minMipLevels = 0;
				samplerInitInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInitInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInitInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				textureContainer.imageSampler = VulkanFunctions::createImageSampler(myCoreObject.logicalDevice, samplerInitInfo);

				ADGRCreateImageViewInfo cinfo2;
				cinfo2.image = textureContainer.image;
				cinfo2.format = VK_FORMAT_R8_UNORM;
				cinfo2.mipLevels = 1;
				cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				cinfo2.component = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B,	VK_COMPONENT_SWIZZLE_A };;
				textureContainer.imageView = VulkanFunctions::createImageView(myCoreObject.logicalDevice, cinfo2);
			}

			void VulkanTextOverlay::_initializeDescriptorPool()
			{
				VkDescriptorPoolSize poolSize;
				poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				poolSize.descriptorCount = 1;

				VkDescriptorPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				poolInfo.poolSizeCount = 1;
				poolInfo.pPoolSizes = &poolSize;
				poolInfo.maxSets = 1;

				VkDescriptorPool _localDescriptorPool = VK_NULL_HANDLE;
				if (vkCreateDescriptorPool(myCoreObject.logicalDevice, &poolInfo, nullptr, &_localDescriptorPool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor pool!");

				myRenderData.descriptors.pool = _localDescriptorPool;
			}

			void VulkanTextOverlay::_initializeDescriptorSetLayout()
			{
				VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
				samplerLayoutBinding.binding = 0; // info.bindIndex;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
				layoutInitInfo.additionalBindings = { samplerLayoutBinding };
				layoutInitInfo.overrideBindings = true;
				//mySwapChainObject.initializeDescriptorSetLayout(layoutInitInfo);
			}

			void VulkanTextOverlay::_initializePipelineLayout()
			{
			}

			void VulkanTextOverlay::_initializeDescriptorSets()
			{
				VkDescriptorSetLayout _layout;

				VkDescriptorSetAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = myRenderData.descriptors.pool;
				allocInfo.descriptorSetCount = 1;
				allocInfo.pSetLayouts = &_layout;

				VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
				if (vkAllocateDescriptorSets(myCoreObject.logicalDevice, &allocInfo, &_descriptorSet) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate descriptor sets!");

				VkDescriptorImageInfo imageInfo = {};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = textureContainer.imageView;
				imageInfo.sampler = textureContainer.imageSampler;

				VkWriteDescriptorSet _writes;
				_writes.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes.dstSet = _descriptorSet;
				_writes.dstBinding = 0;
				_writes.dstArrayElement = 0;
				_writes.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				_writes.descriptorCount = 1;
				_writes.pImageInfo = &imageInfo;
				_writes.pNext = VK_NULL_HANDLE;
				_writes.pTexelBufferView = VK_NULL_HANDLE;

				vkUpdateDescriptorSets(myCoreObject.logicalDevice, 1, &_writes, 0, nullptr);

				myRenderData.descriptors.descriptorSet = _descriptorSet;
			}

			void VulkanTextOverlay::_initializePipelineCache()
			{
				VkPipelineCacheCreateInfo createInfo;
				createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
				if (vkCreatePipelineCache(myCoreObject.logicalDevice, &createInfo, nullptr, &pipelineCache) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create pipeline cache!");
			}

			void VulkanTextOverlay::_initializePipeline()
			{
				VkPipelineColorBlendAttachmentState blendAttachmentState{};
				blendAttachmentState.blendEnable = VK_TRUE;
				blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
				blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
				blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
				blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
				blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

				ARRAY<VulkanShader> _shaders;

				if (vertexShaderPath.size() && vertexShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = vertexShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

					VulkanShader _shader;
					_shader.initialize(myCoreObject.logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (tessellationShaderPath.size() && tessellationShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = tessellationShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

					VulkanShader _shader;
					_shader.initialize(myCoreObject.logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (geometryShaderPath.size() && geometryShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = geometryShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

					VulkanShader _shader;
					_shader.initialize(myCoreObject.logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (fragmentShaderPath.size() && fragmentShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = fragmentShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

					VulkanShader _shader;
					_shader.initialize(myCoreObject.logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}

				VulkanRenderableObject _object;
				_object.myRenderData = myRenderData;
				_object.setSwapChainContainer(&mySwapChainObject);
				ADGRVulkanRenderableObjectInitInfo objInitInfo;
				objInitInfo.logicalDevice = myCoreObject.logicalDevice;
				objInitInfo.physicalDevice = myCoreObject.physicalDevice;
				objInitInfo.commandPool = commandBufferManager.pool;
				objInitInfo.graphicsQueue = myCoreObject.graphicsQueue;
				objInitInfo.presentQueue = myCoreObject.presentQueue;
				_object.initializeResources(objInitInfo);

				ADGRVulkanPipelineInitInfo initInfo;
				initInfo.colorBlendingColorBlendCount = 0;
				initInfo.additionalColorBlendStates = { blendAttachmentState };
				initInfo.inputAssemblyTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				initInfo.inputAssemblyPrimitiveRestartEnable = VK_FALSE;
				initInfo.rasterizerPolygonMode = VK_POLYGON_MODE_FILL;
				initInfo.rasterizerCullMode = VK_CULL_MODE_BACK_BIT;
				initInfo.rasterizerFrontFace = VK_FRONT_FACE_CLOCKWISE;
				initInfo.depthStencilEnable = VK_TRUE;
				initInfo.depthStencilWriteEnable = VK_TRUE;
				initInfo.depthStencilCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
				initInfo.multisamplerMsaaSamples = VK_SAMPLE_COUNT_1_BIT;
				initInfo.dynamicStateEnable = true;
				initInfo.dynamicStateFlags = 0;
				initInfo.vertexBindingDescription = VertexP2N2::getBindingDescription(1);
				initInfo.vertexAttributeDescription = VertexP2N2::getAttributeDescriptions();
				initInfo.shaders = _shaders;
				initInfo.pipelineCache = pipelineCache;
				_object.initializePipeline(initInfo);

				myRenderData = _object.getRenderData();
			}

			void VulkanTextOverlay::_beginUpdate()
			{
				if (vkMapMemory(myCoreObject.logicalDevice, textureContainer.imageMemory, 0, VK_WHOLE_SIZE, 0, (void**)&mapped) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to map memory!");
				numLetters = 0;
			}

			void VulkanTextOverlay::_endUpdate()
			{
				vkUnmapMemory(myCoreObject.logicalDevice, textureContainer.imageMemory);
				mapped = nullptr;
				initializeCommandBuffers();
			}

			void VulkanTextOverlay::_terminateCommandPool()
			{
				commandBufferManager.terminateCommandPool();
			}

			void VulkanTextOverlay::_terminateVertexBuffer()
			{
				vkDestroyBuffer(myCoreObject.logicalDevice, vertexBuffer, nullptr);
				vkFreeMemory(myCoreObject.logicalDevice, vertexBufferMemory, nullptr);
			}

			void VulkanTextOverlay::_terminateTextureImage()
			{
				vkDestroySampler(myCoreObject.logicalDevice, textureContainer.imageSampler, nullptr);
				vkDestroyImage(myCoreObject.logicalDevice, textureContainer.image, nullptr);
				vkDestroyImageView(myCoreObject.logicalDevice, textureContainer.imageView, nullptr);
				vkFreeMemory(myCoreObject.logicalDevice, textureContainer.imageMemory, nullptr);
			}

			void VulkanTextOverlay::_terminateDescriptorPool()
			{
				vkDestroyDescriptorPool(myCoreObject.logicalDevice, myRenderData.descriptors.pool, nullptr);
			}

			void VulkanTextOverlay::_terminatePipeline()
			{
				vkDestroyPipeline(myCoreObject.logicalDevice, myRenderData.pipeline, nullptr);
			}

			void VulkanTextOverlay::_terminateSwapChain()
			{
				mySwapChainObject.terminate();
			}

			void VulkanTextOverlay::_localCommandBuffer::initializeCommandBuffers(ADGRVulkanCommandBufferInitInfo info)
			{
				UI32 size = info.count;
				buffers.resize(size);

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = pool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = size;

				if (vkAllocateCommandBuffers(logicalDevice, &allocInfo, buffers.data()) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate command buffers!");

				for (size_t i = 0; i < size; i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

					if (vkBeginCommandBuffer(buffers[i], &beginInfo) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to begin recording command commandBuffers[i]!");

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = info.swapChain.getRenderPass();
					renderPassInfo.framebuffer = info.swapChain.getFrameBuffer(i);
					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = info.swapChain.getSwapChainExtent();

					std::array<VkClearValue, 2> clearValues = {};
					//clearValues[0].color = {
					//	container->clearScreenValues[0],	// Red
					//	container->clearScreenValues[1],	// Green
					//	container->clearScreenValues[2],	// Blue
					//	container->clearScreenValues[3]		// Alpha
					//};

					clearValues[0].color = {
						0.0f, 0.0f, 0.0f, 0.0f
					};

					renderPassInfo.clearValueCount = static_cast<UI32>(clearValues.size());
					renderPassInfo.pClearValues = clearValues.data();

					/* BEGIN VULKAN COMMANDS */
					VkDeviceSize offsets[] = { 0 };
					// begin render pass
					vkCmdBeginRenderPass(buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					VkViewport viewport{};
					viewport.width = info.swapChain.getSwapChainExtent().width;
					viewport.height = info.swapChain.getSwapChainExtent().height;
					viewport.minDepth = 0;
					viewport.maxDepth = 0;
					vkCmdSetViewport(buffers[i], 0, 1, &viewport);

					VkRect2D rect2D{};
					rect2D.extent.width = info.swapChain.getSwapChainExtent().width;
					rect2D.extent.height = info.swapChain.getSwapChainExtent().height;
					rect2D.offset.x = 0.0f;
					rect2D.offset.y = 1.0f;
					vkCmdSetScissor(buffers[i], 0, 1, &rect2D);

					for (ADGRVulkanRenderData _data : info.objects)
					{
						vkCmdBindPipeline(buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _data.pipeline);

						if (_data.descriptors.descriptorSet != VK_NULL_HANDLE)
							vkCmdBindDescriptorSets(buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _data.pipelineLayout, 0, 1, &_data.descriptors.descriptorSet, 0, NULL);

						vkCmdBindVertexBuffers(buffers[i], 0, 1, &vertexBuffer, offsets);
						vkCmdBindVertexBuffers(buffers[i], 1, 1, &vertexBuffer, offsets);

						for (UI32 j = 0; j < numLetters; j++)
							vkCmdDraw(buffers[i], 4, 1, j * 4, 0);
					}
					// end renderPass
					vkCmdEndRenderPass(buffers[i]);

					/* END VULKAN COMMANDS */
					if (vkEndCommandBuffer(buffers[i]) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to record command commandBuffers[i]!");

					/*
					 Vulkan command (draw) order:
						begin render pass
						Per-object:
							init pipeline
							bind vertex buffers
							bind index buffers
							bind descriptor sets
							draw indexed data
						end render pass
						end command commandBuffers[i]
					*/
				}
			}
		}
	}
}