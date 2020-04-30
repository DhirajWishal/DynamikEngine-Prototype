#include "dmkafx.h"
#include "VulkanRBL.h"

#include "Common/VulkanUtilities.h"
#include "VulkanPresets.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanRBL::basicInternalInitialization()
			{
				VulkanGraphicsPrimitiveManager::setCoreData(&instance.myGraphicsCore, VkCommandPool());
			}

			void VulkanRBL::setWindowHandle(POINTER<GLFWwindow> handle)
			{
				instance.myWindowHandle = handle;
			}

			void VulkanRBL::setWindowExtent(UI32 width, UI32 height)
			{
				instance.windowExtent.width = width;
				instance.windowExtent.height = height;
			}

			void VulkanRBL::setProgressPointer(POINTER<UI32> progress)
			{
				instance.myProgressPointer = progress;
			}

			void VulkanRBL::initializeInstance()
			{
				ADGRVulkanInstanceInitInfo initInfo;
				initInfo.applicationName = "Dynamik Engine";
				initInfo.engineName = "Dynamik";
				instance.myGraphicsCore.initializeInstance(initInfo);
				instance.myGraphicsCore.initializeSurface(instance.myWindowHandle);
			}

			void VulkanRBL::initializeDevices()
			{
				instance.myGraphicsCore.initializeDevice();
			}

			void VulkanRBL::initializeStageOne()
			{
				initializeInstance();
				initializeDevices();

				/* Initialize command buffer resources */
				ADGRVulkanGraphicsCommandBufferInitResources commandBufferInitResource;
				commandBufferInitResource.logicalDevice = instance.myGraphicsCore.logicalDevice;
				commandBufferInitResource.physicalDevice = instance.myGraphicsCore.physicalDevice;
				commandBufferInitResource.surface = instance.myGraphicsCore.surface;
				instance.submitPendingCommandBuffer.initializeResources(commandBufferInitResource);
				instance.inFlightCommandBuffer.initializeResources(commandBufferInitResource);

				/* Initialize command pools */
				instance.submitPendingCommandBuffer.initializeCommandPool();
				instance.inFlightCommandBuffer.initializeCommandPool();
			}

			POINTER<ColorAttachment> VulkanRBL::generateColorAttachment(POINTER<VulkanSwapChain> swapChain)
			{
				VulkanTextureData _textureData;
				VkFormat colorFormat = swapChain->swapChainImageFormat;

				ADGRVulkanCreateImageInfo cinfo;
				cinfo.width = swapChain->swapChainExtent.width;
				cinfo.height = swapChain->swapChainExtent.height;
				cinfo.format = colorFormat;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &_textureData.image;
				cinfo.imageMemory = &_textureData.imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = instance.myGraphicsCore.msaaSamples;
				cinfo.flags = NULL;
				VulkanUtilities::createImage(instance.myGraphicsCore.logicalDevice, instance.myGraphicsCore.physicalDevice, cinfo);

				ADGRVulkanCreateImageViewInfo viewInfo;
				viewInfo.image = _textureData.image;
				viewInfo.format = colorFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.mipLevels = 1;
				_textureData.imageView = VulkanUtilities::createImageView(instance.myGraphicsCore.logicalDevice, viewInfo);

				ADGRVulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = _textureData.image;
				transitionInfo.format = colorFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				transitionInfo.layerCount = 1;
				VulkanUtilities::transitionImageLayout(instance.myGraphicsCore.logicalDevice, instance.submitPendingCommandBuffer.pool, instance.myGraphicsCore.graphicsQueue, instance.myGraphicsCore.presentQueue, transitionInfo);

				auto _colorAttachment = StaticAllocator<ColorAttachment>::allocate();
				_colorAttachment->image = StaticAllocator<VulkanTextureData>::allocate();
				StaticAllocator<VulkanTextureData>::set(_colorAttachment->image, (VulkanTextureData&&)_textureData);
				return _colorAttachment;
			}

			void VulkanRBL::destroyColorAttachment(POINTER<ColorAttachment> ptr)
			{
				StaticAllocator<ColorAttachment>::deAllocate(ptr);
			}

			POINTER<DepthAttachment> VulkanRBL::generateDepthAttachment()
			{
				VulkanTextureData _textureData;
				VkFormat depthFormat = VulkanUtilities::findDepthFormat(instance.myGraphicsCore.physicalDevice);

				ADGRVulkanCreateImageInfo cinfo;
				cinfo.width = instance.windowExtent.width;
				cinfo.height = instance.windowExtent.height;
				cinfo.format = depthFormat;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &_textureData.image;
				cinfo.imageMemory = &_textureData.imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = instance.myGraphicsCore.msaaSamples;
				cinfo.flags = NULL;

				VulkanUtilities::createImage(instance.myGraphicsCore.logicalDevice, instance.myGraphicsCore.physicalDevice, cinfo);

				ADGRVulkanCreateImageViewInfo viewInfo;
				viewInfo.image = _textureData.image;
				viewInfo.format = depthFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
				viewInfo.mipLevels = 1;

				_textureData.imageView = VulkanUtilities::createImageView(instance.myGraphicsCore.logicalDevice, viewInfo);

				ADGRVulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = _textureData.image;
				transitionInfo.format = depthFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				transitionInfo.layerCount = 2;

				VulkanUtilities::transitionImageLayout(instance.myGraphicsCore.logicalDevice, instance.submitPendingCommandBuffer.pool, instance.myGraphicsCore.graphicsQueue, instance.myGraphicsCore.presentQueue, transitionInfo);

				auto _depthAttachment = StaticAllocator<DepthAttachment>::allocate();
				_depthAttachment->image = StaticAllocator<VulkanTextureData>::allocate();
				StaticAllocator<VulkanTextureData>::set(_depthAttachment->image, (VulkanTextureData&&)_textureData);
				return _depthAttachment;
			}

			void VulkanRBL::destroyDepthAttachment(POINTER<DepthAttachment> ptr)
			{
				StaticAllocator<DepthAttachment>::deAllocate(ptr);
			}

			POINTER<VulkanSwapChain> VulkanRBL::initializeSwapChain()
			{
				auto _swapChain = StaticAllocator<VulkanSwapChain>::allocate();
				StaticAllocator<VulkanSwapChain>::set(_swapChain, VulkanGraphicsPrimitiveManager::createSwapChain(instance.windowExtent.width, instance.windowExtent.height));
				return _swapChain;
			}

			void VulkanRBL::terminateSwapChain(POINTER<VulkanSwapChain> swapChain)
			{
				StaticAllocator<VulkanSwapChain>::deAllocate(swapChain);
			}

			POINTER<VulkanRenderPass> VulkanRBL::initializeRenderPass()
			{
				auto _renderPass = StaticAllocator<VulkanRenderPass>::allocate();
				StaticAllocator<VulkanRenderPass>::set(_renderPass, VulkanGraphicsPrimitiveManager::createRenderPass(VulkanPresets::renderPassPreset3D(VkFormat(), VkFormat(), VK_SAMPLE_COUNT_1_BIT)));
				return _renderPass;
			}

			void VulkanRBL::terminateRenderPass(POINTER<VulkanRenderPass> renderPass)
			{
				StaticAllocator<VulkanRenderPass>::deAllocate(renderPass);
			}

			ARRAY<POINTER<VulkanFrameBuffer>> VulkanRBL::initializeFrameBuffers(POINTER<VulkanSwapChain> swapChain, ARRAY<POINTER<RenderAttachment>> attachments)
			{
				ADGRVulkanGraphicsFrameBufferInitInfo initInfo;
				for (auto _attachment : attachments)
				{
					POINTER<InternalAttachment> _imageAttachment = _attachment;
					initInfo.attachments.pushBack(_imageAttachment->image);
				}
				initInfo.swapChainImageViews = swapChain->swapChainImageViews;
				initInfo.bufferCount = swapChain->swapChainImages.size();
				initInfo.swapChainExtent = swapChain->swapChainExtent;
				auto _frameBuffers = VulkanGraphicsPrimitiveManager::createFrameBuffers(initInfo);

				ARRAY<POINTER<VulkanFrameBuffer>> _containers;
				for (auto _buffer : _frameBuffers)
				{
					POINTER<VulkanFrameBuffer> _frameBuffer = StaticAllocator<VulkanFrameBuffer>::allocate();
					StaticAllocator<VulkanFrameBuffer>::set(_frameBuffer, (VulkanFrameBuffer&&)_buffer);
					_containers.pushBack(_frameBuffer);
				}

				return _containers;
			}

			void VulkanRBL::terminateFrameBuffers(ARRAY<POINTER<FrameBuffer>> frameBuffers)
			{
				for (auto buffer : frameBuffers)
				{
					StaticAllocator<VulkanFrameBuffer>::deAllocate(buffer);
				}
			}

			RenderContext VulkanRBL::createContext(RenderContextType type, ARRAY<POINTER<RenderAttachment>> attachments)
			{
				RenderContext _context;
				_context.type = type;

				_context.swapChain = initializeSwapChain().get();
				_context.renderPass = initializeRenderPass().get();

				auto _buffers = initializeFrameBuffers(_context.swapChain, attachments);
				for (auto _buffer : _buffers)
					_context.frameBuffers.pushBack(_buffer.get());

				return _context;
			}

			void VulkanRBL::destroyContext(RenderContext context)
			{
				terminateRenderPass(context.renderPass);
				terminateSwapChain(context.swapChain);
				terminateFrameBuffers(context.frameBuffers);
			}

			POINTER<VulkanVertexBuffer> VulkanRBL::initializeVertexBuffer(const Mesh& mesh, ARRAY<DMKVertexAttribute> attributes)
			{
				auto _vertexBuffer = StaticAllocator<VulkanVertexBuffer>::allocate();
				StaticAllocator<VulkanVertexBuffer>::set(_vertexBuffer, VulkanGraphicsPrimitiveManager::createVertexBuffer(mesh, attributes));
				return _vertexBuffer;
			}

			void VulkanRBL::terminateVertexBuffer(POINTER<VulkanVertexBuffer> buffer)
			{
				StaticAllocator<VulkanVertexBuffer>::deAllocate(buffer);
			}

			POINTER<VulkanIndexBuffer> VulkanRBL::initializeIndexBuffer(const Mesh& mesh, DMKDataType indexType)
			{
				auto _indexBuffer = StaticAllocator<VulkanIndexBuffer>::allocate();
				StaticAllocator<VulkanIndexBuffer>::set(_indexBuffer, VulkanGraphicsPrimitiveManager::createIndexBuffer(mesh, indexType));
				return _indexBuffer;
			}

			void VulkanRBL::terminateIndexBuffer(POINTER<VulkanIndexBuffer> buffer)
			{
				StaticAllocator<VulkanIndexBuffer>::deAllocate(buffer);
			}

			POINTER<VulkanTextureData> VulkanRBL::initializeTextureData(const Texture& texture)
			{
				ADGRVulkanTextureInitInfo info;
				info.mipLevels = 1;
				info.minMipLevels = 0;
				info.maxMipLevels = 1;
				info.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				info.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				info.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				info.magFilter = VK_FILTER_LINEAR;
				info.minFilter = VK_FILTER_LINEAR;
				info.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

				auto _textureData = StaticAllocator<VulkanTextureData>::allocate();
				StaticAllocator<VulkanTextureData>::set(_textureData, VulkanGraphicsPrimitiveManager::createTextureData(info, texture));
				return _textureData;
			}

			void VulkanRBL::terminateTextureData(POINTER<VulkanTextureData> texture)
			{
				StaticAllocator<VulkanTextureData>::deAllocate(texture);
			}

			VulkanUniformBuffer VulkanRBL::initializeUniformBuffer(UI32 size)
			{
				return VulkanGraphicsPrimitiveManager::createUniformBuffer(size);
			}

			void VulkanRBL::terminateUniformBuffer(POINTER<VulkanUniformBuffer> buffer)
			{
				StaticAllocator<VulkanUniformBuffer>::deAllocate(buffer);
			}

			ARRAY<VulkanUniformBuffer> VulkanRBL::initializeUniformBuffers(UI32 size)
			{
				return ARRAY<VulkanUniformBuffer>();
			}

			void VulkanRBL::terminateUniformBuffers(ARRAY<POINTER<VulkanUniformBuffer>> buffers)
			{
				for (auto buffer : buffers)
				{
					StaticAllocator<VulkanUniformBuffer>::deAllocate(buffer);
				}
			}

			VulkanPipeline VulkanRBL::initializePipeline(ADGRVulkanGraphicsPipelineInitInfo info)
			{
				return VulkanGraphicsPrimitiveManager::createPipeline(info);
			}

			void VulkanRBL::terminatePipeline(POINTER<VulkanPipeline> pipeline)
			{
				StaticAllocator<VulkanPipeline>::deAllocate(pipeline);
			}

			void VulkanRBL::addRenderableToQueue(POINTER<InternalFormat> format)
			{
			}
		}
	}
}