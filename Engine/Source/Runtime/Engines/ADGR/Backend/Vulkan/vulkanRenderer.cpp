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

#include "debugger.h"

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
            /* Check the object type of the format and initialize it accordingly */
            switch (format->type)
            {
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_IMAGE_2D:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_MESH:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PLAYER:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_NPC:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXTURE_UI:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKYBOX:
                instance._initializeSkyBox(format);
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SPRITES:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_FONT:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXT_OVERLAY:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PARTICLE:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTION:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTOR:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKELETAL_ANIMATION:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_LIGHT:
                break;
            case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_CAMERA:
                break;
            default:
                break;
            }
        }

        void vulkanRenderer::addObjects(ARRAY<POINTER<InternalFormat>> formats)
        {
            for (auto format : formats)
                addObject(format);
        }

        void vulkanRenderer::drawFrame(DMKRendererDrawFrameInfo info)
        {
        }
        
        void vulkanRenderer::_initializeRenderPass()
        {
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
        
        ADGRVulkanRenderData vulkanRenderer::_initializeSkyBox(POINTER<InternalFormat> format)
        {
            VulkanSkyBox* _renderObject = Cast<VulkanSkyBox*>(format.get());
            _renderObject->initializeResources(_getBasicInitInfo());

            _renderObject->setSwapChainContainer(&mySwapChain.swapChainContainer);
            _renderObject->setFrameBufferContainer(&myFrameBuffer);

            return _renderObject->initializeObject(format, myGraphicsCore.msaaSamples);
        }
    }
}