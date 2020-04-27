/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 vulkanRenderer.cpp file
*/

#include "dmkafx.h"
#include "vulkanRenderer.h"
#include "Platform/windows.h"

#include "Renderer Backend Layer/VulkanReflectObject.h"
#include "Renderer Backend Layer/VulkanPBRObject.h"

#include "Renderer Backend Layer/Compute/VulkanComputeCore.h"

#include "Renderer Backend Layer/Common/VulkanUtilities.h"

#define TEXTOVERLAY_MAX_CHAR_COUNT 2048

/* IDEAS
 Make one time compute objects so that we can compute something at a given instance.
 Make multiple of those to compute multiple things.
*/

namespace Dynamik {
	namespace ADGR {
#ifdef DMK_DEBUG
		Debugger::benchmark::FPS myFPSCal;

		// ----------
#endif
		vulkanRenderer vulkanRenderer::instance;

		void vulkanRenderer::setWindowHandle(POINTER<GLFWwindow> windowHandle)
		{
			instance.myWindowHandle = windowHandle;
		}

		void vulkanRenderer::setWindowExtent(UI32 width, UI32 height)
		{
			instance.windowWidth = width;
			instance.windowHeight = height;
		}

		void vulkanRenderer::setProgress(POINTER<UI32> progress)
		{
			instance.myProgress = progress;
		}

		void vulkanRenderer::initializeGraphicsCore()
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Initialize the instance */
			ADGRVulkanInstanceInitInfo instanceInitInfo;
			instanceInitInfo.applicationName = "Dynamik Engine";
			instanceInitInfo.engineName = "Dynamik";
			instance.myGraphicsCore.initializeInstance(instanceInitInfo);

			/* Initialize the window surface */
			instance.myGraphicsCore.initializeSurface(instance.myWindowHandle);

			/* Initialize the device (physical and logical) */
			instance.myGraphicsCore.initializeDevice();

			/* Initialize command pools (both host visible and client visible) */
			ADGRVulkanGraphicsCommandBufferInitResources commandBufferResourceInitInfo;
			commandBufferResourceInitInfo.logicalDevice = instance.myGraphicsCore.logicalDevice;
			commandBufferResourceInitInfo.physicalDevice = instance.myGraphicsCore.physicalDevice;
			commandBufferResourceInitInfo.surface = instance.myGraphicsCore.surface;
			instance.myResourceContainers[0].commandBuffer.initializeResources(commandBufferResourceInitInfo);
			instance.myResourceContainers[0].commandBuffer.initializeCommandPool();
			instance.myResourceContainers[1].commandBuffer.initializeResources(commandBufferResourceInitInfo);
			instance.myResourceContainers[1].commandBuffer.initializeCommandPool();

			/* Initialize the SwapChain */
			instance.mySwapChain.basicInitialize(
				instance.myGraphicsCore.logicalDevice,
				instance.myGraphicsCore.physicalDevice,
				instance.myGraphicsCore.surface,
				instance.myGraphicsCore.surfaceCapabilities);
			instance.mySwapChain.initializeSwapChain(instance.windowWidth, instance.windowHeight);

			/* Initialize Render pass */
			instance._initializeRenderPass();

			/* Initialize attachments */
			/* Initialize color buffer */
			instance.myColorBuffer.initialize(
				instance.myGraphicsCore.logicalDevice,
				instance.myGraphicsCore.physicalDevice,
				instance.myResourceContainers[instance.inUseIndex].commandBuffer.pool,
				instance.myGraphicsCore.graphicsQueue,
				instance.myGraphicsCore.presentQueue,
				instance.mySwapChain.swapChainContainer.swapChainImageFormat,
				instance.mySwapChain.swapChainContainer.swapChainExtent,
				instance.myGraphicsCore.msaaSamples);

			/* Initialize color buffer */
			instance.myDepthBuffer.initialize(
				instance.myGraphicsCore.logicalDevice,
				instance.myGraphicsCore.physicalDevice,
				instance.myResourceContainers[instance.inUseIndex].commandBuffer.pool,
				instance.myGraphicsCore.graphicsQueue,
				instance.myGraphicsCore.presentQueue,
				instance.mySwapChain.swapChainContainer.swapChainExtent,
				instance.myGraphicsCore.msaaSamples);

			/* Initialize Frame Buffer */
			ADGRVulkanGraphicsFrameBufferInitInfo frameBufferInitInfo;
			frameBufferInitInfo.attachments.pushBack(instance.myColorBuffer.imageView);
			frameBufferInitInfo.attachments.pushBack(instance.myDepthBuffer.imageView);
			frameBufferInitInfo.swapChainImageViews = instance.mySwapChain.swapChainContainer.swapChainImageViews;
			frameBufferInitInfo.bufferCount = instance.mySwapChain.swapChainContainer.swapChainImages.size();
			frameBufferInitInfo.swapChainExtent = instance.mySwapChain.swapChainContainer.swapChainExtent;
			instance.myFrameBuffer.initializeFrameBuffer(instance.myGraphicsCore.logicalDevice, frameBufferInitInfo);

			/* Initialize the overlay */
			/* TODO: Overlay */
		}

		void vulkanRenderer::initializeComputeCore()
		{
		}

		void vulkanRenderer::addObject(POINTER<InternalFormat> format)
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Check the object type of the format and initialize it accordingly */
			switch (format->type)
			{
				/* 2D image initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_IMAGE_2D:
				break;

				/* Debug object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT:
				break;

				/* Mesh object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_MESH:
				break;

				/* Static object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC:
				instance.myResourceContainers[instance.inUseIndex].renderData.pushBack(instance._initializeStaticObject(format));
				break;

				/* Interavtive object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT:
				break;

				/* Player object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PLAYER:
				break;

				/* Non Player Character object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_NPC:
				break;

				/* Texture UI object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXTURE_UI:
				break;

				/* SkyBox object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKYBOX:
				instance.myResourceContainers[instance.inUseIndex].renderData.pushBack(instance._initializeSkyBox(format));
				break;

				/* Sprite sheet object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SPRITES:
				break;

				/* Font sheet object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_FONT:
				break;

				/* Text overlay initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXT_OVERLAY:
				break;

				/* Particle system initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PARTICLE:
				break;

				/* Destruction entity initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTION:
				break;

				/* Destructor entity initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTOR:
				break;

				/* Skeletal animation object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKELETAL_ANIMATION:
				break;

				/* Light object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_LIGHT:
				break;

				/* Camera object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_CAMERA:
				break;
			default:
				DMK_CORE_FATAL("Invalid Object Type!");
				break;
			}
		}

		void vulkanRenderer::addObjects(ARRAY<POINTER<InternalFormat>> formats)
		{
			for (auto format : formats)
				if (format.isValid())
					addObject(format);
		}

		void vulkanRenderer::initializeVertexBuffers(POINTER<InternalFormat> format)
		{

		}

		void vulkanRenderer::initializeIndexBuffers(POINTER<InternalFormat> format)
		{
		}

		void vulkanRenderer::initializeTextures(POINTER<InternalFormat> format)
		{
		}

		void vulkanRenderer::initializeUniformBuffers(POINTER<InternalFormat> format)
		{
		}

		void vulkanRenderer::initializePipeline(POINTER<InternalFormat> format)
		{
		}

		void vulkanRenderer::initializeCommands()
		{
			DMK_BEGIN_PROFILE_TIMER();

			ADGRVulkanGraphicsCommandBufferInitInfo initInfo;
			initInfo.count = instance.mySwapChain.swapChainContainer.swapChainImages.size();
			initInfo.frameBuffer = instance.myFrameBuffer;
			initInfo.swapChain = instance.mySwapChain.swapChainContainer;
			initInfo.objects = instance.myResourceContainers[instance.inUseIndex].renderData;

			instance.myResourceContainers[instance.inUseIndex].commandBuffer.initializeCommandBuffers(initInfo);
		}

		void vulkanRenderer::initializeFinalComponents()
		{
			DMK_BEGIN_PROFILE_TIMER();

			instance.myGraphicsCore.initializeSyncObjects();
		}

		void vulkanRenderer::drawFrame(DMKRendererDrawFrameInfo info)
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Check if the number of update objects are equal to the in-flight objects */
			if (info.formats.size() != instance.myResourceContainers[instance.inUseIndex].renderData.size())
				DMK_CORE_FATAL("Invalid amount of update formats sent to the Draw call!");

			/* Sync Vulkan Fences */
			instance.myGraphicsCore.syncFence(instance.currentFrame);

			/* Get the current image index */
			instance.imageIndex = 0;
			instance.result = instance.myGraphicsCore.getNextImage(instance.mySwapChain.swapChainContainer.swapChain, &instance.imageIndex, instance.currentFrame);

			/* Check if any errors were encountered */
			if (instance.result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				recreateSwapChain();
				return;
			}
			else if (instance.result != VK_SUCCESS && instance.result != VK_SUBOPTIMAL_KHR)
				DMK_CORE_FATAL("Failed to acquire Swap Chain image!");

			/* Update the objects using the Draw Frame Info structure */
			for (UI32 index = 0; index < info.formats.size(); index++)
			{
				DMK_BEGIN_PROFILE_TIMER();

				/* Check for the Uniform buffer attributes and add the data to the container */
				for (auto _description : info.formats[index]->descriptor.uniformBufferObjectDescriptions)
				{
					DMK_BEGIN_PROFILE_TIMER();

					/* Currently the Vulkan RBL supports vertex shader uniform buffer updation only */
					if (_description.location != DMKAttributeLocation::DMK_ATTRIBUTE_LOCATION_VERTEX)
						continue;

					/* Update the objects uniform buffer memory */
					for (auto _container : instance.myResourceContainers[instance.inUseIndex].renderData[index].uniformBufferContainers)
						VulkanUtilities::updateUniformBuffer(instance.myGraphicsCore.logicalDevice, info.formats[index]->onUpdate(info.cameraData), _container.bufferMemories[instance.imageIndex], _description);
				}
			}

			/* Submit queues */
			instance.result = instance.myGraphicsCore.submitQueues(
				{ instance.mySwapChain.swapChainContainer.swapChain },
				instance.imageIndex, instance.currentFrame,
				{ instance.myResourceContainers[instance.inUseIndex].commandBuffer.buffers[instance.imageIndex] });

			/* Check for any errors */
			if (instance.result == VK_ERROR_OUT_OF_DATE_KHR || instance.result == VK_SUBOPTIMAL_KHR)
			{
				recreateSwapChain();
			}
			else if (instance.result != VK_SUCCESS)
				DMK_CORE_FATAL("failed to present swap chain image!");

			/* Update the current frame number */
			instance.currentFrame = (instance.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		}

		void vulkanRenderer::recreateSwapChain()
		{
		}

		void vulkanRenderer::_initializeRenderPass()
		{
			DMK_BEGIN_PROFILE_TIMER();

			ARRAY<VkAttachmentDescription> attachments;

			// attachment descriptions
			VkAttachmentDescription colorAttachment = {};
			colorAttachment.format = mySwapChain.swapChainContainer.swapChainImageFormat;
			colorAttachment.samples = myGraphicsCore.msaaSamples;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			attachments.push_back(colorAttachment);

			VkAttachmentReference colorAttachmentRef = {};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			// sub passes
			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			VkAttachmentDescription depthAttachment = {};
			depthAttachment.format = VulkanUtilities::findDepthFormat(myGraphicsCore.physicalDevice);
			depthAttachment.samples = myGraphicsCore.msaaSamples;
			depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			attachments.push_back(depthAttachment);

			VkAttachmentReference depthAttachmentRef = {};
			depthAttachmentRef.attachment = 1;
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			subpass.pDepthStencilAttachment = &depthAttachmentRef;

			VkAttachmentDescription colorAttachmentResolve = {};
			colorAttachmentResolve.format = mySwapChain.swapChainContainer.swapChainImageFormat;
			colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
			colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			attachments.push_back(colorAttachmentResolve);

			VkAttachmentReference colorAttachmentResolveRef = {};
			colorAttachmentResolveRef.attachment = 2;
			colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			subpass.pResolveAttachments = &colorAttachmentResolveRef;

			ARRAY<VkSubpassDescription> subPasses;
			subPasses.push_back(subpass);

			ADGRVulkanRenderPassInitInfo renderPassInitInfo;
			renderPassInitInfo.attachments = attachments;
			renderPassInitInfo.subPasses = subPasses;
			myFrameBuffer.initializeRenderPass(myGraphicsCore.logicalDevice, renderPassInitInfo);
		}

		void vulkanRenderer::_prepareRenderDataContainer(UI32 index)
		{
			myResourceContainers[index].commandBuffer.terminateCommandBuffers();
			myResourceContainers[index].renderData = {};
		}

		ADGRVulkanGraphicsRenderableObjectInitInfo vulkanRenderer::_getBasicInitInfo()
		{
			ADGRVulkanGraphicsRenderableObjectInitInfo _info;
			_info.logicalDevice = myGraphicsCore.logicalDevice;
			_info.physicalDevice = myGraphicsCore.physicalDevice;
			_info.commandPool = myResourceContainers[inUseIndex].commandBuffer.pool;
			_info.graphicsQueue = myGraphicsCore.graphicsQueue;
			_info.presentQueue = myGraphicsCore.presentQueue;

			return _info;
		}

		ADGRVulkanRenderData vulkanRenderer::_initializeStaticObject(POINTER<InternalFormat> format)
		{
			VulkanGraphicsRenderableObject _renderObject(_getBasicInitInfo());
			_renderObject.setInternalFormat(format);

			_renderObject.setSwapChainContainer(&mySwapChain.swapChainContainer);
			_renderObject.setFrameBufferContainer(&myFrameBuffer);

			return _renderObject.initializeObject(format, myGraphicsCore.msaaSamples);
		}

		ADGRVulkanRenderData vulkanRenderer::_initializeSkyBox(POINTER<InternalFormat> format)
		{
			DMK_BEGIN_PROFILE_TIMER();

			VulkanSkyBox _renderObject(_getBasicInitInfo());
			_renderObject.setInternalFormat(format);

			_renderObject.setSwapChainContainer(&mySwapChain.swapChainContainer);
			_renderObject.setFrameBufferContainer(&myFrameBuffer);

			return _renderObject.initializeObject(format, myGraphicsCore.msaaSamples);
		}

		ADGRVulkanRenderData vulkanRenderer::_initializeSkeletalAnimation(POINTER<InternalFormat> format)
		{
			VulkanSkeletalAnimation* _renderObject = Cast<VulkanSkeletalAnimation*>(format.get());

			return ADGRVulkanRenderData();
		}
	}
}