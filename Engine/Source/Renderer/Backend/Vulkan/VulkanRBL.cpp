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

#include "Renderer Backend Layer/Compute/VulkanComputeCore.h"

#include "Renderer Backend Layer/Common/VulkanUtilities.h"
#include "VulkanPresets.h"
#include "Renderer Backend Layer/Common/VulkanControlHeader.h"

#define TEXTOVERLAY_MAX_CHAR_COUNT 2048

/* IDEAS
 Make one time compute objects so that we can compute something at a given instance
 Make multiple of those to compute multiple things.
*/

namespace Dynamik {
	namespace Renderer {
#ifdef DMK_DEBUG
		Debugger::benchmark::FPS myFPSCal;

		struct _debugVertex {
			VEC3 position = { 0.0f, 0.0f, 0.0f };
			VEC3 color = { 0.0f, 0.0f, 0.0f };
			VEC2 tex = { 0.0f, 0.0f };
			F32 integrity = 1.0f;
		};

		ARRAY<_debugVertex> _debugVertexBuffer()
		{
			ARRAY<_debugVertex> _container;
			_debugVertex _vertex;

			/* Triangle 1 */
			_vertex.position = { 1.0f, 1.0f, 1.0f };
			_vertex.tex = { 1.0f, 1.0f };
			_container.pushBack(_vertex);

			_vertex.position = { -1.0f, -1.0f, 1.0f };
			_vertex.tex = { 1.0f, 0.0f };
			_container.pushBack(_vertex);

			_vertex.position = { -1.0f, 1.0f, 1.0f };
			_vertex.tex = { 0.0f, 1.0f };
			_container.pushBack(_vertex);

			/* Triangle 2 */
			_vertex.position = { 1.0f, -1.0f, 1.0f };
			_vertex.tex = { 0.0f, 0.0f };
			_container.pushBack(_vertex);

			_vertex.position = { 1.0f, 1.0f, 1.0f };
			_vertex.tex = { 1.0f, 1.0f };
			_container.pushBack(_vertex);

			_vertex.position = { -1.0f, -1.0f, 1.0f };
			_vertex.tex = { 1.0f, 0.0f };
			_container.pushBack(_vertex);

			return _container;
		}

		// ----------
#endif
		void VulkanRBL::setMsaaSamples(DMKPipelineMSAASamples samples)
		{
			switch (samples)
			{
			case Dynamik::DMKPipelineMSAASamples::DMK_PIPELINE_MSAA_SAMPLES_1:
				myMsaaSamples = VK_SAMPLE_COUNT_1_BIT;
				break;
			case Dynamik::DMKPipelineMSAASamples::DMK_PIPELINE_MSAA_SAMPLES_2:
				myMsaaSamples = VK_SAMPLE_COUNT_2_BIT;
				break;
			case Dynamik::DMKPipelineMSAASamples::DMK_PIPELINE_MSAA_SAMPLES_4:
				myMsaaSamples = VK_SAMPLE_COUNT_4_BIT;
				break;
			case Dynamik::DMKPipelineMSAASamples::DMK_PIPELINE_MSAA_SAMPLES_8:
				myMsaaSamples = VK_SAMPLE_COUNT_8_BIT;
				break;
			case Dynamik::DMKPipelineMSAASamples::DMK_PIPELINE_MSAA_SAMPLES_16:
				myMsaaSamples = VK_SAMPLE_COUNT_16_BIT;
				break;
			case Dynamik::DMKPipelineMSAASamples::DMK_PIPELINE_MSAA_SAMPLES_32:
				myMsaaSamples = VK_SAMPLE_COUNT_32_BIT;
				break;
			case Dynamik::DMKPipelineMSAASamples::DMK_PIPELINE_MSAA_SAMPLES_64:
				myMsaaSamples = VK_SAMPLE_COUNT_64_BIT;
				break;
			default:
				DMK_CORE_FATAL("Invalid MSAA sample count!");
				break;
			}
		}

		void VulkanRBL::setWindowHandle(DMKWindowHandle windowHandle)
		{
#ifdef DMK_PLATFORM_WINDOWS
			WindowsWindow _window = *(WindowsWindow*)windowHandle.get();
			myWindowHandle = _window.window;

#endif // DMK_PLATFORM_WINDOWS
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
			VulkanInstanceInitInfo instanceInitInfo;
			instanceInitInfo.applicationName = "Dynamik Engine";
			instanceInitInfo.engineName = "Dynamik";
			myGraphicsCore.initializeInstance(instanceInitInfo);

			/* Initialize the debugger */
			myGraphicsCore.initializeDebugger();

			/* Create the basic window surface */
			myBasicSurface = myGraphicsCore.createSurface(myWindowHandle);

			/* Initialize the device (physical and logical) */
			myGraphicsCore.initializeDevice(myBasicSurface.surface);

			/* Validate the provided MSAA samples */
			if (!_validateMSAASamples())
			{
				DMK_CORE_WARN("Provided MSAA samples are not supported by the GPU. Samples will be set to the maximum sample count available.");
				myMsaaSamples = myGraphicsCore.maxMSAASamples;
			}

			/* Validate and get surface capabilities */
			myGraphicsCore.validateSurface(myBasicSurface.surface);
			myGraphicsCore.getSurfaceCapabilities(&myBasicSurface);

			/* Initialize command pools (both host visible and client visible) */
			VulkanGraphicsCommandBufferInitResources commandBufferResourceInitInfo;
			commandBufferResourceInitInfo.logicalDevice = myGraphicsCore.logicalDevice;
			commandBufferResourceInitInfo.physicalDevice = myGraphicsCore.physicalDevice;
			commandBufferResourceInitInfo.surface = myBasicSurface.surface;
			myMainCommandBuffer.initializeResources(commandBufferResourceInitInfo);
			myMainCommandBuffer.initializeCommandPool();

			/* Initialize the overlay */
			/* TODO: Overlay */
		}

		void VulkanRBL::initializeComputeCore()
		{
		}

		void VulkanRBL::createNewContext(DMKRenderContextType type, POINTER<GLFWwindow> windowHandle)
		{
			if (!windowHandle.isValid())
			{
				DMK_CORE_WARN("Invalid window handle passed! Setting the window handle to the parent.");
				windowHandle = myWindowHandle;
			}

			VulkanRenderContext _context;
			_context.type = type;

			_context.surfaceContainer = _createNewSurface(windowHandle);
			_context.swapChain.setBasicData(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, _context.surfaceContainer);

			I32 _width = 0, _height = 0;
			glfwGetWindowSize(windowHandle.get(), &_width, &_height);
			_context.swapChain.initialize(_width, _height, _context.surfaceContainer);

			/* ----------********** TEMPORAY **********---------- */
			ARRAY<VkAttachmentDescription> attachments;

			// attachment descriptions
			VkAttachmentDescription colorAttachment = {};
			colorAttachment.format = _context.swapChain.swapChainImageFormat;
			colorAttachment.samples = myMsaaSamples;
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
			depthAttachment.samples = myMsaaSamples;
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
			colorAttachmentResolve.format = _context.swapChain.swapChainImageFormat;
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

			VulkanRenderPassInitInfo renderPassInitInfo;
			renderPassInitInfo.attachments = attachments;
			renderPassInitInfo.subPasses = subPasses;
			/* ----------********** TEMPORAY **********---------- */

			_context.renderPass.initialize(myGraphicsCore.logicalDevice, renderPassInitInfo);

			VulkanGraphicsCommandBufferInitResources commandBufferResourceInitInfo;
			commandBufferResourceInitInfo.logicalDevice = myGraphicsCore.logicalDevice;
			commandBufferResourceInitInfo.physicalDevice = myGraphicsCore.physicalDevice;
			commandBufferResourceInitInfo.surface = _context.surfaceContainer.surface;
			_context.inFlightCommandBuffer.initializeResources(commandBufferResourceInitInfo);
			_context.inFlightCommandBuffer.pool = myMainCommandBuffer.pool;

			/* Initialize attachments */
			/* Initialize color buffer */
			myColorBuffer.initialize(
				myGraphicsCore.logicalDevice,
				myGraphicsCore.physicalDevice,
				myMainCommandBuffer.pool,
				myGraphicsCore.graphicsQueue,
				myGraphicsCore.presentQueue,
				_context.swapChain.swapChainImageFormat,
				_context.swapChain.swapChainExtent,
				myMsaaSamples);

			/* Initialize color buffer */
			myDepthBuffer.initialize(
				myGraphicsCore.logicalDevice,
				myGraphicsCore.physicalDevice,
				myMainCommandBuffer.pool,
				myGraphicsCore.graphicsQueue,
				myGraphicsCore.presentQueue,
				_context.swapChain.swapChainExtent,
				myMsaaSamples);

			VulkanGraphicsFrameBufferInitInfo frameBufferInitInfo;
			switch (type)
			{
			case Dynamik::Renderer::DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_2D:
				frameBufferInitInfo.attachments.pushBack(myColorBuffer.imageView);
				break;

			case Dynamik::Renderer::DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_3D:
				frameBufferInitInfo.attachments.pushBack(myColorBuffer.imageView);
				frameBufferInitInfo.attachments.pushBack(myDepthBuffer.imageView);
				break;

			case Dynamik::Renderer::DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEFAULT:
				frameBufferInitInfo.attachments.pushBack(myColorBuffer.imageView);
				frameBufferInitInfo.attachments.pushBack(myDepthBuffer.imageView);
				break;

			case Dynamik::Renderer::DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEFAULT_HDR:
				frameBufferInitInfo.attachments.pushBack(myColorBuffer.imageView);
				frameBufferInitInfo.attachments.pushBack(myDepthBuffer.imageView);
				break;

			case Dynamik::Renderer::DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEFAULT_MONO:
				frameBufferInitInfo.attachments.pushBack(myDepthBuffer.imageView);
				break;

			case Dynamik::Renderer::DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEBUG:
				frameBufferInitInfo.attachments.pushBack(myColorBuffer.imageView);
				frameBufferInitInfo.attachments.pushBack(myDepthBuffer.imageView);
				break;

			default:
				DMK_CORE_FATAL("Invalid Render Context type!");
				break;
			}
			frameBufferInitInfo.swapChainImageViews = _context.swapChain.swapChainImageViews;
			frameBufferInitInfo.bufferCount = _context.swapChain.swapChainImages.size();
			frameBufferInitInfo.swapChainExtent = _context.swapChain.swapChainExtent;
			frameBufferInitInfo.renderPass = _context.renderPass.renderPass;
			_context.frameBuffer.initialize(myGraphicsCore.logicalDevice, frameBufferInitInfo);

			myRenderContexts.pushBack(_context);
			_sortRenderContexts();
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
					initializeObject(format);
		}

		VPTR VulkanRBL::initializeObject(POINTER<InternalFormat> format, DMKRenderContextType context)
		{
			DMK_BEGIN_PROFILE_TIMER();

			VulkanRenderContext _context;

			if (myRenderContexts.isValidIndex((UI32)context))
			{
				if (myRenderContexts[(UI32)context].type != context)
				{
					//createNewContext(context, myWindowHandle);
					DMK_CORE_FATAL("Specified context type is not yet initialized. Make sure to create the required contexts prior to loading objects.");
				}
			}
			else
			{
				DMK_CORE_ERROR("Specified render context is not initialized. The objects are set to use the default context.");
				_context = myRenderContexts[0];
			}

			VulkanRenderData _renderData;
			_renderData.renderTechnology = format->descriptor.renderSpecification.renderingTechnology;
			_renderData.indexBufferType = format->descriptor.indexBufferType;

			/* Initialize mesh data */
			for (auto mesh : format->meshDatas)
			{
				/* initialize vertex buffers */
				_renderData.vertexBufferContainer.pushBack(createVertexBuffer(mesh, format->descriptor.vertexBufferObjectDescription.attributes));

				/* initialize index buffers */
				_renderData.indexBufferContainer.pushBack(createIndexBuffer(mesh, format->descriptor.indexBufferType));

				/* initialize textures */
				for (UI32 i = 0; i < mesh.textureDatas.size(); i++)
					_renderData.textures.pushBack(createTextureImage(mesh.textureDatas[i]));
			}

			/* Initialize uniform buffers */
			for (auto _uniformBufferDescription : format->descriptor.uniformBufferObjectDescriptions)
				if (_uniformBufferDescription.type == DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT)
					_renderData.uniformBufferContainers.pushBack(createUniformBuffers(_uniformBufferDescription, _context.swapChain.swapChainImages.size()));

			/* Initialize Descriptors */
			_renderData.descriptors = createDescriptors(format->descriptor.uniformBufferObjectDescriptions, _renderData.uniformBufferContainers, _renderData.textures);

			/* Initialize pipeline */
			_renderData.pipelineContainers.pushBack(createPipeline({ _renderData.descriptors }, format->descriptor.uniformBufferObjectDescriptions, format->descriptor.vertexBufferObjectDescription.attributes, format->shaderPaths, format->type, _context));

			/* Add data to the container and return its address */
			myRenderContexts[(UI32)context].renderDatas.pushBack(_renderData);
			return &myRenderContexts[(UI32)context].renderDatas[myRenderContexts[(UI32)context].renderDatas.size() - 1];
		}

		void VulkanRBL::initializeCommands()
		{
			DMK_BEGIN_PROFILE_TIMER();

			for (UI32 _itr = 0; _itr < myRenderContexts.size(); _itr++)
			{
				if (myRenderContexts[_itr].state != VulkanResourceState::ADGR_VULKAN_RESOURCE_STATE_HOST_VISIBLE)
					continue;

				VulkanGraphicsCommandBufferInitInfo initInfo;
				initInfo.count = myRenderContexts[_itr].swapChain.swapChainImages.size();
				initInfo.frameBuffer = myRenderContexts[_itr].frameBuffer;
				initInfo.swapChain = myRenderContexts[_itr].swapChain;
				initInfo.objects = myRenderContexts[_itr].renderDatas;
				initInfo.renderPass = myRenderContexts[_itr].renderPass;
				myRenderContexts[_itr].inFlightCommandBuffer.initializeCommandBuffers(initInfo);
			}
		}

		void VulkanRBL::initializeFinalComponents()
		{
			DMK_BEGIN_PROFILE_TIMER();

			myGraphicsCore.initializeSyncObjects();
		}

		void VulkanRBL::drawFrame(DMKRendererDrawFrameInfo info, DMKRenderContextType context)
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Get the render data from the context */
			auto _renderContext = myRenderContexts[(UI32)context];

			/* Check if the number of update objects are equal to the in-flight objects */
			if (info.formats.size() != _renderContext.renderDatas.size())
				DMK_CORE_FATAL("Invalid amount of update formats sent to the Draw call!");

			/* Sync Vulkan Fences */
			myGraphicsCore.syncFence(currentFrame);

			/* Get the current image index */
			imageIndex = 0;
			result = myGraphicsCore.getNextImage(_renderContext.swapChain.swapChain, &imageIndex, currentFrame);

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
					if (info.formats[index]->descriptor.uniformBufferObjectDescriptions[_itr].location != DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX)
						continue;

					/* Update the objects uniform buffer memory */
					VulkanUtilities::updateUniformBuffer(
						myGraphicsCore.logicalDevice,
						info.formats[index]->onUpdate(info.cameraData),
						_renderContext.renderDatas[index].uniformBufferContainers[_itr].bufferMemories[imageIndex]);
				}
			}

			/* Submit queues */
			result = myGraphicsCore.submitQueues(
				{ _renderContext.swapChain.swapChain },
				imageIndex, currentFrame,
				{ _renderContext.inFlightCommandBuffer.buffers[imageIndex] });

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

		inline B1 VulkanRBL::_validateMSAASamples()
		{
			return myMsaaSamples <= myGraphicsCore.maxMSAASamples;
		}

		inline VulkanSurfaceContainer VulkanRBL::_createNewSurface(POINTER<GLFWwindow> windowHandle)
		{
			VulkanSurfaceContainer _container = myGraphicsCore.createSurface(windowHandle);
			myGraphicsCore.validateSurface(_container.surface);
			myGraphicsCore.getSurfaceCapabilities(&_container);

			return _container;
		}

		VulkanRenderPassInitInfo VulkanRBL::_getDefaultRenderPassInfo(VkFormat swapChainImageFormat)
		{
			DMK_BEGIN_PROFILE_TIMER();

			ARRAY<VkAttachmentDescription> attachments;

			// attachment descriptions
			VkAttachmentDescription colorAttachment = {};
			colorAttachment.format = swapChainImageFormat;
			colorAttachment.samples = myMsaaSamples;
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
			depthAttachment.samples = myMsaaSamples;
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
			colorAttachmentResolve.format = swapChainImageFormat;
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

			VulkanRenderPassInitInfo renderPassInitInfo;
			renderPassInitInfo.attachments = attachments;
			renderPassInitInfo.subPasses = subPasses;
			return renderPassInitInfo;
		}

		VulkanGraphicsRenderableObjectInitInfo VulkanRBL::_getBasicInitInfo()
		{
			VulkanGraphicsRenderableObjectInitInfo _info;
			_info.logicalDevice = myGraphicsCore.logicalDevice;
			_info.physicalDevice = myGraphicsCore.physicalDevice;
			_info.commandPool = myMainCommandBuffer.pool;
			_info.graphicsQueue = myGraphicsCore.graphicsQueue;
			_info.presentQueue = myGraphicsCore.presentQueue;

			return _info;
		}

		VulkanBufferContainer VulkanRBL::createVertexBuffer(Mesh mesh, ARRAY<DMKVertexAttribute> attributes)
		{
			VulkanBufferContainer _container;
			_container.dataCount = mesh.vertexDataStore.size();

			VkDeviceSize bufferSize = mesh.allocatableSize(attributes);
			VkBuffer stagingBuffer = VK_NULL_HANDLE;
			VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

			VulkanCreateBufferInfo bufferInfo;
			bufferInfo.bufferSize = bufferSize;
			bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			bufferInfo.buffer = &stagingBuffer;
			bufferInfo.bufferMemory = &stagingBufferMemory;
			VulkanUtilities::createBuffer(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, bufferInfo);

			void* data = nullptr;
			vkMapMemory(myGraphicsCore.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
			mesh.packData(attributes, data);
			vkUnmapMemory(myGraphicsCore.logicalDevice, stagingBufferMemory);

			VulkanCreateBufferInfo vertBufferInfo;
			vertBufferInfo.bufferSize = bufferSize;
			vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			vertBufferInfo.buffer = &_container.buffer;
			vertBufferInfo.bufferMemory = &_container.bufferMemory;
			VulkanUtilities::createBuffer(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, vertBufferInfo);

			VulkanUtilities::copyBuffer(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, stagingBuffer, _container.buffer, bufferSize);

			vkDestroyBuffer(myGraphicsCore.logicalDevice, stagingBuffer, nullptr);
			vkFreeMemory(myGraphicsCore.logicalDevice, stagingBufferMemory, nullptr);

			return _container;
		}

		VulkanBufferContainer VulkanRBL::createIndexBuffer(Mesh mesh, DMKDataType type)
		{
			VulkanBufferContainer _container;
			_container.dataCount = mesh.indexes.size();

			VkDeviceSize bufferSize = _container.dataCount * (UI32)type;
			VkBuffer stagingBuffer = VK_NULL_HANDLE;
			VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

			VulkanCreateBufferInfo bufferInfo;
			bufferInfo.bufferSize = bufferSize;
			bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			bufferInfo.buffer = &stagingBuffer;
			bufferInfo.bufferMemory = &stagingBufferMemory;
			VulkanUtilities::createBuffer(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, bufferInfo);

			void* data = nullptr;
			vkMapMemory(myGraphicsCore.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, mesh.indexes.data(), (size_t)bufferSize);
			vkUnmapMemory(myGraphicsCore.logicalDevice, stagingBufferMemory);

			VulkanCreateBufferInfo indexBufferInfo;
			indexBufferInfo.bufferSize = bufferSize;
			indexBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
			indexBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			indexBufferInfo.buffer = &_container.buffer;
			indexBufferInfo.bufferMemory = &_container.bufferMemory;
			VulkanUtilities::createBuffer(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, indexBufferInfo);

			VulkanUtilities::copyBuffer(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, stagingBuffer, _container.buffer, bufferSize);

			vkDestroyBuffer(myGraphicsCore.logicalDevice, stagingBuffer, nullptr);
			vkFreeMemory(myGraphicsCore.logicalDevice, stagingBufferMemory, nullptr);

			return _container;
		}

		VulkanTextureContainer VulkanRBL::createTextureImage(Texture texture)
		{
			VulkanUtilitiesTextureInitInfo initInfo;
			initInfo.logicalDevice = myGraphicsCore.logicalDevice;
			initInfo.physicalDevice = myGraphicsCore.physicalDevice;
			initInfo.commandPool = myMainCommandBuffer.pool;
			initInfo.processQueue = myGraphicsCore.graphicsQueue;
			initInfo.utilityQueue = myGraphicsCore.presentQueue;
			initInfo.mipLevels = 1;
			initInfo.minMipLevels = 0;
			initInfo.maxMipLevels = 1;
			if (texture.type == DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP)
			{
				initInfo.modeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				initInfo.modeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				initInfo.modeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			}
			else
			{
				initInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				initInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				initInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			}
			initInfo.magFilter = VK_FILTER_LINEAR;
			initInfo.minFilter = VK_FILTER_LINEAR;
			initInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			return VulkanUtilities::createTexture(initInfo, texture);
		}

		VulkanUnformBufferContainer VulkanRBL::createUniformBuffers(DMKUniformBufferObjectDescriptor uniformBufferDescriptor, UI32 bufferCount)
		{
			return VulkanUtilities::createUniformBuffers(
				myGraphicsCore.logicalDevice,
				myGraphicsCore.physicalDevice,
				DMKUniformBufferObjectDescriptor::uniformByteSize(uniformBufferDescriptor.attributes),
				bufferCount);
		}

		VulkanGraphicsDescriptor VulkanRBL::createDescriptors(ARRAY<DMKUniformBufferObjectDescriptor> descriptors, ARRAY<VulkanUnformBufferContainer> uniformBufferContainers, ARRAY<VulkanTextureContainer> textureContainers)
		{
			VulkanGraphicsDescriptor _descriptor;

			/* Initialize descriptor set layout */
			VulkanDescriptorSetLayoutInitInfo layoutInitInfo;
			layoutInitInfo.bindings = VulkanUtilities::getDescriptorSetBindings(descriptors);
			_descriptor.initializeLayout(myGraphicsCore.logicalDevice, layoutInitInfo);

			/* Initialize descriptor pool */
			VulkanDescriptorPoolInitInfo initInfo;
			initInfo.poolSizes = VulkanUtilities::getPoolSizes(descriptors, uniformBufferContainers.size(), textureContainers.size());
			_descriptor.initializePool(myGraphicsCore.logicalDevice, initInfo);

			/* Initialize descriptor sets */
			ARRAY<VkWriteDescriptorSet> _writes;

			for (UI32 binding = 0; binding < descriptors.size(); binding++)
			{
				ARRAY<VkDescriptorBufferInfo> bufferInfos;

				switch (descriptors[binding].type)
				{
					/* Initialize Uniform buffer descriptor */
				case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT:
					for (UI32 itr = 0; itr < uniformBufferContainers.size(); itr++)
					{
						for (UI32 i = 0; i < uniformBufferContainers[itr].buffers.size(); i++)
						{
							VkDescriptorBufferInfo bufferInfo = {};
							bufferInfo.buffer = uniformBufferContainers[itr].buffers[i];
							bufferInfo.offset = 0;
							bufferInfo.range = DMKUniformBufferObjectDescriptor::uniformByteSize(descriptors[binding].attributes);
							bufferInfos.pushBack(bufferInfo);
						}
					}

					VkWriteDescriptorSet _writes1;
					_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes1.dstBinding = 0;
					_writes1.dstArrayElement = 0;
					_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					_writes1.descriptorCount = 1;
					_writes1.pBufferInfo = bufferInfos.data();
					_writes1.pNext = VK_NULL_HANDLE;
					_writes1.pImageInfo = VK_NULL_HANDLE;
					_writes1.pTexelBufferView = VK_NULL_HANDLE;
					_writes.push_back(_writes1);
					break;

				case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_CONSTANT:
					break;

				default:
					for (UI32 _texIndex = 0; _texIndex < textureContainers.size(); _texIndex++)
					{
						VkDescriptorImageInfo imageInfo = {};
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						imageInfo.imageView = textureContainers[_texIndex].imageView;
						imageInfo.sampler = textureContainers[_texIndex].imageSampler;

						VkWriteDescriptorSet _writes2;
						_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes2.dstBinding = 1;
						_writes2.dstArrayElement = 0;
						_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						_writes2.descriptorCount = 1;
						_writes2.pImageInfo = &imageInfo;
						_writes2.pNext = VK_NULL_HANDLE;
						_writes2.pTexelBufferView = VK_NULL_HANDLE;
						_writes2.pBufferInfo = VK_NULL_HANDLE;
						_writes.push_back(_writes2);
					}
					break;
				}
			}

			VulkanDescriptorSetsInitInfo setInitInfo;
			setInitInfo.descriptorWrites = _writes;
			_descriptor.initializeSets(myGraphicsCore.logicalDevice, setInitInfo);

			return _descriptor;
		}

		VulkanGraphicsPipeline VulkanRBL::createPipeline(
			ARRAY<VulkanGraphicsDescriptor> descriptors,
			ARRAY<DMKUniformBufferObjectDescriptor> uniformBufferDescriptors,
			ARRAY<DMKVertexAttribute> attributes,
			ShaderPaths paths,
			DMKObjectType objectType,
			VulkanRenderContext context)
		{
			VulkanGraphicsPipeline _pipeline;

			/* Initialize pipeline layout */
			VulkanGraphicsPipelineLayoutInitInfo layoutInitInfo;
			for (auto _descriptor : descriptors)
				layoutInitInfo.layouts.pushBack(_descriptor.layout);

			UI32 _oldRangeSize = 0;
			for (auto _description : uniformBufferDescriptors)
			{
				if (_description.type != DMKUniformType::DMK_UNIFORM_TYPE_CONSTANT)
					continue;

				VkPushConstantRange _range;
				_range.stageFlags = VulkanUtilities::getShaderStage(_description.location);
				_range.size = DMKUniformBufferObjectDescriptor::uniformByteSize(_description.attributes);
				_range.offset = _oldRangeSize;
				layoutInitInfo.pushConstantRanges.pushBack(_range);

				_oldRangeSize += _range.size;
			}
			_pipeline.initializePipelineLayout(myGraphicsCore.logicalDevice, layoutInitInfo);

			/* Initialize pipeline */
			auto _shaders = VulkanUtilities::getGraphicsShaders(myGraphicsCore.logicalDevice, paths);
			switch (objectType)
			{
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_IMAGE_2D:
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myMsaaSamples,
						context.renderPass.renderPass,
						_shaders,
						attributes,
						context.swapChain.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_MESH:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myMsaaSamples,
						context.renderPass.renderPass,
						_shaders,
						attributes,
						context.swapChain.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myMsaaSamples,
						context.renderPass.renderPass,
						_shaders,
						attributes,
						context.swapChain.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myMsaaSamples,
						context.renderPass.renderPass,
						_shaders,
						attributes,
						context.swapChain.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PLAYER:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myMsaaSamples,
						context.renderPass.renderPass,
						_shaders,
						attributes,
						context.swapChain.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_NPC:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myMsaaSamples,
						context.renderPass.renderPass,
						_shaders,
						attributes,
						context.swapChain.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXTURE_UI:
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKYBOX:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePresetSkyBox(
						myGraphicsCore.logicalDevice,
						myMsaaSamples,
						context.renderPass.renderPass,
						_shaders,
						attributes,
						context.swapChain.swapChainExtent));
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
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myMsaaSamples,
						context.renderPass.renderPass,
						_shaders,
						attributes,
						context.swapChain.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_LIGHT:
				break;
			}

			return _pipeline;
		}

		VulkanRenderData VulkanRBL::_initializeSkeletalAnimation(POINTER<InternalFormat> format)
		{
			//VulkanSkeletalAnimation* _renderObject = Cast<VulkanSkeletalAnimation*>(format.get());

			return VulkanRenderData();
		}

		inline void VulkanRBL::_sortRenderContexts()
		{
			UI32 _size = myRenderContexts.size() - 1, _indexCount = 0;
			UI32 _index = 0, _itr = 0;

			while (_indexCount < _size)
			{
				_index = 0;
				_itr = 0;
				while (_itr < _size - _indexCount)
				{
					if (myRenderContexts[_index].type > myRenderContexts[_itr].type)
						std::swap(myRenderContexts[_index], myRenderContexts[_itr]);
					_index++;
					_itr++;
				}
				_indexCount++;
			}
		}
	}
}