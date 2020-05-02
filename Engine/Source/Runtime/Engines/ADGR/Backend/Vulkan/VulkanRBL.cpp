/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 VulkanRBL.cpp file
*/

#include "dmkafx.h"
#include "VulkanRBL.h"
#include "Platform/windows.h"

#include "Renderer Backend Layer/VulkanReflectObject.h"
#include "Renderer Backend Layer/VulkanPBRObject.h"

#include "Renderer Backend Layer/Compute/VulkanComputeCore.h"

#include "Renderer Backend Layer/Common/VulkanUtilities.h"

#define TEXTOVERLAY_MAX_CHAR_COUNT 2048

/* IDEAS
 Make one time compute objects so that we can compute something at a given instance
 Make multiple of those to compute multiple things.
*/

namespace Dynamik {
	namespace ADGR {
#ifdef DMK_DEBUG
		Debugger::benchmark::FPS myFPSCal;

		// ----------
#endif
		void VulkanRBL::setMsaaSamples(DMKPipelineMSAASamples samples)
		{
		}
		
		void VulkanRBL::setWindowHandle(POINTER<GLFWwindow> windowHandle)
		{
			myWindowHandle = windowHandle;
		}

		void VulkanRBL::setWindowExtent(UI32 width, UI32 height)
		{
			windowWidth = width;
			windowHeight = height;
		}

		void VulkanRBL::setProgress(POINTER<UI32> progress)
		{
			myProgress = progress;
		}

		void VulkanRBL::initializeGraphicsCore()
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Initialize the instance */
			ADGRVulkanInstanceInitInfo instanceInitInfo;
			instanceInitInfo.applicationName = "Dynamik Engine";
			instanceInitInfo.engineName = "Dynamik";
			myGraphicsCore.initializeInstance(instanceInitInfo);

			/* Initialize the window surface */
			myGraphicsCore.initializeSurface(myWindowHandle);

			/* Initialize the device (physical and logical) */
			myGraphicsCore.initializeDevice();

			/* Initialize command pools (both host visible and client visible) */
			ADGRVulkanGraphicsCommandBufferInitResources commandBufferResourceInitInfo;
			commandBufferResourceInitInfo.logicalDevice = myGraphicsCore.logicalDevice;
			commandBufferResourceInitInfo.physicalDevice = myGraphicsCore.physicalDevice;
			commandBufferResourceInitInfo.surface = myGraphicsCore.surface;
			myResourceContainers[0].commandBuffer.initializeResources(commandBufferResourceInitInfo);
			myResourceContainers[0].commandBuffer.initializeCommandPool();
			myResourceContainers[1].commandBuffer.initializeResources(commandBufferResourceInitInfo);
			myResourceContainers[1].commandBuffer.initializeCommandPool();

			/* Initialize the SwapChain */
			mySwapChain.basicInitialize(
				myGraphicsCore.logicalDevice,
				myGraphicsCore.physicalDevice,
				myGraphicsCore.surface,
				myGraphicsCore.surfaceCapabilities);
			mySwapChain.initializeSwapChain(windowWidth, windowHeight);

			/* Initialize Render pass */
			_initializeRenderPass();

			/* Initialize attachments */
			/* Initialize color buffer */
			myColorBuffer.initialize(
				myGraphicsCore.logicalDevice,
				myGraphicsCore.physicalDevice,
				myResourceContainers[inUseIndex].commandBuffer.pool,
				myGraphicsCore.graphicsQueue,
				myGraphicsCore.presentQueue,
				mySwapChain.swapChainContainer.swapChainImageFormat,
				mySwapChain.swapChainContainer.swapChainExtent,
				myGraphicsCore.msaaSamples);

			/* Initialize color buffer */
			myDepthBuffer.initialize(
				myGraphicsCore.logicalDevice,
				myGraphicsCore.physicalDevice,
				myResourceContainers[inUseIndex].commandBuffer.pool,
				myGraphicsCore.graphicsQueue,
				myGraphicsCore.presentQueue,
				mySwapChain.swapChainContainer.swapChainExtent,
				myGraphicsCore.msaaSamples);

			/* Initialize Frame Buffer */
			ADGRVulkanGraphicsFrameBufferInitInfo frameBufferInitInfo;
			frameBufferInitInfo.attachments.pushBack(myColorBuffer.imageView);
			frameBufferInitInfo.attachments.pushBack(myDepthBuffer.imageView);
			frameBufferInitInfo.swapChainImageViews = mySwapChain.swapChainContainer.swapChainImageViews;
			frameBufferInitInfo.bufferCount = mySwapChain.swapChainContainer.swapChainImages.size();
			frameBufferInitInfo.swapChainExtent = mySwapChain.swapChainContainer.swapChainExtent;
			myFrameBuffer.initializeFrameBuffer(myGraphicsCore.logicalDevice, frameBufferInitInfo);

			/* Initialize the overlay */
			/* TODO: Overlay */
		}

		void VulkanRBL::initializeComputeCore()
		{
		}

		void VulkanRBL::addObject(POINTER<InternalFormat> format)
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
				myResourceContainers[inUseIndex].renderData.pushBack(_initializeStaticObject(format));
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
				myResourceContainers[inUseIndex].renderData.pushBack(_initializeSkyBox(format));
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

		void VulkanRBL::addObjects(ARRAY<POINTER<InternalFormat>> formats)
		{
			for (auto format : formats)
				if (format.isValid())
					addObject(format);
		}

		void VulkanRBL::initializeCommands()
		{
			DMK_BEGIN_PROFILE_TIMER();

			ADGRVulkanGraphicsCommandBufferInitInfo initInfo;
			initInfo.count = mySwapChain.swapChainContainer.swapChainImages.size();
			initInfo.frameBuffer = myFrameBuffer;
			initInfo.swapChain = mySwapChain.swapChainContainer;
			initInfo.objects = myResourceContainers[inUseIndex].renderData;

			myResourceContainers[inUseIndex].commandBuffer.initializeCommandBuffers(initInfo);
		}

		void VulkanRBL::initializeFinalComponents()
		{
			DMK_BEGIN_PROFILE_TIMER();

			myGraphicsCore.initializeSyncObjects();
		}

		void VulkanRBL::drawFrame(DMKRendererDrawFrameInfo info)
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Check if the number of update objects are equal to the in-flight objects */
			if (info.formats.size() != myResourceContainers[inUseIndex].renderData.size())
				DMK_CORE_FATAL("Invalid amount of update formats sent to the Draw call!");

			/* Sync Vulkan Fences */
			myGraphicsCore.syncFence(currentFrame);

			/* Get the current image index */
			imageIndex = 0;
			result = myGraphicsCore.getNextImage(mySwapChain.swapChainContainer.swapChain, &imageIndex, currentFrame);

			/* Check if any errors were encountered */
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
				DMK_CORE_FATAL("Failed to acquire Swap Chain image!");

			/* Update the objects using the Draw Frame Info structure */
			for (UI32 index = 0; index < info.formats.size(); index++)
			{
				DMK_BEGIN_PROFILE_TIMER();

				/* Check for the Uniform buffer attributes and add the data to the container */
				for (UI32 _itr = 0; _itr < info.formats[index]->descriptor.uniformBufferObjectDescriptions.size(); _itr++)
				{
					DMK_BEGIN_PROFILE_TIMER();

					/* Currently the Vulkan RBL supports vertex shader uniform buffer updation only */
					DMKUniformBufferObjectDescriptor _description = info.formats[index]->descriptor.uniformBufferObjectDescriptions[_itr];
					if (_description.location != DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX)
						continue;

					/* Update the objects uniform buffer memory */
					ADGRVulkanUnformBufferContainer _container = myResourceContainers[inUseIndex].renderData[index].uniformBufferContainers[_itr];
					VulkanUtilities::updateUniformBuffer(myGraphicsCore.logicalDevice, info.formats[index]->onUpdate(info.cameraData), _container.bufferMemories[imageIndex], _description);
				}
			}

			/* Submit queues */
			result = myGraphicsCore.submitQueues(
				{ mySwapChain.swapChainContainer.swapChain },
				imageIndex, currentFrame,
				{ myResourceContainers[inUseIndex].commandBuffer.buffers[imageIndex] });

			/* Check for any errors */
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				recreateSwapChain();
			}
			else if (result != VK_SUCCESS)
				DMK_CORE_FATAL("failed to present swap chain image!");

			/* Update the current frame number */
			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		}

		void VulkanRBL::recreateSwapChain()
		{
		}

		void VulkanRBL::_initializeRenderPass()
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

		void VulkanRBL::_prepareRenderDataContainer(UI32 index)
		{
			myResourceContainers[index].commandBuffer.terminateCommandBuffers();
			myResourceContainers[index].renderData = {};
		}

		ADGRVulkanGraphicsRenderableObjectInitInfo VulkanRBL::_getBasicInitInfo()
		{
			ADGRVulkanGraphicsRenderableObjectInitInfo _info;
			_info.logicalDevice = myGraphicsCore.logicalDevice;
			_info.physicalDevice = myGraphicsCore.physicalDevice;
			_info.commandPool = myResourceContainers[inUseIndex].commandBuffer.pool;
			_info.graphicsQueue = myGraphicsCore.graphicsQueue;
			_info.presentQueue = myGraphicsCore.presentQueue;

			return _info;
		}

		ADGRVulkanRenderData VulkanRBL::_initializeStaticObject(POINTER<InternalFormat> format)
		{
			VulkanGraphicsRenderableObject _renderObject(_getBasicInitInfo());
			_renderObject.setInternalFormat(format);

			_renderObject.setSwapChainContainer(&mySwapChain.swapChainContainer);
			_renderObject.setFrameBufferContainer(&myFrameBuffer);

			return _renderObject.initializeObject(format, myGraphicsCore.msaaSamples);
		}

		ADGRVulkanRenderData VulkanRBL::_initializeSkyBox(POINTER<InternalFormat> format)
		{
			DMK_BEGIN_PROFILE_TIMER();

			VulkanSkyBox _renderObject(_getBasicInitInfo());
			_renderObject.setInternalFormat(format);

			_renderObject.setSwapChainContainer(&mySwapChain.swapChainContainer);
			_renderObject.setFrameBufferContainer(&myFrameBuffer);

			return _renderObject.initializeObject(format, myGraphicsCore.msaaSamples);
		}

		ADGRVulkanRenderData VulkanRBL::_initializeSkeletalAnimation(POINTER<InternalFormat> format)
		{
			VulkanSkeletalAnimation* _renderObject = Cast<VulkanSkeletalAnimation*>(format.get());

			return ADGRVulkanRenderData();
		}
	}
}