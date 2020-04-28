#include "dmkafx.h"
#include "VulkanGraphicsPrimitiveManager.h"

#include "../Common/VulkanUtilities.h"
#include "../Common/VulkanOneTimeCommandBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			/* Helper Functions */
			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const ARRAY<VkSurfaceFormatKHR>& availableFormats) {
				for (const auto& availableFormat : availableFormats)
					if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM
						&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
						return availableFormat;

				return ((ARRAY<VkSurfaceFormatKHR>)availableFormats)[0];
			}

			VkPresentModeKHR chooseSwapPresentMode(const ARRAY<VkPresentModeKHR>& availablePresentModes) {
				VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

				for (const auto& availablePresentMode : availablePresentModes) {
					if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
						return availablePresentMode;
					}
					else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
						bestMode = availablePresentMode;
				}

				return bestMode;
			}

			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, UI32 width, UI32 height) {
				if (capabilities.currentExtent.width != std::numeric_limits<UI32>::max())
					return capabilities.currentExtent;
				else {
					VkExtent2D actualExtent = {
						width,
						height
					};

					actualExtent.width = std::max(capabilities.minImageExtent.width,
						std::min(capabilities.maxImageExtent.width, actualExtent.width));
					actualExtent.height = std::max(capabilities.minImageExtent.height,
						std::min(capabilities.maxImageExtent.height, actualExtent.height));

					return actualExtent;
				}
			}

			/* Main class definitions */
			VulkanGraphicsPrimitiveManager VulkanGraphicsPrimitiveManager::instance;

			void VulkanGraphicsPrimitiveManager::setCoreData(POINTER<VulkanGraphicsCore> core, VkCommandPool cmdPool)
			{
				instance.myCore = core;
				instance.commandPool = cmdPool;
			}

			void VulkanGraphicsPrimitiveManager::changeCore(POINTER<VulkanGraphicsCore> newCore)
			{
				instance.myCore = newCore;
			}

			void VulkanGraphicsPrimitiveManager::changeCommandPool(VkCommandPool newCommandPool)
			{
				instance.commandPool = newCommandPool;
			}

			VulkanRenderPass VulkanGraphicsPrimitiveManager::createRenderPass(ADGRVulkanRenderPassInitInfo info)
			{
				VulkanRenderPass _renderPass;
				ARRAY<VkSubpassDependency> dependencies;

				if (!info.overrideDependencies)
				{
					VkSubpassDependency dependency = {};
					dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
					dependency.dstSubpass = info.destinationSubpass;
					dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					dependency.srcAccessMask = info.accessFlags;
					dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

					dependencies.push_back(dependency);
				}

				for (VkSubpassDependency _dependency : info.additionalSubPassDependencies)
					dependencies.push_back(_dependency);

				// render pass info
				VkRenderPassCreateInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				renderPassInfo.attachmentCount = static_cast<UI32>(info.attachments.size());
				renderPassInfo.pAttachments = info.attachments.data();
				renderPassInfo.subpassCount = info.subPasses.size();
				renderPassInfo.pSubpasses = info.subPasses.data();
				renderPassInfo.dependencyCount = dependencies.size();
				renderPassInfo.pDependencies = dependencies.data();

				// create the render pass
				if (vkCreateRenderPass(instance.myCore->logicalDevice, &renderPassInfo, nullptr, &_renderPass.renderPass) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create render pass!");

				return _renderPass;
			}

			void VulkanGraphicsPrimitiveManager::destroyRenderPass(const VulkanRenderPass& renderPass)
			{
				if (renderPass.renderPass != VK_NULL_HANDLE)
					vkDestroyRenderPass(instance.myCore->logicalDevice, renderPass.renderPass, nullptr);
			}

			VulkanFrameBuffer VulkanGraphicsPrimitiveManager::createFrameBuffer(VkRenderPass renderPass, ARRAY<VkImageView> attachments, VkExtent2D extent, UI32 layerCount)
			{
				VkFramebufferCreateInfo framebufferInfo = {};
				framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = renderPass;
				framebufferInfo.attachmentCount = attachments.size();
				framebufferInfo.pAttachments = attachments.data();
				framebufferInfo.width = extent.width;
				framebufferInfo.height = extent.height;
				framebufferInfo.layers = layerCount;

				VulkanFrameBuffer _buffer;
				_buffer.width = extent.width;
				_buffer.height = extent.height;
				if (vkCreateFramebuffer(instance.myCore->logicalDevice, &framebufferInfo, nullptr, &_buffer.buffer) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create framebuffer!");

				return _buffer;
			}

			ARRAY<VulkanFrameBuffer> VulkanGraphicsPrimitiveManager::createFrameBuffers(ADGRVulkanGraphicsFrameBufferInitInfo info)
			{
				ARRAY<VulkanFrameBuffer> _frameBuffers;

				for (size_t i = 0; i < info.bufferCount; i++)
				{
					ARRAY<VkImageView> attachments;
					for (VkImageView _imageView : info.attachments)
						attachments.push_back(_imageView);

					if ((info.swapChainImageViews[i] != VK_NULL_HANDLE) && (info.swapChainImageViews.size()))
						attachments.pushBack(info.swapChainImageViews[i]);

					_frameBuffers.pushBack(createFrameBuffer(info.renderPass, attachments, info.swapChainExtent, 1));
				}

				return _frameBuffers;
			}

			void VulkanGraphicsPrimitiveManager::destroyFrameBuffer(const ARRAY<VulkanFrameBuffer>& buffers)
			{
				for (auto frameBuffer : buffers)
					vkDestroyFramebuffer(instance.myCore->logicalDevice, frameBuffer.buffer, nullptr);
			}

			VulkanSwapChain VulkanGraphicsPrimitiveManager::createSwapChain(UI32 width, UI32 height)
			{
				VulkanGraphicsSwapChainSupportDetails swapChainSupport = VulkanGraphicsCore::querySwapChainSupport(instance.myCore->physicalDevice, instance.myCore->surface);

				VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
				VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
				VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

				VkCompositeAlphaFlagBitsKHR surfaceComposite =
					(instance.myCore->surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
					? VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
					: (instance.myCore->surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
					? VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR
					: (instance.myCore->surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
					? VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR
					: VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;

				UI32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
				if (swapChainSupport.capabilities.maxImageCount > 0
					&& imageCount > swapChainSupport.capabilities.maxImageCount)
					imageCount = swapChainSupport.capabilities.maxImageCount;

				VkSwapchainCreateInfoKHR createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				createInfo.surface = instance.myCore->surface;
				createInfo.minImageCount = imageCount;
				createInfo.imageFormat = surfaceFormat.format;
				createInfo.imageColorSpace = surfaceFormat.colorSpace;
				createInfo.imageExtent = extent;
				createInfo.imageArrayLayers = 1;
				createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				//createInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

				ADGRVulkanQueue indices = VulkanGraphicsCore::findQueueFamilies(instance.myCore->physicalDevice, instance.myCore->surface);
				UI32 queueFamilyindices[] = {
					indices.graphicsFamily.value(),
					indices.presentFamily.value()
				};

				if (indices.graphicsFamily != indices.presentFamily) {
					createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
					createInfo.queueFamilyIndexCount = 2;
					createInfo.pQueueFamilyIndices = queueFamilyindices;
				}
				else {
					createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
					createInfo.queueFamilyIndexCount = 0;
					createInfo.pQueueFamilyIndices = nullptr;
				}

				createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
				createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
				createInfo.presentMode = presentMode;
				createInfo.clipped = VK_TRUE;
				createInfo.oldSwapchain = VK_NULL_HANDLE;

				VulkanSwapChain _swapChain;
				if (vkCreateSwapchainKHR(instance.myCore->logicalDevice, &createInfo, nullptr, &_swapChain.swapChain))
					DMK_CORE_FATAL("Failed to create Swap Chain!");

				vkGetSwapchainImagesKHR(instance.myCore->logicalDevice, _swapChain.swapChain, &imageCount, nullptr);
				_swapChain.swapChainImages.resize(imageCount);
				vkGetSwapchainImagesKHR(instance.myCore->logicalDevice, _swapChain.swapChain, &imageCount, _swapChain.swapChainImages.data());

				_swapChain.swapChainImageFormat = surfaceFormat.format;
				_swapChain.swapChainExtent = extent;

				createSwaChainImageView(&_swapChain);

				return _swapChain;
			}

			void VulkanGraphicsPrimitiveManager::createSwaChainImageView(POINTER<VulkanSwapChain> swapchain)
			{
				swapchain->swapChainImageViews.resize(swapchain->swapChainImages.size());

				for (UI32 i = 0; i < swapchain->swapChainImages.size(); i++) {
					ADGRVulkanCreateImageViewInfo info;
					info.image = swapchain->swapChainImages[i];
					info.format = swapchain->swapChainImageFormat;
					info.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					info.mipLevels = 1;

					swapchain->swapChainImageViews.at(i) = VulkanUtilities::createImageView(instance.myCore->logicalDevice, info);
				}
			}

			VulkanVertexBuffer VulkanGraphicsPrimitiveManager::createVertexBuffer(Mesh mesh, const ARRAY<DMKVertexAttribute>& attributes)
			{
				VulkanVertexBuffer _vertexBuffer;

				_vertexBuffer.vertexCount = mesh.vertexDataStore.size();
				VkDeviceSize bufferSize = _vertexBuffer.vertexCount * DMKVertexBufferObjectDescriptor::vertexByteSize(attributes);

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRVulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanUtilities::createBuffer(instance.myCore->logicalDevice, instance.myCore->physicalDevice, bufferInfo);

				void* data = nullptr;
				vkMapMemory(instance.myCore->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				mesh.packData(attributes, data);
				vkUnmapMemory(instance.myCore->logicalDevice, stagingBufferMemory);

				ADGRVulkanCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &_vertexBuffer.buffer;
				vertBufferInfo.bufferMemory = &_vertexBuffer.bufferMemory;

				VulkanUtilities::createBuffer(instance.myCore->logicalDevice, instance.myCore->physicalDevice, vertBufferInfo);

				VulkanUtilities::copyBuffer(instance.myCore->logicalDevice, instance.commandPool, instance.myCore->graphicsQueue, instance.myCore->presentQueue, stagingBuffer, _vertexBuffer.buffer, bufferSize);

				vkDestroyBuffer(instance.myCore->logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(instance.myCore->logicalDevice, stagingBufferMemory, nullptr);

				return _vertexBuffer;
			}

			void VulkanGraphicsPrimitiveManager::destroyVertexBuffer(const VulkanVertexBuffer& buffer)
			{
				VulkanUtilities::terminateBuffers(instance.myCore->logicalDevice, buffer.buffer, buffer.bufferMemory);
			}

			VulkanIndexBuffer VulkanGraphicsPrimitiveManager::createIndexBuffer(Mesh mesh, DMKDataType indexType)
			{
				VulkanIndexBuffer _indexBuffer;

				_indexBuffer.indexCount = mesh.indexes.size();
				VkDeviceSize bufferSize = _indexBuffer.indexCount * (UI32)indexType;

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRVulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanUtilities::createBuffer(instance.myCore->logicalDevice, instance.myCore->physicalDevice, bufferInfo);

				void* data = nullptr;
				vkMapMemory(instance.myCore->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, mesh.indexes.data(), (size_t)bufferSize);
				vkUnmapMemory(instance.myCore->logicalDevice, stagingBufferMemory);

				ADGRVulkanCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &_indexBuffer.buffer;
				vertBufferInfo.bufferMemory = &_indexBuffer.bufferMemory;

				VulkanUtilities::createBuffer(instance.myCore->logicalDevice, instance.myCore->physicalDevice, vertBufferInfo);

				VulkanUtilities::copyBuffer(instance.myCore->logicalDevice, instance.commandPool, instance.myCore->graphicsQueue, instance.myCore->presentQueue, stagingBuffer, _indexBuffer.buffer, bufferSize);

				vkDestroyBuffer(instance.myCore->logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(instance.myCore->logicalDevice, stagingBufferMemory, nullptr);

				return _indexBuffer;
			}

			void VulkanGraphicsPrimitiveManager::destroyIndexBuffer(const VulkanIndexBuffer& buffer)
			{
				VulkanUtilities::terminateBuffers(instance.myCore->logicalDevice, buffer.buffer, buffer.bufferMemory);
			}

			VulkanTextureData VulkanGraphicsPrimitiveManager::createTextureData(ADGRVulkanTextureInitInfo info, Texture texture)
			{
				VulkanTextureData _container;

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

				VulkanUtilities::createBuffer(instance.myCore->logicalDevice, instance.myCore->physicalDevice, bufferInfo);

				void* data;
				if (vkMapMemory(instance.myCore->logicalDevice, stagingBufferMemory, 0, static_cast<size_t>(imageSize), 0, &data) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to map memory!");
				memcpy(data, texture.textureData.get(), static_cast<size_t>(imageSize / 6));
				vkUnmapMemory(instance.myCore->logicalDevice, stagingBufferMemory);

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
				cinfo.arrayLayers = arrayLayers;
				VulkanUtilities::createImage(instance.myCore->logicalDevice, instance.myCore->physicalDevice, cinfo);

				ADGRVulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = _container.image;
				transitionInfo.format = _container.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.mipLevels = info.mipLevels;
				transitionInfo.layerCount = arrayLayers;
				VulkanUtilities::transitionImageLayout(instance.myCore->logicalDevice, instance.commandPool, instance.myCore->graphicsQueue, instance.myCore->presentQueue, transitionInfo);

				ADGRVulkanCopyBufferToImageInfo cpyInfo;
				cpyInfo.buffer = stagingBuffer;
				cpyInfo.image = _container.image;
				cpyInfo.width = _container.width;
				cpyInfo.height = _container.height;
				VulkanUtilities::copyBufferToImage(instance.myCore->logicalDevice, instance.commandPool, instance.myCore->graphicsQueue, instance.myCore->presentQueue, cpyInfo);

				transitionInfo.image = _container.image;
				transitionInfo.format = _container.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				transitionInfo.mipLevels = info.mipLevels;
				transitionInfo.layerCount = arrayLayers;
				VulkanUtilities::transitionImageLayout(instance.myCore->logicalDevice, instance.commandPool, instance.myCore->graphicsQueue, instance.myCore->presentQueue, transitionInfo);

				vkDestroyBuffer(instance.myCore->logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(instance.myCore->logicalDevice, stagingBufferMemory, nullptr);

				if (texture.type != DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP)
					generateMipMaps(&_container);

				ADGRVulkanTextureSamplerInitInfo samplerInitInfo;
				samplerInitInfo.magFilter = info.magFilter;
				samplerInitInfo.minFilter = info.minFilter;
				samplerInitInfo.maxMipLevels = info.maxMipLevels;
				samplerInitInfo.minMipLevels = info.minMipLevels;
				samplerInitInfo.modeU = info.modeU;
				samplerInitInfo.modeV = info.modeV;
				samplerInitInfo.modeW = info.modeW;
				_container.imageSampler = VulkanUtilities::createImageSampler(instance.myCore->logicalDevice, samplerInitInfo);

				ADGRVulkanCreateImageViewInfo cinfo2;
				cinfo2.image = _container.image;
				cinfo2.format = _container.format;
				cinfo2.mipLevels = _container.mipLevels;
				cinfo2.aspectFlags = info.aspectFlags;
				cinfo2.viewType = imageViewType;
				_container.imageView = VulkanUtilities::createImageView(instance.myCore->logicalDevice, cinfo2);

				return _container;
			}

			void VulkanGraphicsPrimitiveManager::generateMipMaps(POINTER<VulkanTextureData> container)
			{
				UI32 _width = container->width, _height = container->height;

				VkFormatProperties formatProperties;
				vkGetPhysicalDeviceFormatProperties(instance.myCore->physicalDevice, container->format, &formatProperties);

				if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
					DMK_CORE_FATAL("texture image format does not support linear blitting!");

				VulkanOneTimeCommandBuffer oneTimeCommandBuffer(instance.myCore->logicalDevice, instance.commandPool, instance.myCore->graphicsQueue, instance.myCore->presentQueue);
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

			void VulkanGraphicsPrimitiveManager::destroyTextureData(const VulkanTextureData& texture)
			{
			}

			VulkanUniformBuffer VulkanGraphicsPrimitiveManager::createUniformBuffer(UI32 bufferSize)
			{
				VulkanUniformBuffer _container;

				ADGRVulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &_container.buffer;
				bufferInfo.bufferMemory = &_container.bufferMemory;
				VulkanUtilities::createBuffer(instance.myCore->logicalDevice, instance.myCore->physicalDevice, bufferInfo);

				return _container;
			}
			
			void VulkanGraphicsPrimitiveManager::updateUniformBuffer(const VulkanUniformBuffer& buffer, UI32 bufferSize, DMKUniformBufferData bufferData)
			{
				VPTR data = nullptr;
				vkMapMemory(instance.myCore->logicalDevice, buffer.bufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, bufferData.data(), bufferSize);
				vkUnmapMemory(instance.myCore->logicalDevice, buffer.bufferMemory);
			}
			
			void VulkanGraphicsPrimitiveManager::destroyUniformBuffer(const VulkanUniformBuffer& buffer)
			{
				VulkanUtilities::terminateBuffers(instance.myCore->logicalDevice, buffer.buffer, buffer.bufferMemory);
			}
			
			VulkanPipeline VulkanGraphicsPrimitiveManager::createPipeline(ADGRVulkanGraphicsPipelineInitInfo info)
			{
				VulkanPipeline _pipeline;

				/* Pipeline layout creation */
				// initialize the pipeline layout
				VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
				pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipelineLayoutInfo.setLayoutCount = info.layouts.size();
				pipelineLayoutInfo.pSetLayouts = info.layouts.data();

				if (info.pushConstantRanges.size())
				{
					pipelineLayoutInfo.pushConstantRangeCount = info.pushConstantRanges.size();
					pipelineLayoutInfo.pPushConstantRanges = info.pushConstantRanges.data();
				}
				else
				{
					pipelineLayoutInfo.pushConstantRangeCount = 0;
					pipelineLayoutInfo.pPushConstantRanges = VK_NULL_HANDLE;
				}

				// create the pipeline layout
				if (vkCreatePipelineLayout(instance.myCore->logicalDevice, &pipelineLayoutInfo, nullptr, &_pipeline.layout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create pipeline layout!");

				/* Pipeline creation */
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
					viewport.width = (F32)info.swapChainExtent.width;
					viewport.height = (F32)info.swapChainExtent.height;
					viewport.minDepth = 0.0f;
					viewport.maxDepth = 1.0f;

					viewports.push_back(viewport);
				}

				// initialize the scissor(s)
				ARRAY<VkRect2D> scissors;
				for (I32 i = 0; i < info.scissorCount; i++) {
					VkRect2D scissor = {};
					scissor.offset = info.offsets[0];
					scissor.extent = info.swapChainExtent;

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
				for (VulkanGraphicsShader shader : info.shaders)
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
				pipelineInfo.layout = _pipeline.layout;
				pipelineInfo.renderPass = info.renderPass;

				if (info.dynamicStateEnable)
					pipelineInfo.pDynamicState = &dynamicStateInfo;

				// create the pipeline
				if (vkCreateGraphicsPipelines(instance.myCore->logicalDevice, info.pipelineCache, 1, &pipelineInfo, nullptr, &_pipeline.pipeline) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create graphics pipeline!");

				return _pipeline;
			}
			
			void VulkanGraphicsPrimitiveManager::drstroyPipeline(const VulkanPipeline& pipeline)
			{
				vkDestroyPipeline(instance.myCore->logicalDevice, pipeline.pipeline, nullptr);
				vkDestroyPipelineLayout(instance.myCore->logicalDevice, pipeline.layout, nullptr);
			}
		}
	}
}