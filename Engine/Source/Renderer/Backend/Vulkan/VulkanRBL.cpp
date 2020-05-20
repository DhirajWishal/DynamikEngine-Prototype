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
#include "Renderer Backend Layer/Common/VulkanOneTimeCommandBuffer.h"

#define TEXTOVERLAY_MAX_CHAR_COUNT 2048
#define M_PI	3.14159265359

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

		std::vector<_debugVertex> _debugVertexBuffer()
		{
			std::vector<_debugVertex> _container;
			_debugVertex _vertex;

			/* Triangle 1 */
			_vertex.position = { 1.0f, 1.0f, 1.0f };
			_vertex.tex = { 1.0f, 1.0f };
			_container.push_back(_vertex);

			_vertex.position = { -1.0f, -1.0f, 1.0f };
			_vertex.tex = { 1.0f, 0.0f };
			_container.push_back(_vertex);

			_vertex.position = { -1.0f, 1.0f, 1.0f };
			_vertex.tex = { 0.0f, 1.0f };
			_container.push_back(_vertex);

			/* Triangle 2 */
			_vertex.position = { 1.0f, -1.0f, 1.0f };
			_vertex.tex = { 0.0f, 0.0f };
			_container.push_back(_vertex);

			_vertex.position = { 1.0f, 1.0f, 1.0f };
			_vertex.tex = { 1.0f, 1.0f };
			_container.push_back(_vertex);

			_vertex.position = { -1.0f, -1.0f, 1.0f };
			_vertex.tex = { 1.0f, 0.0f };
			_container.push_back(_vertex);

			return _container;
		}

		std::vector<MeshPointStore> _generateBoundingMesh(F32 _position)
		{
			std::vector<MeshPointStore> _vertexBufferObject;
			MeshPointStore _store;
			_store.color = { 1.0f, 1.0f, 1.0f };

			_store.position = { -_position, _position, _position };
			_vertexBufferObject.push_back(_store);

			_store.position = { -_position, _position, -_position };
			_vertexBufferObject.push_back(_store);

			_store.position = { _position,  _position, -_position };
			_vertexBufferObject.push_back(_store);

			_store.position = { _position,  _position, _position };
			_vertexBufferObject.push_back(_store);

			_store.position = { _position, -_position, _position };
			_vertexBufferObject.push_back(_store);

			_store.position = { _position, -_position, -_position };
			_vertexBufferObject.push_back(_store);

			_store.position = { -_position, -_position, -_position };
			_vertexBufferObject.push_back(_store);

			_store.position = { -_position, -_position, _position };
			_vertexBufferObject.push_back(_store);

			return _vertexBufferObject;
		}

		std::vector<UI32> _generateBoundingMeshIndices()
		{
			return {
				0, 1, 1, 2, 2, 3, 3, 0,
				3, 4, 4, 5, 5, 2, 5, 6,
				6, 1, 6, 7, 7, 0, 7, 4
			};
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

			for (auto _context : myRenderContexts)
			{
				if (_context.type == type)
				{
					DMK_CORE_ERROR("The requested conetxt is already been initialized!");
					return;
				}
			}

			VulkanRenderContext _context;
			_context.type = type;

			_context.surfaceContainer = _createNewSurface(windowHandle);
			_context.swapChain.setBasicData(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, _context.surfaceContainer);

			I32 _width = 0, _height = 0;
			glfwGetWindowSize(windowHandle.get(), &_width, &_height);
			_context.swapChain.initialize(_width, _height, _context.surfaceContainer);

			/* ----------********** TEMPORAY **********---------- */
			std::vector<VkAttachmentDescription> attachments;

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

			std::vector<VkSubpassDescription> subPasses;
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
				frameBufferInitInfo.attachments.push_back(myColorBuffer.imageView);
				break;

			case Dynamik::Renderer::DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_3D:
				frameBufferInitInfo.attachments.push_back(myColorBuffer.imageView);
				frameBufferInitInfo.attachments.push_back(myDepthBuffer.imageView);
				break;

			case Dynamik::Renderer::DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEFAULT:
				frameBufferInitInfo.attachments.push_back(myColorBuffer.imageView);
				frameBufferInitInfo.attachments.push_back(myDepthBuffer.imageView);
				break;

			case Dynamik::Renderer::DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEFAULT_HDR:
				frameBufferInitInfo.attachments.push_back(myColorBuffer.imageView);
				frameBufferInitInfo.attachments.push_back(myDepthBuffer.imageView);
				break;

			case Dynamik::Renderer::DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEFAULT_MONO:
				frameBufferInitInfo.attachments.push_back(myDepthBuffer.imageView);
				break;

			case Dynamik::Renderer::DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEBUG:
				frameBufferInitInfo.attachments.push_back(myColorBuffer.imageView);
				frameBufferInitInfo.attachments.push_back(myDepthBuffer.imageView);
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

			myRenderContexts.push_back(_context);
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

		void VulkanRBL::addObjects(std::vector<POINTER<InternalFormat>> formats)
		{
			for (auto format : formats)
				if (format.isValid())
					initializeObject(format);
		}

		VPTR VulkanRBL::initializeObject(POINTER<InternalFormat> format, DMKRenderContextType context)
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Pre initialization stage */
			VulkanRenderContext _context;
			VulkanRenderData _renderData;
			VulkanRenderObject _object;
			_object.type = format->type;

			/* Resolve attachments */
			B1 isTextureAvailable = false;
			for (auto _attachment : format->descriptor.additionalAttachments)
			{
				switch (_attachment)
				{
				case Dynamik::DMKAttachmentType::DMK_ATTACHMENT_TYPE_CUBE_MAP:
					if (!skyboxObject->textures.size())
						DMK_CORE_FATAL("Unable to resolve attachmen! Requested cubemap is not yet loaded. Try loading the skybox object before adding this object.");

					_renderData.textures = skyboxObject->textures;
					isTextureAvailable = true;
					break;

				case Dynamik::DMKAttachmentType::DMK_ATTACHMENT_TYPE_IMAGE_BASED_LIGHTING:
					if (!skyboxObject->textures.size())
						DMK_CORE_FATAL("Unable to resolve attachmen! Requested cubemap is not yet loaded. Try loading the skybox object before adding this object.");

					_renderData.textures.push_back(irradianceCube.textures[0]);
					_renderData.textures.push_back(BRDFObject.textures[0]);
					_renderData.textures.push_back(prefilteredCube.textures[0]);
					isTextureAvailable = true;
					break;
				}
			}

			if ((UI32)context < myRenderContexts.size())
			{
				if (myRenderContexts[(UI32)context].type != context)
				{
					//createNewContext(context, myWindowHandle);
					DMK_CORE_FATAL("Specified context type is not yet initialized. Make sure to create the required contexts prior to loading objects.");
				}
				else
					_context = myRenderContexts[(UI32)context];
			}
			else
			{
				DMK_CORE_ERROR("Specified render context is not initialized. The objects are set to use the default context.");
				_context = myRenderContexts[0];
			}

			_renderData.renderTechnology = format->descriptor.renderSpecification.renderingTechnology;
			_object.indexBufferType = format->descriptor.indexBufferType;

			/* Initialize mesh data */
			for (auto mesh : format->meshDatas)
			{
				/* initialize vertex buffers */
				_object.vertexBufferContainer.push_back(createVertexBuffer(mesh, format->descriptor.vertexBufferObjectDescription.attributes));

				/* initialize index buffers */
				_object.indexBufferContainer.push_back(createIndexBuffer(mesh, format->descriptor.indexBufferType));

				/* initialize textures */
				if (!isTextureAvailable)
					for (UI32 i = 0; i < mesh.textureDatas.size(); i++)
						_renderData.textures.push_back(createTextureImage(mesh.textureDatas[i]));

				if (format->type != DMKObjectType::DMK_OBJECT_TYPE_SKYBOX)
					_renderData.renderObject.push_back(createBoundingBox(mesh, myRenderContexts[(UI32)context], format->descriptor.transformDescriptor.hitBoxRadius));
			}

			/* Initialize uniform buffers */
			for (auto _uniformBufferDescription : format->descriptor.uniformBufferObjectDescriptions)
				if (_uniformBufferDescription.type == DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT)
					_object.uniformBufferContainers.push_back(createUniformBuffers(_uniformBufferDescription, _context.swapChain.swapChainImages.size()));

			/* Initialize Descriptors */
			_object.descriptors = createDescriptors(format->descriptor.uniformBufferObjectDescriptions, _object.uniformBufferContainers, _renderData.textures);

			/* Initialize pipeline */
			_object.pipeline = createPipeline({ _object.descriptors }, format->descriptor.uniformBufferObjectDescriptions, format->descriptor.vertexBufferObjectDescription.attributes, format->shaderPaths, format->type, _context);

			_renderData.renderObject.push_back(_object);

			/* Add data to the container and return its address */
			myRenderContexts[(UI32)context].renderDatas.push_back(_renderData);
			auto _address = &myRenderContexts[(UI32)context].renderDatas[myRenderContexts[(UI32)context].renderDatas.size() - 1];

			if (format->type == DMKObjectType::DMK_OBJECT_TYPE_SKYBOX)
			{
				skyboxObject = _address;

				//initializeBRDFTable();
				//initializeIrradianceCube();
				//initializePrefilteredCube();

				return skyboxObject;
			}

			return _address;
		}

		void VulkanRBL::initializeBRDFTable()
		{
			VkFormat format = VK_FORMAT_R16G16_SFLOAT;
			const I32 dim = 512;

			VulkanTextureContainer BRDFTexture;
			BRDFTexture.mipLevels = 1;
			BRDFTexture.format = format;
			BRDFTexture.width = dim;
			BRDFTexture.height = dim;

			VulkanCreateImageInfo imageInfo;
			imageInfo.width = dim;
			imageInfo.height = dim;
			imageInfo.format = format;
			imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			imageInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			imageInfo.image = &BRDFTexture.image;
			imageInfo.imageMemory = &BRDFTexture.imageMemory;
			imageInfo.mipLevels = BRDFTexture.mipLevels;
			imageInfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.arrayLayers = 1;
			imageInfo.flags = NULL;
			VulkanUtilities::createImage(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, imageInfo);

			VulkanTextureSamplerInitInfo samplerInitInfo;
			samplerInitInfo.magFilter = VK_FILTER_LINEAR;
			samplerInitInfo.minFilter = VK_FILTER_LINEAR;
			samplerInitInfo.maxMipLevels = 1.0f;
			samplerInitInfo.minMipLevels = 0.0f;
			samplerInitInfo.modeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInitInfo.modeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInitInfo.modeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInitInfo.mipLoadBias = 0.0f;
			samplerInitInfo.mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInitInfo.compareOp = VK_COMPARE_OP_NEVER;
			samplerInitInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
			samplerInitInfo.maxAnisotrophy = 1.0f;
			BRDFTexture.imageSampler = VulkanUtilities::createImageSampler(myGraphicsCore.logicalDevice, samplerInitInfo);

			VulkanCreateImageViewInfo cinfo2;
			cinfo2.image = BRDFTexture.image;
			cinfo2.format = BRDFTexture.format;
			cinfo2.mipLevels = BRDFTexture.mipLevels;
			cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			cinfo2.viewType = VK_IMAGE_VIEW_TYPE_2D;
			cinfo2.layerCount = 1;
			BRDFTexture.imageView = VulkanUtilities::createImageView(myGraphicsCore.logicalDevice, cinfo2);

			BRDFObject.textures.push_back(BRDFTexture);

			VkAttachmentDescription attDesc = {};
			// Color attachment
			attDesc.format = format;
			attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
			attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attDesc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

			VkSubpassDescription subpassDescription = {};
			subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpassDescription.colorAttachmentCount = 1;
			subpassDescription.pColorAttachments = &colorReference;

			// Use subpass dependencies for layout transitions
			std::array<VkSubpassDependency, 2> dependencies;
			dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[0].dstSubpass = 0;
			dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			dependencies[1].srcSubpass = 0;
			dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			VulkanRenderContext _context;
			VulkanRenderPassInitInfo renderPassInitInfo;
			renderPassInitInfo.additionalSubPassDependencies = { dependencies[0], dependencies[1] };
			renderPassInitInfo.attachments = { attDesc };
			renderPassInitInfo.subPasses = { subpassDescription };
			renderPassInitInfo.overrideDependencies = true;
			_context.renderPass.initialize(myGraphicsCore.logicalDevice, renderPassInitInfo);

			VulkanGraphicsFrameBufferInitInfo frameBufferInitInfo;
			frameBufferInitInfo.renderPass = _context.renderPass.renderPass;
			frameBufferInitInfo.swapChainExtent = { dim, dim };
			frameBufferInitInfo.bufferCount = 1;
			frameBufferInitInfo.attachments = { BRDFTexture.imageView };
			_context.frameBuffer.initialize(myGraphicsCore.logicalDevice, frameBufferInitInfo);

			VulkanDescriptor descriptor;
			descriptor.initializeLayout(myGraphicsCore.logicalDevice, VulkanDescriptorSetLayoutInitInfo());

			VkDescriptorPoolSize _poolInfo;
			_poolInfo.descriptorCount = 1;
			_poolInfo.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

			VulkanDescriptorPoolInitInfo poolInitInfo;
			poolInitInfo.maxDescriptorSets = 1;
			poolInitInfo.poolSizes = { _poolInfo };
			descriptor.initializePool(myGraphicsCore.logicalDevice, poolInitInfo);

			descriptor.allocateSets(myGraphicsCore.logicalDevice, 1);

			VulkanGraphicsPipeline pipeline;
			VulkanGraphicsPipelineLayoutInitInfo pipelineLayoutInitInfo;
			pipelineLayoutInitInfo.layouts = { descriptor.layout };
			pipeline.initializePipelineLayout(myGraphicsCore.logicalDevice, pipelineLayoutInitInfo);

			ShaderPaths _shaderPaths;
			_shaderPaths.vertexShader = "Shaders/BRDF/vert.spv";
			_shaderPaths.fragmentShader = "Shaders/BRDF/frag.spv";
			auto _shaders = VulkanUtilities::getGraphicsShaders(myGraphicsCore.logicalDevice, _shaderPaths);

			VulkanGraphicsPipelineInitInfo pipelineInitInfo = VulkanPresets::pipelinePreset3D(
				VK_SAMPLE_COUNT_1_BIT,
				_context.renderPass.renderPass,
				_shaders, {}, { dim, dim });
			pipelineInitInfo.dynamicStateEnable = true;
			pipelineInitInfo.depthStencilCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
			pipelineInitInfo.rasterizerCullMode = VK_CULL_MODE_NONE;
			pipelineInitInfo.rasterizerFrontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			pipelineInitInfo.swapChainExtent = { dim, dim };
			pipeline.initializePipeline(myGraphicsCore.logicalDevice, pipelineInitInfo);

			/* Command buffer */
			{
				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = _context.renderPass.renderPass;
				renderPassInfo.framebuffer = _context.frameBuffer.buffers[0];
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = { dim, dim };

				std::array<VkClearValue, 2> clearValues = {};
				clearValues[0].color = {
					0.0f, 0.0f, 0.0f, 0.0f
				};
				renderPassInfo.clearValueCount = static_cast<UI32>(clearValues.size());
				renderPassInfo.pClearValues = clearValues.data();

				VulkanOneTimeCommandBuffer cmdBuf(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue);
				vkCmdBeginRenderPass(cmdBuf.buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				VkViewport viewport;
				viewport.width = (F32)dim;
				viewport.height = (F32)dim;
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;
				viewport.x = 0;
				viewport.y = 0;

				VkRect2D scissor;
				scissor.extent = { dim, dim };
				scissor.offset = { 0, 0 };

				vkCmdSetViewport(cmdBuf.buffer, 0, 1, &viewport);
				vkCmdSetScissor(cmdBuf.buffer, 0, 1, &scissor);

				vkCmdBindPipeline(cmdBuf.buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

				vkCmdDraw(cmdBuf.buffer, 3, 1, 0, 0);

				vkCmdEndRenderPass(cmdBuf.buffer);
			}

			vkQueueWaitIdle(myGraphicsCore.presentQueue);
		}

		void VulkanRBL::initializeIrradianceCube()
		{
			VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;
			const I32 dim = 64;

			VulkanTextureContainer IrradianceCube;
			IrradianceCube.mipLevels = static_cast<uint32_t>(floor(log2(dim))) + 1;
			IrradianceCube.format = format;
			IrradianceCube.width = dim;
			IrradianceCube.height = dim;

			VulkanCreateImageInfo imageInfo;
			imageInfo.width = dim;
			imageInfo.height = dim;
			imageInfo.format = format;
			imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			imageInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			imageInfo.image = &IrradianceCube.image;
			imageInfo.imageMemory = &IrradianceCube.imageMemory;
			imageInfo.mipLevels = IrradianceCube.mipLevels;
			imageInfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.arrayLayers = 6;
			imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
			VulkanUtilities::createImage(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, imageInfo);

			VulkanTextureSamplerInitInfo samplerInitInfo;
			samplerInitInfo.magFilter = VK_FILTER_LINEAR;
			samplerInitInfo.minFilter = VK_FILTER_LINEAR;
			samplerInitInfo.maxMipLevels = 1.0f;
			samplerInitInfo.minMipLevels = IrradianceCube.mipLevels;
			samplerInitInfo.modeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInitInfo.modeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInitInfo.modeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInitInfo.mipLoadBias = 0.0f;
			samplerInitInfo.mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInitInfo.compareOp = VK_COMPARE_OP_NEVER;
			samplerInitInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
			samplerInitInfo.maxAnisotrophy = 1.0f;
			IrradianceCube.imageSampler = VulkanUtilities::createImageSampler(myGraphicsCore.logicalDevice, samplerInitInfo);

			VulkanCreateImageViewInfo cinfo2;
			cinfo2.image = IrradianceCube.image;
			cinfo2.format = IrradianceCube.format;
			cinfo2.mipLevels = IrradianceCube.mipLevels;
			cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			cinfo2.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
			cinfo2.layerCount = 6;
			IrradianceCube.imageView = VulkanUtilities::createImageView(myGraphicsCore.logicalDevice, cinfo2);

			irradianceCube.textures.push_back(IrradianceCube);

			VkAttachmentDescription attDesc = {};
			// Color attachment
			attDesc.format = format;
			attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
			attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

			VkSubpassDescription subpassDescription = {};
			subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpassDescription.colorAttachmentCount = 1;
			subpassDescription.pColorAttachments = &colorReference;

			// Use subpass dependencies for layout transitions
			std::array<VkSubpassDependency, 2> dependencies;
			dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[0].dstSubpass = 0;
			dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			dependencies[1].srcSubpass = 0;
			dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			VulkanRenderContext _context;
			VulkanRenderPassInitInfo renderPassInitInfo;
			renderPassInitInfo.additionalSubPassDependencies = { dependencies[0], dependencies[1] };
			renderPassInitInfo.attachments = { attDesc };
			renderPassInitInfo.subPasses = { subpassDescription };
			renderPassInitInfo.overrideDependencies = true;
			_context.renderPass.initialize(myGraphicsCore.logicalDevice, renderPassInitInfo);

			VulkanTextureContainer offscreenTexture;
			offscreenTexture.mipLevels = 1.0f;
			offscreenTexture.format = format;
			offscreenTexture.width = dim;
			offscreenTexture.height = dim;
			{
				VulkanCreateImageInfo imageInfo;
				imageInfo.width = dim;
				imageInfo.height = dim;
				imageInfo.format = format;
				imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
				imageInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				imageInfo.image = &offscreenTexture.image;
				imageInfo.imageMemory = &offscreenTexture.imageMemory;
				imageInfo.mipLevels = offscreenTexture.mipLevels;
				imageInfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
				imageInfo.arrayLayers = 1;
				imageInfo.flags = NULL;
				VulkanUtilities::createImage(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, imageInfo);

				VulkanCreateImageViewInfo cinfo2;
				cinfo2.image = offscreenTexture.image;
				cinfo2.format = offscreenTexture.format;
				cinfo2.mipLevels = offscreenTexture.mipLevels;
				cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				cinfo2.viewType = VK_IMAGE_VIEW_TYPE_2D;
				cinfo2.layerCount = 1;
				offscreenTexture.imageView = VulkanUtilities::createImageView(myGraphicsCore.logicalDevice, cinfo2);

				VulkanGraphicsFrameBufferInitInfo frameBufferInitInfo;
				frameBufferInitInfo.attachments = { offscreenTexture.imageView };
				frameBufferInitInfo.renderPass = _context.renderPass.renderPass;
				frameBufferInitInfo.bufferCount = 1;
				frameBufferInitInfo.swapChainExtent = { dim, dim };
				_context.frameBuffer.initialize(myGraphicsCore.logicalDevice, frameBufferInitInfo);

				VulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = offscreenTexture.image;
				transitionInfo.format = offscreenTexture.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				transitionInfo.mipLevels = offscreenTexture.mipLevels;
				transitionInfo.layerCount = 1;
				VulkanUtilities::transitionImageLayout(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, transitionInfo);
			}

			struct PushBlock {
				glm::mat4 mvp;
				// Sampling deltas
				float deltaPhi = (2.0f * float(M_PI)) / 180.0f;
				float deltaTheta = (0.5f * float(M_PI)) / 64.0f;
			} pushBlock;

			VulkanGraphicsPipeline pipeline;
			VkPushConstantRange _range;
			_range.offset = 0;
			_range.size = sizeof(pushBlock);
			_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

			VulkanGraphicsPipelineLayoutInitInfo pipelineLayoutInitInfo;
			pipelineLayoutInitInfo.layouts = { skyboxObject->renderObject[0].descriptors.layout };
			pipelineLayoutInitInfo.pushConstantRanges = { _range };
			pipeline.initializePipelineLayout(myGraphicsCore.logicalDevice, pipelineLayoutInitInfo);

			ShaderPaths _shaderPaths;
			_shaderPaths.vertexShader = "Shaders/IrradianceCube/vert.spv";
			_shaderPaths.fragmentShader = "Shaders/IrradianceCube/frag.spv";
			auto _shaders = VulkanUtilities::getGraphicsShaders(myGraphicsCore.logicalDevice, _shaderPaths);

			DMKVertexAttribute _attribute;
			_attribute.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
			_attribute.name = DMKVertexData::DMK_VERTEX_DATA_POSITION;

			VulkanGraphicsPipelineInitInfo pipelineInitInfo = VulkanPresets::pipelinePreset3D(
				VK_SAMPLE_COUNT_1_BIT,
				_context.renderPass.renderPass,
				_shaders, { _attribute }, { dim, dim });
			pipelineInitInfo.dynamicStateEnable = true;
			pipelineInitInfo.depthStencilCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
			pipelineInitInfo.rasterizerCullMode = VK_CULL_MODE_NONE;
			pipelineInitInfo.rasterizerFrontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			pipelineInitInfo.swapChainExtent = { dim, dim };
			pipeline.initializePipeline(myGraphicsCore.logicalDevice, pipelineInitInfo);

			/* Command buffer */
			{
				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = _context.renderPass.renderPass;
				renderPassInfo.framebuffer = _context.frameBuffer.buffers[0];
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = { dim, dim };

				std::array<VkClearValue, 2> clearValues = {};
				clearValues[0].color = {
					0.0f, 0.0f, 0.0f, 0.0f
				};
				renderPassInfo.clearValueCount = static_cast<UI32>(clearValues.size());
				renderPassInfo.pClearValues = clearValues.data();

				std::vector<glm::mat4> matrices = {
					// POSITIVE_X
					glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// NEGATIVE_X
					glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// POSITIVE_Y
					glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// NEGATIVE_Y
					glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// POSITIVE_Z
					glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// NEGATIVE_Z
					glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
				};

				VulkanOneTimeCommandBuffer cmdBuf(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue);

				VkViewport viewport;
				viewport.width = (F32)dim;
				viewport.height = (F32)dim;
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;
				viewport.x = 0;
				viewport.y = 0;

				VkRect2D scissor;
				scissor.extent = { dim, dim };
				scissor.offset = { 0, 0 };

				vkCmdSetViewport(cmdBuf.buffer, 0, 1, &viewport);
				vkCmdSetScissor(cmdBuf.buffer, 0, 1, &scissor);

				VkImageSubresourceRange subresourceRange = {};
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = IrradianceCube.mipLevels;
				subresourceRange.layerCount = 6;

				VulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = IrradianceCube.image;
				transitionInfo.format = IrradianceCube.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.mipLevels = IrradianceCube.mipLevels;
				transitionInfo.layerCount = 6;
				VulkanUtilities::transitionImageLayout(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, transitionInfo);

				for (uint32_t m = 0; m < IrradianceCube.mipLevels; m++) {
					for (uint32_t f = 0; f < 6; f++) {
						viewport.width = static_cast<float>(dim * std::pow(0.5f, m));
						viewport.height = static_cast<float>(dim * std::pow(0.5f, m));
						vkCmdSetViewport(cmdBuf.buffer, 0, 1, &viewport);

						// Render scene from cube face's point of view
						vkCmdBeginRenderPass(cmdBuf.buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

						// Update shader push constant block
						float FOV = M_PI / 2.0f;
						pushBlock.mvp = glm::perspective(FOV, 1.0f, 0.1f, 512.0f) * matrices[f];

						vkCmdPushConstants(cmdBuf.buffer, pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushBlock), &pushBlock);

						vkCmdBindPipeline(cmdBuf.buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

						for (auto _object : skyboxObject->renderObject)
						{
							for (VkDescriptorSet _set : _object.descriptors.descriptorSets)
								vkCmdBindDescriptorSets(cmdBuf.buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 1, &_set, 0, NULL);

							VkDeviceSize offsets[1] = { 0 };

							for (UI32 itr = 0; itr < _object.vertexBufferContainer.size(); itr++)
							{
								vkCmdBindVertexBuffers(cmdBuf.buffer, 0, 1, &_object.vertexBufferContainer[itr].buffer, offsets);
								vkCmdBindIndexBuffer(cmdBuf.buffer, _object.indexBufferContainer[itr].buffer, 0, VK_INDEX_TYPE_UINT32);
								vkCmdDrawIndexed(cmdBuf.buffer, _object.indexBufferContainer[itr].dataCount, 1, 0, 0, 0);
							}
						}

						vkCmdEndRenderPass(cmdBuf.buffer);

						transitionInfo.image = offscreenTexture.image;
						transitionInfo.format = offscreenTexture.format;
						transitionInfo.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
						transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
						transitionInfo.mipLevels = 1;
						transitionInfo.layerCount = 1;
						VulkanUtilities::transitionImageLayout(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, transitionInfo);

						// Copy region for transfer from framebuffer to cube face
						VkImageCopy copyRegion = {};

						copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						copyRegion.srcSubresource.baseArrayLayer = 0;
						copyRegion.srcSubresource.mipLevel = 0;
						copyRegion.srcSubresource.layerCount = 1;
						copyRegion.srcOffset = { 0, 0, 0 };

						copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						copyRegion.dstSubresource.baseArrayLayer = f;
						copyRegion.dstSubresource.mipLevel = m;
						copyRegion.dstSubresource.layerCount = 1;
						copyRegion.dstOffset = { 0, 0, 0 };

						copyRegion.extent.width = static_cast<UI32>(viewport.width);
						copyRegion.extent.height = static_cast<UI32>(viewport.height);
						copyRegion.extent.depth = 1;

						{
							VulkanOneTimeCommandBuffer oneTimeBuffer(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue);

							vkCmdCopyImage(
								oneTimeBuffer.buffer,
								offscreenTexture.image,
								VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
								IrradianceCube.image,
								VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
								1,
								&copyRegion);
						}

						// Transform framebuffer color attachment back 
						transitionInfo.image = offscreenTexture.image;
						transitionInfo.format = offscreenTexture.format;
						transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
						transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
						transitionInfo.mipLevels = 1;
						transitionInfo.layerCount = 1;
						VulkanUtilities::transitionImageLayout(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, transitionInfo);
					}
				}

				transitionInfo.image = IrradianceCube.image;
				transitionInfo.format = IrradianceCube.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				transitionInfo.mipLevels = IrradianceCube.mipLevels;
				transitionInfo.layerCount = 6;
				VulkanUtilities::transitionImageLayout(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, transitionInfo);
			}

			vkQueueWaitIdle(myGraphicsCore.presentQueue);
		}

		void VulkanRBL::initializePrefilteredCube()
		{
			VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;
			const I32 dim = 512;

			VulkanTextureContainer PrefilteredCube;
			PrefilteredCube.mipLevels = static_cast<uint32_t>(floor(log2(dim))) + 1;
			PrefilteredCube.format = format;
			PrefilteredCube.width = dim;
			PrefilteredCube.height = dim;

			VulkanCreateImageInfo imageInfo;
			imageInfo.width = dim;
			imageInfo.height = dim;
			imageInfo.format = format;
			imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			imageInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			imageInfo.image = &PrefilteredCube.image;
			imageInfo.imageMemory = &PrefilteredCube.imageMemory;
			imageInfo.mipLevels = PrefilteredCube.mipLevels;
			imageInfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.arrayLayers = 6;
			imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
			VulkanUtilities::createImage(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, imageInfo);

			VulkanTextureSamplerInitInfo samplerInitInfo;
			samplerInitInfo.magFilter = VK_FILTER_LINEAR;
			samplerInitInfo.minFilter = VK_FILTER_LINEAR;
			samplerInitInfo.maxMipLevels = 1.0f;
			samplerInitInfo.minMipLevels = PrefilteredCube.mipLevels;
			samplerInitInfo.modeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInitInfo.modeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInitInfo.modeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInitInfo.mipLoadBias = 0.0f;
			samplerInitInfo.mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInitInfo.compareOp = VK_COMPARE_OP_NEVER;
			samplerInitInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
			samplerInitInfo.maxAnisotrophy = 1.0f;
			PrefilteredCube.imageSampler = VulkanUtilities::createImageSampler(myGraphicsCore.logicalDevice, samplerInitInfo);

			VulkanCreateImageViewInfo cinfo2;
			cinfo2.image = PrefilteredCube.image;
			cinfo2.format = PrefilteredCube.format;
			cinfo2.mipLevels = PrefilteredCube.mipLevels;
			cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			cinfo2.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
			cinfo2.layerCount = 6;
			PrefilteredCube.imageView = VulkanUtilities::createImageView(myGraphicsCore.logicalDevice, cinfo2);

			prefilteredCube.textures.push_back(PrefilteredCube);

			VkAttachmentDescription attDesc = {};
			// Color attachment
			attDesc.format = format;
			attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
			attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

			VkSubpassDescription subpassDescription = {};
			subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpassDescription.colorAttachmentCount = 1;
			subpassDescription.pColorAttachments = &colorReference;

			// Use subpass dependencies for layout transitions
			std::array<VkSubpassDependency, 2> dependencies;
			dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[0].dstSubpass = 0;
			dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			dependencies[1].srcSubpass = 0;
			dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			VulkanRenderContext _context;
			VulkanRenderPassInitInfo renderPassInitInfo;
			renderPassInitInfo.additionalSubPassDependencies = { dependencies[0], dependencies[1] };
			renderPassInitInfo.attachments = { attDesc };
			renderPassInitInfo.subPasses = { subpassDescription };
			renderPassInitInfo.overrideDependencies = true;
			_context.renderPass.initialize(myGraphicsCore.logicalDevice, renderPassInitInfo);

			VulkanTextureContainer offscreenTexture;
			offscreenTexture.mipLevels = 1.0f;
			offscreenTexture.format = format;
			offscreenTexture.width = dim;
			offscreenTexture.height = dim;
			{
				VulkanCreateImageInfo imageInfo;
				imageInfo.width = dim;
				imageInfo.height = dim;
				imageInfo.format = format;
				imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
				imageInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				imageInfo.image = &offscreenTexture.image;
				imageInfo.imageMemory = &offscreenTexture.imageMemory;
				imageInfo.mipLevels = offscreenTexture.mipLevels;
				imageInfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
				imageInfo.arrayLayers = 1;
				imageInfo.flags = NULL;
				VulkanUtilities::createImage(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, imageInfo);

				VulkanCreateImageViewInfo cinfo2;
				cinfo2.image = offscreenTexture.image;
				cinfo2.format = offscreenTexture.format;
				cinfo2.mipLevels = offscreenTexture.mipLevels;
				cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				cinfo2.viewType = VK_IMAGE_VIEW_TYPE_2D;
				cinfo2.layerCount = 1;
				offscreenTexture.imageView = VulkanUtilities::createImageView(myGraphicsCore.logicalDevice, cinfo2);

				VulkanGraphicsFrameBufferInitInfo frameBufferInitInfo;
				frameBufferInitInfo.attachments = { offscreenTexture.imageView };
				frameBufferInitInfo.renderPass = _context.renderPass.renderPass;
				frameBufferInitInfo.bufferCount = 1;
				frameBufferInitInfo.swapChainExtent = { dim, dim };
				_context.frameBuffer.initialize(myGraphicsCore.logicalDevice, frameBufferInitInfo);

				VulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = offscreenTexture.image;
				transitionInfo.format = offscreenTexture.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				transitionInfo.mipLevels = offscreenTexture.mipLevels;
				transitionInfo.layerCount = 1;
				VulkanUtilities::transitionImageLayout(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, transitionInfo);
			}

			struct PushBlock {
				glm::mat4 mvp;
				// Sampling deltas
				float roughness;
				uint32_t numSamples = 32u;
			} pushBlock;

			VulkanGraphicsPipeline pipeline;
			VkPushConstantRange _range;
			_range.offset = 0;
			_range.size = sizeof(pushBlock);
			_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

			VulkanGraphicsPipelineLayoutInitInfo pipelineLayoutInitInfo;
			pipelineLayoutInitInfo.layouts = { skyboxObject->renderObject[0].descriptors.layout };
			pipelineLayoutInitInfo.pushConstantRanges = { _range };
			pipeline.initializePipelineLayout(myGraphicsCore.logicalDevice, pipelineLayoutInitInfo);

			ShaderPaths _shaderPaths;
			_shaderPaths.vertexShader = "Shaders/PrefilteredCube/vert.spv";
			_shaderPaths.fragmentShader = "Shaders/PrefilteredCube/frag.spv";
			auto _shaders = VulkanUtilities::getGraphicsShaders(myGraphicsCore.logicalDevice, _shaderPaths);

			DMKVertexAttribute _attribute;
			_attribute.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
			_attribute.name = DMKVertexData::DMK_VERTEX_DATA_POSITION;

			VulkanGraphicsPipelineInitInfo pipelineInitInfo = VulkanPresets::pipelinePreset3D(
				VK_SAMPLE_COUNT_1_BIT,
				_context.renderPass.renderPass,
				_shaders, { _attribute }, { dim, dim });
			pipelineInitInfo.dynamicStateEnable = true;
			pipelineInitInfo.depthStencilCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
			pipelineInitInfo.rasterizerCullMode = VK_CULL_MODE_NONE;
			pipelineInitInfo.rasterizerFrontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			pipelineInitInfo.swapChainExtent = { dim, dim };
			pipeline.initializePipeline(myGraphicsCore.logicalDevice, pipelineInitInfo);

			/* Command buffer */
			{
				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = _context.renderPass.renderPass;
				renderPassInfo.framebuffer = _context.frameBuffer.buffers[0];
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = { dim, dim };

				std::array<VkClearValue, 2> clearValues = {};
				clearValues[0].color = {
					0.0f, 0.0f, 0.0f, 0.0f
				};
				renderPassInfo.clearValueCount = static_cast<UI32>(clearValues.size());
				renderPassInfo.pClearValues = clearValues.data();

				std::vector<glm::mat4> matrices = {
					// POSITIVE_X
					glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// NEGATIVE_X
					glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// POSITIVE_Y
					glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// NEGATIVE_Y
					glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// POSITIVE_Z
					glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
					// NEGATIVE_Z
					glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
				};

				VulkanOneTimeCommandBuffer cmdBuf(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue);

				VkViewport viewport;
				viewport.width = (F32)dim;
				viewport.height = (F32)dim;
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;
				viewport.x = 0;
				viewport.y = 0;

				VkRect2D scissor;
				scissor.extent = { dim, dim };
				scissor.offset = { 0, 0 };

				vkCmdSetViewport(cmdBuf.buffer, 0, 1, &viewport);
				vkCmdSetScissor(cmdBuf.buffer, 0, 1, &scissor);

				VkImageSubresourceRange subresourceRange = {};
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = PrefilteredCube.mipLevels;
				subresourceRange.layerCount = 6;

				VulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = PrefilteredCube.image;
				transitionInfo.format = PrefilteredCube.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.mipLevels = PrefilteredCube.mipLevels;
				transitionInfo.layerCount = 6;
				VulkanUtilities::transitionImageLayout(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, transitionInfo);

				for (uint32_t m = 0; m < PrefilteredCube.mipLevels; m++) {
					for (uint32_t f = 0; f < 6; f++) {
						viewport.width = static_cast<float>(dim * std::pow(0.5f, m));
						viewport.height = static_cast<float>(dim * std::pow(0.5f, m));
						vkCmdSetViewport(cmdBuf.buffer, 0, 1, &viewport);

						// Render scene from cube face's point of view
						vkCmdBeginRenderPass(cmdBuf.buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

						// Update shader push constant block
						float FOV = M_PI / 2.0f;
						pushBlock.mvp = glm::perspective(FOV, 1.0f, 0.1f, 512.0f) * matrices[f];

						vkCmdPushConstants(cmdBuf.buffer, pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushBlock), &pushBlock);

						vkCmdBindPipeline(cmdBuf.buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

						for (auto _object : skyboxObject->renderObject)
						{
							for (VkDescriptorSet _set : _object.descriptors.descriptorSets)
								vkCmdBindDescriptorSets(cmdBuf.buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 1, &_set, 0, NULL);

							VkDeviceSize offsets[1] = { 0 };

							for (UI32 itr = 0; itr < _object.vertexBufferContainer.size(); itr++)
							{
								vkCmdBindVertexBuffers(cmdBuf.buffer, 0, 1, &_object.vertexBufferContainer[itr].buffer, offsets);
								vkCmdBindIndexBuffer(cmdBuf.buffer, _object.indexBufferContainer[itr].buffer, 0, VK_INDEX_TYPE_UINT32);
								vkCmdDrawIndexed(cmdBuf.buffer, _object.indexBufferContainer[itr].dataCount, 1, 0, 0, 0);
							}
						}

						vkCmdEndRenderPass(cmdBuf.buffer);

						transitionInfo.image = offscreenTexture.image;
						transitionInfo.format = offscreenTexture.format;
						transitionInfo.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
						transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
						transitionInfo.mipLevels = 1;
						transitionInfo.layerCount = 1;
						VulkanUtilities::transitionImageLayout(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, transitionInfo);

						// Copy region for transfer from framebuffer to cube face
						VkImageCopy copyRegion = {};

						copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						copyRegion.srcSubresource.baseArrayLayer = 0;
						copyRegion.srcSubresource.mipLevel = 0;
						copyRegion.srcSubresource.layerCount = 1;
						copyRegion.srcOffset = { 0, 0, 0 };

						copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						copyRegion.dstSubresource.baseArrayLayer = f;
						copyRegion.dstSubresource.mipLevel = m;
						copyRegion.dstSubresource.layerCount = 1;
						copyRegion.dstOffset = { 0, 0, 0 };

						copyRegion.extent.width = static_cast<UI32>(viewport.width);
						copyRegion.extent.height = static_cast<UI32>(viewport.height);
						copyRegion.extent.depth = 1;

						{
							VulkanOneTimeCommandBuffer oneTimeBuffer(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue);

							vkCmdCopyImage(
								oneTimeBuffer.buffer,
								offscreenTexture.image,
								VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
								PrefilteredCube.image,
								VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
								1,
								&copyRegion);
						}

						// Transform framebuffer color attachment back 
						transitionInfo.image = offscreenTexture.image;
						transitionInfo.format = offscreenTexture.format;
						transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
						transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
						transitionInfo.mipLevels = 1;
						transitionInfo.layerCount = 1;
						VulkanUtilities::transitionImageLayout(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, transitionInfo);
					}
				}

				transitionInfo.image = PrefilteredCube.image;
				transitionInfo.format = PrefilteredCube.format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				transitionInfo.mipLevels = PrefilteredCube.mipLevels;
				transitionInfo.layerCount = 6;
				VulkanUtilities::transitionImageLayout(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, transitionInfo);
			}

			vkQueueWaitIdle(myGraphicsCore.presentQueue);
		}

		void VulkanRBL::generateBoundingBoxes()
		{
			DMK_CORE_INFO("Generating bounding boxes! Binding context is set to default.");
			auto _context = myRenderContexts[0];

			myBoundingBox.initialize();
		}

		void VulkanRBL::initializeCommands()
		{
			DMK_BEGIN_PROFILE_TIMER();

			for (UI32 _itr = 0; _itr < myRenderContexts.size(); _itr++)
			{
				if (myRenderContexts[_itr].state != VulkanResourceState::ADGR_VULKAN_RESOURCE_STATE_HOST_VISIBLE)
					continue;

				myRenderContexts[_itr].inFlight = myRenderContexts[_itr].renderDatas;
				myRenderContexts[_itr].renderDatas.clear();

				VulkanGraphicsCommandBufferInitInfo initInfo;
				initInfo.count = myRenderContexts[_itr].swapChain.swapChainImages.size();
				initInfo.frameBuffer = myRenderContexts[_itr].frameBuffer;
				initInfo.swapChain = myRenderContexts[_itr].swapChain;
				initInfo.objects = myRenderContexts[_itr].inFlight;
				initInfo.renderPass = myRenderContexts[_itr].renderPass;
				//initInfo.captureFrames = true;
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
			if (info.formats.size() != _renderContext.inFlight.size())
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
					if ((info.formats[index]->descriptor.uniformBufferObjectDescriptions[_itr].type != DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT) &&
						(info.formats[index]->descriptor.uniformBufferObjectDescriptions[_itr].type != DMKUniformType::DMK_UNIFORM_TYPE_CONSTANT))
						continue;

					/* Update the objects uniform buffer memory */
					for (auto _object : _renderContext.inFlight[index].renderObject)
					{
						if (_object.type == DMKObjectType::DMK_OBJECT_TYPE_BOUNDING_BOX)
						{
							info.formats[index]->checkRayCollition(info.cameraData.rayOrigin, info.cameraData.rayDirection, _object.limits);

							VulkanUtilities::updateBoundingBox(
								myGraphicsCore.logicalDevice,
								info.formats[index]->onUpdate(info.cameraData),
								_object.uniformBufferContainers[_itr].bufferMemories[imageIndex],
								_object.limits,
								info.formats[index]->descriptor.transformDescriptor.location,
								info.formats[index]->isSelected);
						}
						else if (info.formats[index]->type == DMKObjectType::DMK_OBJECT_TYPE_SKELETAL_ANIMATION)
						{
							for (auto _animation : info.formats[index]->animation)
							{
								_animation.update(runningTime);

								boneTransforms.clear();
								for (auto _transform : _animation.boneTransforms)
									boneTransforms.push_back(glm::make_mat4(&_transform.a1));

								VulkanUtilities::updateUniformBuffer(
									myGraphicsCore.logicalDevice,
									info.formats[index]->onUpdate(info.cameraData, boneTransforms),
									_object.uniformBufferContainers[_itr].bufferMemories[imageIndex]);
							}
						}
						else
						{
							VulkanUtilities::updateUniformBuffer(
								myGraphicsCore.logicalDevice,
								info.formats[index]->onUpdate(info.cameraData),
								_object.uniformBufferContainers[_itr].bufferMemories[imageIndex]);
						}
					}
				}
			}

			/* Submit queues */
			result = myGraphicsCore.submitQueues(
				{ _renderContext.swapChain.swapChain },
				imageIndex, currentFrame,
				{ _renderContext.inFlightCommandBuffer.buffers[imageIndex] });

#ifdef DMK_DEBUG
			//_storeData(_renderContext.inFlightCommandBuffer.getImage(imageIndex));

#endif // DMK_DEBUG


			/* Check for any errors */
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				recreateSwapChain();
			}
			else if (result != VK_SUCCESS)
				DMK_CORE_FATAL("failed to present swap chain image!");

			/* Update the current frame number */
			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
			runningTime++;
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

			std::vector<VkAttachmentDescription> attachments;

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

			std::vector<VkSubpassDescription> subPasses;
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

		VulkanBufferContainer VulkanRBL::createVertexBuffer(Mesh mesh, std::vector<DMKVertexAttribute> attributes)
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

		inline MeshVertexLimits VulkanRBL::getVertexLimits(Mesh mesh)
		{
			MeshVertexLimits _limits;
			_limits.limitX.x = mesh.vertexDataStore[0].position.x;
			_limits.limitX.y = mesh.vertexDataStore[0].position.x;
			_limits.limitY.x = mesh.vertexDataStore[0].position.y;
			_limits.limitY.y = mesh.vertexDataStore[0].position.y;
			_limits.limitZ.x = mesh.vertexDataStore[0].position.z;
			_limits.limitZ.y = mesh.vertexDataStore[0].position.z;

			for (auto _vertex : mesh.vertexDataStore)
			{
				if (_vertex.position.x < _limits.limitX.x) _limits.limitX.x = _vertex.position.x;
				if (_vertex.position.x > _limits.limitX.y) _limits.limitX.y = _vertex.position.x;
				if (_vertex.position.y < _limits.limitY.x) _limits.limitY.x = _vertex.position.y;
				if (_vertex.position.y > _limits.limitY.y) _limits.limitY.y = _vertex.position.y;
				if (_vertex.position.z < _limits.limitZ.x) _limits.limitZ.x = _vertex.position.z;
				if (_vertex.position.z > _limits.limitZ.y) _limits.limitZ.y = _vertex.position.z;
			}

			return _limits;
		}

		inline VulkanRenderObject VulkanRBL::createBoundingBox(Mesh mesh, VulkanRenderContext context, F32 hitboxRadius)
		{
			VulkanRenderObject _object;
			_object.type = DMKObjectType::DMK_OBJECT_TYPE_BOUNDING_BOX;

			Mesh _meshObject;
			_meshObject.vertexDataStore = _generateBoundingMesh(hitboxRadius);
			_meshObject.indexes = _generateBoundingMeshIndices();

			std::vector<DMKVertexAttribute> _attributes;
			DMKVertexAttribute _attribute;
			_attribute.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
			_attribute.name = DMKVertexData::DMK_VERTEX_DATA_POSITION;
			_attributes.push_back(_attribute);

			_attribute.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
			_attribute.name = DMKVertexData::DMK_VERTEX_DATA_COLOR;
			_attributes.push_back(_attribute);

			_object.vertexBufferContainer.push_back(createVertexBuffer(_meshObject, _attributes));
			_object.indexBufferContainer.push_back(createIndexBuffer(_meshObject, DMKDataType::DMK_DATA_TYPE_UI32));

			DMKUniformBufferObjectDescriptor UBODescriptor;
			UBODescriptor.type = DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT;
			UBODescriptor.location = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX;
			UBODescriptor.binding = 0;

			DMKUniformAttribute uAttribute1;
			uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_MODEL;
			uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
			UBODescriptor.attributes.push_back(uAttribute1);

			DMKUniformAttribute uAttribute2;
			uAttribute2.name = DMKUniformData::DMK_UNIFORM_DATA_VIEW;
			uAttribute2.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
			UBODescriptor.attributes.push_back(uAttribute2);

			DMKUniformAttribute uAttribute3;
			uAttribute3.name = DMKUniformData::DMK_UNIFORM_DATA_PROJECTION;
			uAttribute3.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
			UBODescriptor.attributes.push_back(uAttribute3);

			_object.uniformBufferContainers.push_back(createUniformBuffers(UBODescriptor, context.swapChain.swapChainImages.size()));

			_object.descriptors = createDescriptors({ UBODescriptor }, _object.uniformBufferContainers, {});

			ShaderPaths _paths;
			_paths.vertexShader = "Shaders/BoundingBox/vert.spv";
			_paths.fragmentShader = "Shaders/BoundingBox/frag.spv";
			_object.pipeline = createPipeline({ _object.descriptors }, { UBODescriptor }, _attributes, _paths, DMKObjectType::DMK_OBJECT_TYPE_BOUNDING_BOX, context);

			_object.limits = getVertexLimits(mesh);

			return _object;
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

		VulkanGraphicsDescriptor VulkanRBL::createDescriptors(std::vector<DMKUniformBufferObjectDescriptor> descriptors, std::vector<VulkanUnformBufferContainer> uniformBufferContainers, std::vector<VulkanTextureContainer> textureContainers)
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
			std::vector<std::vector<VkDescriptorBufferInfo>> bufferInfos;
			std::vector<VkWriteDescriptorSet> _writes(layoutInitInfo.bindings.size(), VkWriteDescriptorSet());

			UI32 uniformIndex = 0;
			UI32 textureIndex = 0;
			for (UI32 binding = 0; binding < descriptors.size(); binding++)
			{
				switch (descriptors[binding].type)
				{
					/* Initialize Uniform buffer descriptor */
				case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT:
					bufferInfos.push_back(std::vector<VkDescriptorBufferInfo>());
					for (UI32 i = 0; i < uniformBufferContainers[uniformIndex].buffers.size(); i++)
					{
						VkDescriptorBufferInfo bufferInfo = {};
						bufferInfo.buffer = uniformBufferContainers[uniformIndex].buffers[i];
						bufferInfo.offset = 0;
						bufferInfo.range = DMKUniformBufferObjectDescriptor::uniformByteSize(descriptors[binding].attributes);
						bufferInfos[uniformIndex].push_back(bufferInfo);
					}

					VkWriteDescriptorSet _writes1;
					_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes1.dstBinding = descriptors[binding].binding;
					_writes1.dstArrayElement = 0;
					_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					_writes1.descriptorCount = 1;
					_writes1.pBufferInfo = bufferInfos[uniformIndex].data();
					_writes1.pNext = VK_NULL_HANDLE;
					_writes1.pImageInfo = VK_NULL_HANDLE;
					_writes1.pTexelBufferView = VK_NULL_HANDLE;
					_writes[binding] = _writes1;

					uniformIndex++;
					break;

				case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_CONSTANT:
					continue;
					break;

				default:
					VkDescriptorImageInfo imageInfo = {};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = textureContainers[textureIndex].imageView;
					imageInfo.sampler = textureContainers[textureIndex].imageSampler;

					VkWriteDescriptorSet _writes2;
					_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes2.dstBinding = descriptors[binding].binding;
					_writes2.dstArrayElement = 0;
					_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_writes2.descriptorCount = 1;
					_writes2.pImageInfo = &imageInfo;
					_writes2.pNext = VK_NULL_HANDLE;
					_writes2.pTexelBufferView = VK_NULL_HANDLE;
					_writes2.pBufferInfo = VK_NULL_HANDLE;
					_writes[binding] = _writes2;

					textureIndex++;
					break;
				}
			}

			VulkanDescriptorSetsInitInfo setInitInfo;
			setInitInfo.descriptorWrites = _writes;
			_descriptor.initializeSets(myGraphicsCore.logicalDevice, setInitInfo);

			return _descriptor;
		}

		VulkanGraphicsPipeline VulkanRBL::createPipeline(
			std::vector<VulkanGraphicsDescriptor> descriptors,
			std::vector<DMKUniformBufferObjectDescriptor> uniformBufferDescriptors,
			std::vector<DMKVertexAttribute> attributes,
			ShaderPaths paths,
			DMKObjectType objectType,
			VulkanRenderContext context)
		{
			VulkanGraphicsPipeline _pipeline;

			/* Initialize pipeline layout */
			VulkanGraphicsPipelineLayoutInitInfo layoutInitInfo;
			for (auto _descriptor : descriptors)
				layoutInitInfo.layouts.push_back(_descriptor.layout);

			UI32 _oldRangeSize = 0;
			for (auto _description : uniformBufferDescriptors)
			{
				if (_description.type != DMKUniformType::DMK_UNIFORM_TYPE_CONSTANT)
					continue;

				VkPushConstantRange _range;
				_range.stageFlags = VulkanUtilities::getShaderStage(_description.location);
				_range.size = DMKUniformBufferObjectDescriptor::uniformByteSize(_description.attributes);
				_range.offset = _description.offset;
				layoutInitInfo.pushConstantRanges.push_back(_range);

				_oldRangeSize += _range.size;
			}
			_pipeline.initializePipelineLayout(myGraphicsCore.logicalDevice, layoutInitInfo);

			/* Initialize pipeline */
			auto _shaders = VulkanUtilities::getGraphicsShaders(myGraphicsCore.logicalDevice, paths);
			switch (objectType)
			{
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_IMAGE_2D:
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_BOUNDING_BOX:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePresetBoundingBox(
						myMsaaSamples,
						context.renderPass.renderPass,
						_shaders,
						attributes,
						context.swapChain.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
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

		void VulkanRBL::_storeData(VulkanTextureContainer container)
		{
			std::ofstream _outputFile(std::string("Captures/capture_") + std::to_string(_captureCount) + std::string(".raw"), std::ostream::binary);

			VPTR data = nullptr;
			if (vkMapMemory(myGraphicsCore.logicalDevice, container.imageMemory, 0, container.width * container.height * 4, 0, &data) != VK_SUCCESS)
				DMK_CORE_FATAL("Unable to map image memory!");
			_outputFile.write((CPTR)data, container.width * container.height * 4);
			vkUnmapMemory(myGraphicsCore.logicalDevice, container.imageMemory);

			_outputFile.close();
			_captureCount++;
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