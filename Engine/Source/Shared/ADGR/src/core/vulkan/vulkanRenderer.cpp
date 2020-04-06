/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 vulkanRenderer.cpp file
*/

#include "adgrafx.h"
#include "vulkanRenderer.h"
#include "Platform/windows.h"
#include "keyCodes.h"

#include "debugger.h"
#include "CentralDataHub.h"
#include "Platform.h"

#include "Renderer Backend Layer/VulkanSkyBox.h"
#include "Renderer Backend Layer/VulkanReflectObject.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		using namespace core;

#ifdef DMK_DEBUG
		Debugger::benchmark::FPS myFPSCal;

		// ----------
#endif

		// initialize the renderer
		void vulkanRenderer::init() {	// 14 inc
			initStageOne();
			initStageTwo();
			initStageThree();
		}

		// Basic one-time initializations
		void vulkanRenderer::initStageOne()
		{
			// initialize the window
			DMKWindowManagerInitInfo windowInitInfo;
			windowInitInfo.iconPaths = "E:/Projects/Dynamik Engine/Versions/Dynamik (Prototype)/Dependencies/Assets/icons/Dynamik.jpg";
			myWindowManager.initialize(windowInitInfo);

			// initialize the instance
			ADGRVulkanInstanceInitInfo instanceInitInfo;
			instanceInitInfo.applicationName = "Dynamik Engine";
			instanceInitInfo.engineName = "Dynamik";
			myVulkanCore.initializeInstance(instanceInitInfo);
			myVulkanCore.initializeSurface(&myWindowManager.window);

			myVulkanCore.initializeDevice();

			myVulkanCore.initializeCommandPool();

			initializeSwapChain();

			initializeColorBuffer();
			initializeDepthBuffer();

			ADGRVulkanFrameBufferInitInfo frameBufferInitInfo;
			frameBufferInitInfo.preAttachments = { myColorBuffer.imageView, myDepthBuffer.imageView };
			mySwapChain3D.initializeFrameBuffers(frameBufferInitInfo);

			ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
			mySwapChain3D.initializeDescriptorSetLayout(layoutInitInfo);

			ADGRVulkanPipelineLayoutInitInfo pipelineLayoutInitInfo;
			pipelineLayoutInitInfo.pushConstantCount = 6;
			pipelineLayoutInitInfo.pushConstantOffset = 0;
			pipelineLayoutInitInfo.pushConstantsEnable = true;
			mySwapChain3D.initializePipelineLayout(pipelineLayoutInitInfo);
		}

		// object loading and initialization
		void vulkanRenderer::initStageTwo() {
			initializeObjects();

			ADGRVulkanCommandBufferInitInfo commandBufferInitInfo;
			commandBufferInitInfo.objects = renderDatas;
			commandBufferInitInfo.swapChain = mySwapChain3D.swapChainContainer;
			myVulkanCore.initializeCommandBuffers(commandBufferInitInfo);
		}

		// final initialization
		void vulkanRenderer::initStageThree() {
			myVulkanCore.initializeSyncObjects();
		}

		// shutdown the renderer
		void vulkanRenderer::shutDown() {
			shutDownStageOne();
			shutDownStageTwo();
			shutDownStageThree();
		}

		// basic one-time shut down functions
		void vulkanRenderer::shutDownStageOne() {
			myDepthBuffer.terminate(myVulkanCore.getLogicalDevice());
			myColorBuffer.terminate(myVulkanCore.getLogicalDevice());

			myVulkanCore.terminateCommandBuffers();

			mySwapChain3D.terminate();
		}

		// per object shut down functions
		void vulkanRenderer::shutDownStageTwo() {
			for (ADGRVulkanRenderData& _objectData : renderDatas)
			{
				VulkanRenderableObject _object(RenderableObjectInitInfo()
					);

				_object.setRenderData(_objectData);

				_object.terminatePipeline();

				_object.terminateUniformBuffer();

				_object.terminateTextures();

				_object.terminateIndexBuffer();

				_object.terminateVertexBuffer();
			}
		}

		// final shut down functions
		void vulkanRenderer::shutDownStageThree()
		{
			myVulkanCore.terminateSyncObjects();

			myVulkanCore.terminateDevice();

			myVulkanCore.terminateDebugger();

			myVulkanCore.terminateInstance();

			myWindowManager.terminate();
		}

		// draw frame
		void vulkanRenderer::drawFrame() {
#ifdef DMK_DEBUG
			myFPSCal.getFPS();	// FPS calculator

 // ----------
#endif
			myWindowManager.pollEvents();
			eventContainer = myWindowManager.getEventContainer();

			myVulkanCore.syncFence(currentFrame);

			//draw2D(mySwapChain2D.getSwapChain());
			draw3D(mySwapChain3D.swapChainContainer.getSwapChain());

			// current frame select
			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

			if (myWindowManager.isFrameBufferResized())
			{
				myWindowManager.frameBufferResizedUpdate(false);
				recreateSwapChain();
			}
		}

		// recreate the swapchain
		void vulkanRenderer::recreateSwapChain() {
			shutDownStageOne();

			myVulkanCore.initializeCommandPool();

			for (ADGRVulkanRenderData& _objectData : renderDatas)
			{
				VulkanRenderableObject _object(RenderableObjectInitInfo()
					);

				_object.setRenderData(_objectData);

				_object.terminatePipeline();

				_object.terminateUniformBuffer();
			}

			initializeSwapChain();

			initializeColorBuffer();
			initializeDepthBuffer();

			ADGRVulkanFrameBufferInitInfo frameBufferInitInfo;
			frameBufferInitInfo.preAttachments = { myColorBuffer.imageView, myDepthBuffer.imageView };
			mySwapChain3D.initializeFrameBuffers(frameBufferInitInfo);

			ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
			mySwapChain3D.initializeDescriptorSetLayout(layoutInitInfo);

			ADGRVulkanPipelineLayoutInitInfo pipelineLayoutInitInfo;
			pipelineLayoutInitInfo.pushConstantCount = 6;
			pipelineLayoutInitInfo.pushConstantOffset = 0;
			pipelineLayoutInitInfo.pushConstantsEnable = true;
			mySwapChain3D.initializePipelineLayout(pipelineLayoutInitInfo);

			initializeObjectsBasic();

			ADGRVulkanCommandBufferInitInfo commandBufferInitInfo;
			commandBufferInitInfo.objects = renderDatas;
			commandBufferInitInfo.swapChain = mySwapChain3D.swapChainContainer;
			myVulkanCore.initializeCommandBuffers(commandBufferInitInfo);
		}

		void vulkanRenderer::initializeObjects()
		{
			for (ADGRVulkan3DObjectData _object : rawObjects)
			{
				if (_object.type == DMKObjectType::DMK_OBJECT_TYPE_CAMERA)
				{
					VulkanRenderableObject _renderObject(RenderableObjectInitInfo()
						);

					_renderObject.setSwapChainContainer(&mySwapChain3D.swapChainContainer);

					ARRAY<VulkanShader> _shaders;

					if (_object.vertexShaderPath.size() && _object.vertexShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.vertexShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
						_shaders.pushBack(_shader);
					}
					if (_object.tessellationShaderPath.size() && _object.tessellationShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.tessellationShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
						_shaders.pushBack(_shader);
					}
					if (_object.geometryShaderPath.size() && _object.geometryShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.geometryShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
						_shaders.pushBack(_shader);
					}
					if (_object.fragmentShaderPath.size() && _object.fragmentShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.fragmentShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
						_shaders.pushBack(_shader);
					}

					// initialize pipeline
					ADGRVulkanPipelineInitInfo pipelineInitInfo;
					pipelineInitInfo.shaders = _shaders;
					pipelineInitInfo.multisamplerMsaaSamples = myVulkanCore.getMsaaSamples();
					pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
					pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
					_renderObject.initializePipeline(pipelineInitInfo);

					for (VulkanShader _shader : _shaders)
						_shader.terminate(myVulkanCore.getLogicalDevice());

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

					_renderObject.initializeTextures(textureInitInfos);

					// initialize vertex buffers
					for (UI32 _itr = 0; _itr < _object.vertexBufferObjects->size(); _itr++)
						_renderObject.initializeVertexBuffer(&_object.vertexBufferObjects->at(_itr));

					// initialize index buffers
					for (UI32 _itr = 0; _itr < _object.indexBufferObjects->size(); _itr++)
						_renderObject.initializeIndexBufferUI32(&_object.indexBufferObjects->at(_itr));

					// initialize uniform buffers
					_renderObject.initializeUniformBuffer();

					// initialize descriptor pool
					ADGRVulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
					_renderObject.initializeDescriptorPool(descriptorPoolInitInfo);

					// initialize descriptor sets
					ADGRVulkanDescriptorSetsInitInfo descriptorSetsInitInfo;
					_renderObject.initializeDescriptorSets(descriptorSetsInitInfo);

					renderDatas.push_back(_renderObject.getRenderData());
				}
				else if (_object.type == DMKObjectType::DMK_OBJECT_TYPE_SKYBOX)
				{
					VulkanSkyBox _renderObject(RenderableObjectInitInfo()
						);

					_renderObject.setSwapChainContainer(&mySwapChain3D.swapChainContainer);

					ARRAY<VulkanShader> _shaders;

					if (_object.vertexShaderPath.size() && _object.vertexShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.vertexShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
						_shaders.pushBack(_shader);
					}
					if (_object.tessellationShaderPath.size() && _object.tessellationShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.tessellationShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
						_shaders.pushBack(_shader);
					}
					if (_object.geometryShaderPath.size() && _object.geometryShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.geometryShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
						_shaders.pushBack(_shader);
					}
					if (_object.fragmentShaderPath.size() && _object.fragmentShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.fragmentShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
						_shaders.pushBack(_shader);
					}

					// initialize pipeline
					ADGRVulkanPipelineInitInfo pipelineInitInfo;
					pipelineInitInfo.shaders = _shaders;
					pipelineInitInfo.multisamplerMsaaSamples = myVulkanCore.getMsaaSamples();
					pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
					pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
					_renderObject.initializePipeline(pipelineInitInfo);

					for (VulkanShader _shader : _shaders)
						_shader.terminate(myVulkanCore.getLogicalDevice());

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

					_renderObject.initializeTextures(textureInitInfos);

					// initialize vertex buffers
					for (UI32 _itr = 0; _itr < _object.vertexBufferObjects->size(); _itr++)
						_renderObject.initializeVertexBuffer(&_object.vertexBufferObjects->at(_itr));

					// initialize index buffers
					for (UI32 _itr = 0; _itr < _object.indexBufferObjects->size(); _itr++)
						_renderObject.initializeIndexBufferUI32(&_object.indexBufferObjects->at(_itr));

					// initialize uniform buffers
					_renderObject.initializeUniformBuffer();

					// initialize descriptor pool
					ADGRVulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
					_renderObject.initializeDescriptorPool(descriptorPoolInitInfo);

					// initialize descriptor sets
					ADGRVulkanDescriptorSetsInitInfo descriptorSetsInitInfo;
					_renderObject.initializeDescriptorSets(descriptorSetsInitInfo);

					renderDatas.push_back(_renderObject.getRenderData());
				}
				else if (_object.type == DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT)
				{
					VulkanReflectObject _renderObject(RenderableObjectInitInfo());

					_renderObject.setSwapChainContainer(&mySwapChain3D.swapChainContainer);

					ARRAY<VulkanShader> _shaders;

					if (_object.vertexShaderPath.size() && _object.vertexShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.vertexShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
						_shaders.pushBack(_shader);
					}
					if (_object.tessellationShaderPath.size() && _object.tessellationShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.tessellationShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
						_shaders.pushBack(_shader);
					}
					if (_object.geometryShaderPath.size() && _object.geometryShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.geometryShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
						_shaders.pushBack(_shader);
					}
					if (_object.fragmentShaderPath.size() && _object.fragmentShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.fragmentShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
						_shaders.pushBack(_shader);
					}

					// initialize pipeline
					ADGRVulkanPipelineInitInfo pipelineInitInfo;
					pipelineInitInfo.shaders = _shaders;
					pipelineInitInfo.multisamplerMsaaSamples = myVulkanCore.getMsaaSamples();
					pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
					pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
					_renderObject.initializePipeline(pipelineInitInfo);

					for (VulkanShader _shader : _shaders)
						_shader.terminate(myVulkanCore.getLogicalDevice());

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

					_renderObject.initializeTextures(textureInitInfos);

					// initialize vertex buffers
					for (UI32 _itr = 0; _itr < _object.vertexBufferObjects->size(); _itr++)
						_renderObject.initializeVertexBuffer(&_object.vertexBufferObjects->at(_itr));

					// initialize index buffers
					for (UI32 _itr = 0; _itr < _object.indexBufferObjects->size(); _itr++)
						_renderObject.initializeIndexBufferUI32(&_object.indexBufferObjects->at(_itr));

					// initialize uniform buffers
					_renderObject.initializeUniformBuffer();

					// initialize descriptor pool
					ADGRVulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
					_renderObject.initializeDescriptorPool(descriptorPoolInitInfo);

					// initialize descriptor sets
					ADGRVulkanDescriptorSetsInitInfo descriptorSetsInitInfo;
					_renderObject.initializeDescriptorSets(descriptorSetsInitInfo);

					renderDatas.push_back(_renderObject.getRenderData());
				}
			}
		}

		void vulkanRenderer::initializeObjectsBasic()
		{
			for (ADGRVulkan3DObjectData _object : rawObjects)
			{
				VulkanObject3D _renderObject(RenderableObjectInitInfo());

				_renderObject.setSwapChainContainer(&mySwapChain3D.swapChainContainer);

				ARRAY<VulkanShader> _shaders;

				if (_object.vertexShaderPath.size() && _object.vertexShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.vertexShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

					VulkanShader _shader;
					_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.tessellationShaderPath.size() && _object.tessellationShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.tessellationShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

					VulkanShader _shader;
					_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.geometryShaderPath.size() && _object.geometryShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.geometryShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

					VulkanShader _shader;
					_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.fragmentShaderPath.size() && _object.fragmentShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.fragmentShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

					VulkanShader _shader;
					_shader.initialize(myVulkanCore.getLogicalDevice(), _initInfo);
					_shaders.pushBack(_shader);
				}

				// initialize pipeline
				ADGRVulkanPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = _shaders;
				pipelineInitInfo.multisamplerMsaaSamples = myVulkanCore.getMsaaSamples();
				pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
				pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
				_renderObject.initializePipeline(pipelineInitInfo);

				for (VulkanShader _shader : _shaders)
					_shader.terminate(myVulkanCore.getLogicalDevice());

				// initialize uniform buffers
				_renderObject.initializeUniformBuffer();

				// initialize descriptor pool
				ADGRVulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
				_renderObject.initializeDescriptorPool(descriptorPoolInitInfo);

				// initialize descriptor sets
				ADGRVulkanDescriptorSetsInitInfo descriptorSetsInitInfo;
				_renderObject.initializeDescriptorSets(descriptorSetsInitInfo);

				renderDatas.push_back(_renderObject.getRenderData());
			}
		}

		ADGRVulkanRenderableObjectInitInfo vulkanRenderer::RenderableObjectInitInfo()
		{
			ADGRVulkanRenderableObjectInitInfo _info;
			_info.logicalDevice = myVulkanCore.getLogicalDevice();
			_info.physicalDevice = myVulkanCore.getPhysicalDevice();
			_info.commandPool = myVulkanCore.getCommandPool();
			_info.graphicsQueue = myVulkanCore.getGraphicsQueue();
			_info.presentQueue = myVulkanCore.getPresentQueue();
			return _info;
		}

		void vulkanRenderer::initializeSwapChain()
		{
			mySwapChain3D.basicInitialize(
				myVulkanCore.getLogicalDevice(),
				myVulkanCore.getPhysicalDevice(),
				myVulkanCore.getSurface(),
				myVulkanCore.getSurfaceCapabilities());

			mySwapChain3D.initializeSwapChain(myWindowManager.windowWidth, myWindowManager.windowHeight);

			mySwapChain3D.initializeRenderPass(myVulkanCore.getMsaaSamples());
		}

		void vulkanRenderer::initializeColorBuffer()
		{
			myColorBuffer.initialize(
				myVulkanCore.getLogicalDevice(),
				myVulkanCore.getPhysicalDevice(),
				myVulkanCore.getCommandPool(),
				myVulkanCore.getGraphicsQueue(),
				myVulkanCore.getPresentQueue(),
				mySwapChain3D.swapChainContainer.getSwapChainImageFormat(),
				mySwapChain3D.swapChainContainer.getSwapChainExtent(),
				myVulkanCore.getMsaaSamples());
		}

		void vulkanRenderer::initializeDepthBuffer()
		{
			myDepthBuffer.initialize(
				myVulkanCore.getLogicalDevice(),
				myVulkanCore.getPhysicalDevice(),
				myVulkanCore.getCommandPool(),
				myVulkanCore.getGraphicsQueue(),
				myVulkanCore.getPresentQueue(),
				mySwapChain3D.swapChainContainer.getSwapChainExtent(),
				myVulkanCore.getMsaaSamples());
		}

		void vulkanRenderer::draw3D(VkSwapchainKHR swapChain)
		{
			// get image index
			imageIndex = 0;
			result = myVulkanCore.getNextImage(swapChain, &imageIndex, currentFrame);

			// recreate swachain if needed
			if (result == VK_ERROR_OUT_OF_DATE_KHR) 
			{
				recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
				DMK_CORE_FATAL("failed to acquire swap chain image!");

			// draw call
			// uniform buffer object update
			for (ADGRVulkanRenderData _object : renderDatas)
			{
				if (_object.type == DMKObjectType::DMK_OBJECT_TYPE_SKYBOX)
				{
					VulkanSkyBox _renderObject(RenderableObjectInitInfo());
					_renderObject.setRenderData(_object);

					DMKUpdateInfo info;
					info.aspectRatio = (F32)myWindowManager.windowWidth / (F32)myWindowManager.windowHeight;
					_renderObject.updateUniformBuffer(myCameraSkybox.updateCamera(eventContainer, info), imageIndex);
				}
				else if (_object.type == DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT)
				{
					VulkanRenderableObject _renderObject(RenderableObjectInitInfo());
					_renderObject.setRenderData(_object);

					DMKUpdateInfo info;
					info.aspectRatio = (F32)myWindowManager.windowWidth / (F32)myWindowManager.windowHeight;
					_renderObject.updateUniformBuffer(myCamera3D.updateCamera(eventContainer, info), imageIndex);
				}
			}

			result = myVulkanCore.submitQueues(swapChain, imageIndex, currentFrame);

			// frame buffer resize event
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) 
			{
				recreateSwapChain();
			}
			else if (result != VK_SUCCESS)
				DMK_CORE_FATAL("failed to present swap chain image!");
		}

		// events
		std::deque<DMKEventContainer>* vulkanRenderer::events() {
			return &eventContainer;
		}

		B1 vulkanRenderer::closeEvent()
		{
			return myWindowManager.isWindowCloseEvent;
		}

		void vulkanRenderer::setFormats(ARRAY<RendererFormat>& rendererFormats)
		{
			setFormats3D(rendererFormats);
		}

		void vulkanRenderer::setFormats3D(ARRAY<RendererFormat>& rendererFormats) {
			ARRAY<ADGRVulkan3DObjectData> _objectDatas;

			for (UI32 _itr = 0; _itr < rendererFormats.size(); _itr++)
			{
				ADGRVulkan3DObjectData _object;
				_object.vertexShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.vertexShaderPath;
				_object.tessellationShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.tessellationShaderPath;
				_object.geometryShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.geometryShaderPath;
				_object.fragmentShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.fragmentShaderPath;

				_object.texturePaths = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.texturePaths;

				_object.vertexBufferObjects = &rendererFormats[_itr].myInternalFormat->myVertexBufferObjects;
				_object.indexBufferObjects = &rendererFormats[_itr].myInternalFormat->myIndexBufferObjects;
				_object.type = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.type;

				_objectDatas.pushBack(_object);
			}

			rawObjects = _objectDatas;
		}

		void vulkanRenderer::updateFormats3D(ARRAY<RendererFormat>& rendererFormats)
		{
			setFormats3D(rendererFormats);
		}
	}
}
#endif