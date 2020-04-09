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

#define TEXTOVERLAY_MAX_CHAR_COUNT 2048

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

			ADGRVulkanCommandBufferInitResources initRes;
			initRes.logicalDevice = myVulkanCore.logicalDevice;
			initRes.physicalDevice = myVulkanCore.physicalDevice;
			initRes.surface = myVulkanCore.surface;
			myCommandBuffer.initializeResources(initRes);
			myCommandBuffer.initializeCommandPool();

			initializeSwapChain();

			initializeColorBuffer();
			initializeDepthBuffer();

			ADGRVulkanFrameBufferInitInfo frameBufferInitInfo;
			frameBufferInitInfo.preAttachments = { myColorBuffer.imageView, myDepthBuffer.imageView };
			mySwapChain3D.initializeFrameBuffers(frameBufferInitInfo);

			ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
			mySwapChain3D.initializeDescriptorSetLayout(layoutInitInfo);

			ADGRVulkanPipelineLayoutInitInfo pipelineLayoutInitInfo;
			pipelineLayoutInitInfo.pushConstantCount = 0;
			pipelineLayoutInitInfo.pushConstantOffset = 0;
			pipelineLayoutInitInfo.pushConstantsEnable = false;
			mySwapChain3D.initializePipelineLayout(pipelineLayoutInitInfo);

			_initializeOverlayStageOne();
		}

		// object loading and initialization
		void vulkanRenderer::initStageTwo() {
			initializeObjects();

			ADGRVulkanCommandBufferInitInfo commandBufferInitInfo;
			commandBufferInitInfo.objects = renderDatas;
			commandBufferInitInfo.swapChain = mySwapChain3D.swapChainContainer;
			myCommandBuffer.initializeCommandBuffers(commandBufferInitInfo);
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
			myDepthBuffer.terminate(myVulkanCore.logicalDevice);
			myColorBuffer.terminate(myVulkanCore.logicalDevice);

			myCommandBuffer.terminateCommandBuffers();

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

			myCommandBuffer.initializeCommandPool();

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
			myCommandBuffer.initializeCommandBuffers(commandBufferInitInfo);
		}

		void vulkanRenderer::initializeObjects()
		{
			for (ADGRVulkan3DObjectData _object : rawObjects)
			{
				if (_object.type == DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT)
				{
					VulkanRenderableObject _renderObject(RenderableObjectInitInfo());

					_renderObject.setSwapChainContainer(&mySwapChain3D.swapChainContainer);

					ARRAY<VulkanShader> _shaders;

					if (_object.vertexShaderPath.size() && _object.vertexShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.vertexShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
						_shaders.pushBack(_shader);
					}
					if (_object.tessellationShaderPath.size() && _object.tessellationShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.tessellationShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
						_shaders.pushBack(_shader);
					}
					if (_object.geometryShaderPath.size() && _object.geometryShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.geometryShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
						_shaders.pushBack(_shader);
					}
					if (_object.fragmentShaderPath.size() && _object.fragmentShaderPath != "NONE")
					{
						ADGRVulkanShaderInitInfo _initInfo;
						_initInfo.path = _object.fragmentShaderPath;
						_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

						VulkanShader _shader;
						_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
						_shaders.pushBack(_shader);
					}

					// initialize pipeline
					ADGRVulkanPipelineInitInfo pipelineInitInfo;
					pipelineInitInfo.shaders = _shaders;
					pipelineInitInfo.multisamplerMsaaSamples = myVulkanCore.msaaSamples;
					pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
					pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
					pipelineInitInfo.isTexturesAvailable = _object.texturePaths.size();
					_renderObject.initializePipeline(pipelineInitInfo);

					for (VulkanShader _shader : _shaders)
						_shader.terminate(myVulkanCore.logicalDevice);

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
					renderDatas.push_back(initializeSkyboxObject(_object));
				}
				else if (_object.type == DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT)
				{
					renderDatas.push_back(initializeReflectObject(_object));
				}
				else if (_object.type == DMKObjectType::DMK_OBJECT_TYPE_TEXT_OVERLAY)
				{
					ADGRVulkanShaderPathContainer shaderContainer;
					shaderContainer.vertexShaderPath = _object.vertexShaderPath;
					shaderContainer.tessellationShaderPath = _object.tessellationShaderPath;
					shaderContainer.geometryShaderPath = _object.geometryShaderPath;
					shaderContainer.fragmentShaderPath = _object.fragmentShaderPath;
					_initializeOverlayStageTwo(shaderContainer);
					addText("Hello World", 15.0f, 15.0f, DMKTextAlign::DMK_TEXT_ALIGN_LEFT);
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
					_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.tessellationShaderPath.size() && _object.tessellationShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.tessellationShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

					VulkanShader _shader;
					_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.geometryShaderPath.size() && _object.geometryShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.geometryShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

					VulkanShader _shader;
					_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.fragmentShaderPath.size() && _object.fragmentShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.fragmentShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

					VulkanShader _shader;
					_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}

				// initialize pipeline
				ADGRVulkanPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = _shaders;
				pipelineInitInfo.multisamplerMsaaSamples = myVulkanCore.msaaSamples;
				pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
				pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
				pipelineInitInfo.isTexturesAvailable = _object.texturePaths.size();
				_renderObject.initializePipeline(pipelineInitInfo);

				for (VulkanShader _shader : _shaders)
					_shader.terminate(myVulkanCore.logicalDevice);

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

		void vulkanRenderer::initializeOverlay()
		{

		}

		void vulkanRenderer::addOverlay()
		{
		}

		std::pair<ARRAY<Vertex>, ARRAY<UI32>> vulkanRenderer::_skyBoxTest()
		{
			ARRAY<Vertex> _localVertexBufferObject = {};

			F32 S_SIZE = 5.0f;

			ARRAY<ARRAY<F32>> locations =
			{
				{-S_SIZE, S_SIZE, -S_SIZE},
				{-S_SIZE, -S_SIZE, -S_SIZE},
				{S_SIZE, -S_SIZE, -S_SIZE },
				{S_SIZE, -S_SIZE, -S_SIZE },
				{S_SIZE, S_SIZE, -S_SIZE  },
				{-S_SIZE, S_SIZE, -S_SIZE },

				{-S_SIZE, -S_SIZE, S_SIZE },
				{-S_SIZE, -S_SIZE, -S_SIZE},
				{-S_SIZE, S_SIZE, -S_SIZE },
				{-S_SIZE, S_SIZE, -S_SIZE },
				{-S_SIZE, S_SIZE, S_SIZE  },
				{-S_SIZE, -S_SIZE, S_SIZE },

				{S_SIZE, -S_SIZE, -S_SIZE },
				{S_SIZE, -S_SIZE, S_SIZE  },
				{S_SIZE, S_SIZE, S_SIZE	  },
				{S_SIZE, S_SIZE, S_SIZE	  },
				{S_SIZE, S_SIZE, -S_SIZE  },
				{S_SIZE, -S_SIZE, -S_SIZE },

				{-S_SIZE, -S_SIZE, S_SIZE },
				{-S_SIZE, S_SIZE, S_SIZE  },
				{S_SIZE, S_SIZE, S_SIZE	  },
				{S_SIZE, S_SIZE, S_SIZE	  },
				{S_SIZE, -S_SIZE, S_SIZE  },
				{-S_SIZE, -S_SIZE, S_SIZE },

				{-S_SIZE, S_SIZE, -S_SIZE },
				{S_SIZE, S_SIZE, -S_SIZE  },
				{S_SIZE, S_SIZE, S_SIZE	  },
				{S_SIZE, S_SIZE, S_SIZE	  },
				{-S_SIZE, S_SIZE, S_SIZE  },
				{-S_SIZE, S_SIZE, -S_SIZE },

				{-S_SIZE, -S_SIZE, -S_SIZE},
				{-S_SIZE, -S_SIZE, S_SIZE },
				{S_SIZE, -S_SIZE, -S_SIZE },
				{S_SIZE, -S_SIZE, -S_SIZE },
				{-S_SIZE, -S_SIZE, S_SIZE },
				{S_SIZE, -S_SIZE, S_SIZE  }
			};

			ARRAY<UI32> indexBufferObject;
			/*
			std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

			for (I32 itr = 0; itr < 6; itr++) {
				for (I32 x = itr * 6; x < ((itr * 6) + 6); x++) {
					Vertex v;
					v.Position = { locations[x][0], locations[x][1], locations[x][2] };
					v.Color = { 1.0f, 1.0f, 1.0f };
					v.TexCoordinates = { 0.0f, 0.0f };
					v.Normals = { 0.0f, 0.0f, 0.0f };
					v.Integrity = 1.0f;

					if (uniqueVertices.count(v) == 0) {
						uniqueVertices[v] = static_cast<uint32_t>(locations.size());
						_localVertexBufferObject.push_back(v);
					}

					indexBufferObject.push_back(uniqueVertices[v]);
				}
			}
			*/
			return { _localVertexBufferObject, indexBufferObject };
		}

		ADGRVulkanRenderableObjectInitInfo vulkanRenderer::RenderableObjectInitInfo()
		{
			ADGRVulkanRenderableObjectInitInfo _info;
			_info.logicalDevice = myVulkanCore.logicalDevice;
			_info.physicalDevice = myVulkanCore.physicalDevice;
			_info.graphicsQueue = myVulkanCore.graphicsQueue;
			_info.presentQueue = myVulkanCore.presentQueue;
			_info.commandPool = myCommandBuffer.pool;
			return _info;
		}

		void vulkanRenderer::initializeSwapChain()
		{
			mySwapChain3D.basicInitialize(
				myVulkanCore.logicalDevice,
				myVulkanCore.physicalDevice,
				myVulkanCore.surface,
				myVulkanCore.surfaceCapabilities);

			mySwapChain3D.initializeSwapChain(myWindowManager.windowWidth, myWindowManager.windowHeight);

			mySwapChain3D.initializeRenderPass(myVulkanCore.msaaSamples);
		}

		void vulkanRenderer::initializeColorBuffer()
		{
			myColorBuffer.initialize(
				myVulkanCore.logicalDevice,
				myVulkanCore.physicalDevice,
				myCommandBuffer.pool,
				myVulkanCore.graphicsQueue,
				myVulkanCore.presentQueue,
				mySwapChain3D.swapChainContainer.getSwapChainImageFormat(),
				mySwapChain3D.swapChainContainer.getSwapChainExtent(),
				myVulkanCore.msaaSamples);
		}

		void vulkanRenderer::initializeDepthBuffer()
		{
			myDepthBuffer.initialize(
				myVulkanCore.logicalDevice,
				myVulkanCore.physicalDevice,
				myCommandBuffer.pool,
				myVulkanCore.graphicsQueue,
				myVulkanCore.presentQueue,
				mySwapChain3D.swapChainContainer.getSwapChainExtent(),
				myVulkanCore.msaaSamples);
		}

		ADGRVulkanRenderData vulkanRenderer::initializeSkyboxObject(ADGRVulkan3DObjectData _object)
		{
			VulkanSkyBox _renderObject(RenderableObjectInitInfo());

			_renderObject.setSwapChainContainer(&mySwapChain3D.swapChainContainer);

			ARRAY<VulkanShader> _shaders;

			if (_object.vertexShaderPath.size() && _object.vertexShaderPath != "NONE")
			{
				ADGRVulkanShaderInitInfo _initInfo;
				_initInfo.path = _object.vertexShaderPath;
				_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

				VulkanShader _shader;
				_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
				_shaders.pushBack(_shader);
			}
			if (_object.tessellationShaderPath.size() && _object.tessellationShaderPath != "NONE")
			{
				ADGRVulkanShaderInitInfo _initInfo;
				_initInfo.path = _object.tessellationShaderPath;
				_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

				VulkanShader _shader;
				_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
				_shaders.pushBack(_shader);
			}
			if (_object.geometryShaderPath.size() && _object.geometryShaderPath != "NONE")
			{
				ADGRVulkanShaderInitInfo _initInfo;
				_initInfo.path = _object.geometryShaderPath;
				_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

				VulkanShader _shader;
				_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
				_shaders.pushBack(_shader);
			}
			if (_object.fragmentShaderPath.size() && _object.fragmentShaderPath != "NONE")
			{
				ADGRVulkanShaderInitInfo _initInfo;
				_initInfo.path = _object.fragmentShaderPath;
				_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

				VulkanShader _shader;
				_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
				_shaders.pushBack(_shader);
			}

			// initialize pipeline
			ADGRVulkanPipelineInitInfo pipelineInitInfo;
			pipelineInitInfo.shaders = _shaders;
			pipelineInitInfo.multisamplerMsaaSamples = myVulkanCore.msaaSamples;
			pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
			pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
			pipelineInitInfo.isTexturesAvailable = _object.texturePaths.size();
			pipelineInitInfo.rasterizerFrontFace = VK_FRONT_FACE_CLOCKWISE;
			_renderObject.initializePipeline(pipelineInitInfo);

			for (VulkanShader _shader : _shaders)
				_shader.terminate(myVulkanCore.logicalDevice);

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

			//auto buffer = _skyBoxTest();

			// initialize vertex buffers
			for (UI32 _itr = 0; _itr < _object.vertexBufferObjects->size(); _itr++)
				//_renderObject.initializeVertexBuffer(&buffer);
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

			return _renderObject.getRenderData();
		}

		ADGRVulkanRenderData vulkanRenderer::initializeReflectObject(ADGRVulkan3DObjectData _object)
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
				_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
				_shaders.pushBack(_shader);
			}
			if (_object.tessellationShaderPath.size() && _object.tessellationShaderPath != "NONE")
			{
				ADGRVulkanShaderInitInfo _initInfo;
				_initInfo.path = _object.tessellationShaderPath;
				_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

				VulkanShader _shader;
				_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
				_shaders.pushBack(_shader);
			}
			if (_object.geometryShaderPath.size() && _object.geometryShaderPath != "NONE")
			{
				ADGRVulkanShaderInitInfo _initInfo;
				_initInfo.path = _object.geometryShaderPath;
				_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

				VulkanShader _shader;
				_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
				_shaders.pushBack(_shader);
			}
			if (_object.fragmentShaderPath.size() && _object.fragmentShaderPath != "NONE")
			{
				ADGRVulkanShaderInitInfo _initInfo;
				_initInfo.path = _object.fragmentShaderPath;
				_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

				VulkanShader _shader;
				_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
				_shaders.pushBack(_shader);
			}

			// initialize pipeline
			ADGRVulkanPipelineInitInfo pipelineInitInfo;
			pipelineInitInfo.shaders = _shaders;
			pipelineInitInfo.multisamplerMsaaSamples = myVulkanCore.msaaSamples;
			pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
			pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
			pipelineInitInfo.isTexturesAvailable = _object.texturePaths.size();
			_renderObject.initializePipeline(pipelineInitInfo);

			for (VulkanShader _shader : _shaders)
				_shader.terminate(myVulkanCore.logicalDevice);

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

			return _renderObject.getRenderData();
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
				else if (_object.type == DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT)
				{
					VulkanReflectObject _renderObject(RenderableObjectInitInfo());
					_renderObject.setRenderData(_object);

					DMKUpdateInfo info;
					info.aspectRatio = (F32)myWindowManager.windowWidth / (F32)myWindowManager.windowHeight;
					_renderObject.updateUniformBuffer(myCameraReflect.updateCamera(eventContainer, info), imageIndex);
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

			if (overlayContainer.isVisible && overlayContainer.isInitialized)
				result = myVulkanCore.submitQueues({ swapChain }, imageIndex, currentFrame, { myCommandBuffer.buffers[imageIndex], _drawOverlay(imageIndex) });
			else
				result = myVulkanCore.submitQueues({ swapChain }, imageIndex, currentFrame, { myCommandBuffer.buffers[imageIndex] });

			// frame buffer resize event
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				recreateSwapChain();
			}
			else if (result != VK_SUCCESS)
				DMK_CORE_FATAL("failed to present swap chain image!");
		}

		void vulkanRenderer::_initializeOverlayCommandPool()
		{
			ADGRVulkanCommandBufferInitResources initRes;
			initRes.logicalDevice = myVulkanCore.logicalDevice;
			initRes.physicalDevice = myVulkanCore.physicalDevice;
			initRes.surface = myVulkanCore.surface;
			overlayCommandBuffer.initializeResources(initRes);
			overlayCommandBuffer.initializeCommandPool();
		}

		void vulkanRenderer::_initializeOverlayDescriptorSetLayout()
		{
			VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
			samplerLayoutBinding.binding = 0; // info.bindIndex;
			samplerLayoutBinding.descriptorCount = 1;
			samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
			samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = 1;
			layoutInfo.pBindings = &samplerLayoutBinding;

			if (vkCreateDescriptorSetLayout(myVulkanCore.logicalDevice, &layoutInfo, nullptr, &overlayContainer.descriptorSetLayout) != VK_SUCCESS)
				DMK_CORE_FATAL("failed to create descriptor set layout!");
		}

		void vulkanRenderer::_initializeOverlayPipelineLayout()
		{
			// initialize the pipeline layout
			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 1;
			pipelineLayoutInfo.pSetLayouts = &overlayContainer.descriptorSetLayout;
			pipelineLayoutInfo.pushConstantRangeCount = 0;	// make support for multiple
			pipelineLayoutInfo.pPushConstantRanges = nullptr;

			// create the pipeline layout
			VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
			if (vkCreatePipelineLayout(myVulkanCore.logicalDevice, &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
				DMK_CORE_FATAL("failed to create pipeline layout!");

			overlayContainer.pipelineLayout = _pipelineLayout;
		}

		void vulkanRenderer::_initializeOverlayRenderPass()
		{
			VkAttachmentDescription attachments[2] = {};

			// Color attachment
			attachments[0].format = mySwapChain3D.swapChainContainer.getSwapChainImageFormat();
			attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
			// Don't clear the framebuffer (like the renderpass from the example does)
			attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
			attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachments[0].initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			// Depth attachment
			attachments[1].format = VulkanFunctions::findDepthFormat(myVulkanCore.physicalDevice);
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

			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 2;
			renderPassInfo.pAttachments = attachments;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = subPasses.data();
			renderPassInfo.dependencyCount = 2;
			renderPassInfo.pDependencies = subpassDependencies;

			// create the render pass
			if (vkCreateRenderPass(myVulkanCore.logicalDevice, &renderPassInfo, nullptr, &overlayContainer.renderPass) != VK_SUCCESS)
				DMK_CORE_FATAL("failed to create render pass!");
		}

		void vulkanRenderer::_initializeOverlayCommandBuffers(ADGRVulkanCommandBufferInitInfo info)
		{
			UI32 size = info.count;
			overlayCommandBuffer.buffers.resize(size);

			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = overlayCommandBuffer.pool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = size;

			if (vkAllocateCommandBuffers(myVulkanCore.logicalDevice, &allocInfo, overlayCommandBuffer.buffers.data()) != VK_SUCCESS)
				DMK_CORE_FATAL("failed to allocate command buffers!");

			for (size_t i = 0; i < size; i++) {
				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

				if (vkBeginCommandBuffer(overlayCommandBuffer.buffers[i], &beginInfo) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to begin recording command commandBuffers[i]!");

				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = info.swapChain.getRenderPass();
				renderPassInfo.framebuffer = info.swapChain.getFrameBuffer(i);
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = info.swapChain.getSwapChainExtent();

				std::array<VkClearValue, 2> clearValues = {};

				clearValues[0].color = {
					0.0f, 0.0f, 0.0f, 0.0f
				};

				renderPassInfo.clearValueCount = static_cast<UI32>(clearValues.size());
				renderPassInfo.pClearValues = clearValues.data();

				/* BEGIN VULKAN COMMANDS */
				VkDeviceSize offsets[] = { 0 };
				// begin render pass
				vkCmdBeginRenderPass(overlayCommandBuffer.buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				VkViewport viewport{};
				viewport.width = info.swapChain.getSwapChainExtent().width;
				viewport.height = info.swapChain.getSwapChainExtent().height;
				viewport.minDepth = 0;
				viewport.maxDepth = 0;
				vkCmdSetViewport(overlayCommandBuffer.buffers[i], 0, 1, &viewport);

				VkRect2D rect2D{};
				rect2D.extent.width = info.swapChain.getSwapChainExtent().width;
				rect2D.extent.height = info.swapChain.getSwapChainExtent().height;
				rect2D.offset.x = 0.0f;
				rect2D.offset.y = 1.0f;
				vkCmdSetScissor(overlayCommandBuffer.buffers[i], 0, 1, &rect2D);

				vkCmdBindPipeline(overlayCommandBuffer.buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, overlayContainer.pipeline);

				if (overlayContainer.descriptorSet != VK_NULL_HANDLE)
					vkCmdBindDescriptorSets(overlayCommandBuffer.buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, overlayContainer.pipelineLayout, 0, 1, &overlayContainer.descriptorSet, 0, NULL);

				vkCmdBindVertexBuffers(overlayCommandBuffer.buffers[i], 0, 1, &overlayContainer.vertexBuffer, offsets);
				vkCmdBindVertexBuffers(overlayCommandBuffer.buffers[i], 1, 1, &overlayContainer.vertexBuffer, offsets);

				for (UI32 j = 0; j < overlayContainer.numberOfLetters; j++)
					vkCmdDraw(overlayCommandBuffer.buffers[i], 4, 1, j * 4, 0);

				// end renderPass
				vkCmdEndRenderPass(overlayCommandBuffer.buffers[i]);

				/* END VULKAN COMMANDS */
				if (vkEndCommandBuffer(overlayCommandBuffer.buffers[i]) != VK_SUCCESS)
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

		void vulkanRenderer::_initializeOverlayStageOne()
		{
			_initializeOverlayCommandPool();
			_initializeOverlayDescriptorSetLayout();
			_initializeOverlayPipelineLayout();
			//_initializeOverlayRenderPass();
			overlayContainer.isInitialized = false;
		}

		void vulkanRenderer::_initializeOverlayStageTwo(ADGRVulkanShaderPathContainer shaderContainer)
		{
			_initializeOverlayVertexBuffer();
			_initializeOverlayTextureImage();
			_initializeOverlayPipeline(shaderContainer);
			_initializeOverlayDescriptorPool();
			_initializeOverlayDescriptorSets();
			overlayContainer.isInitialized = true;
		}

		void vulkanRenderer::_terminateOverlay()
		{
		}

		void vulkanRenderer::_initializeOverlayVertexBuffer()
		{
			VkDeviceSize bufferSize = TEXTOVERLAY_MAX_CHAR_COUNT * sizeof(glm::vec4);

			ADGRCreateBufferInfo vertBufferInfo;
			vertBufferInfo.bufferSize = bufferSize;
			vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			vertBufferInfo.buffer = &overlayContainer.vertexBuffer;
			vertBufferInfo.bufferMemory = &overlayContainer.vertexBufferMemory;

			VulkanFunctions::createBuffer(myVulkanCore.logicalDevice, myVulkanCore.physicalDevice, vertBufferInfo);
		}

		void vulkanRenderer::_initializeOverlayTextureImage()
		{
			VkBuffer stagingBuffer = VK_NULL_HANDLE;
			VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

			VkDeviceSize bufferSize = TEXTOVERLAY_MAX_CHAR_COUNT * sizeof(glm::vec4);
			UI32 width = mySwapChain3D.swapChainContainer.getSwapChainExtent().width;
			UI32 height = mySwapChain3D.swapChainContainer.getSwapChainExtent().height;
			UI32 size = width * height;

			const uint32_t fontWidth = STB_FONT_consolas_24_latin1_BITMAP_WIDTH;
			const uint32_t fontHeight = STB_FONT_consolas_24_latin1_BITMAP_WIDTH;
			static unsigned char font24pixels[fontWidth][fontHeight];
			stb_font_consolas_24_latin1(overlayContainer.stbFontData, font24pixels, fontHeight);

			ADGRCreateImageInfo cinfo;
			cinfo.width = fontWidth;
			cinfo.height = fontHeight;
			cinfo.format = VK_FORMAT_R8_UNORM;
			cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			cinfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			cinfo.image = &overlayContainer.textureContainer.image;
			cinfo.imageMemory = &overlayContainer.textureContainer.imageMemory;
			cinfo.mipLevels = 1;
			cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
			cinfo.flags = NULL;
			VulkanFunctions::createImage(myVulkanCore.logicalDevice, myVulkanCore.physicalDevice, cinfo);

			ADGRCreateBufferInfo bufferInfo;
			bufferInfo.bufferSize = fontWidth * fontHeight;
			bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			bufferInfo.buffer = &stagingBuffer;
			bufferInfo.bufferMemory = &stagingBufferMemory;
			VulkanFunctions::createBuffer(myVulkanCore.logicalDevice, myVulkanCore.physicalDevice, bufferInfo);

			void* data;
			if (vkMapMemory(myVulkanCore.logicalDevice, stagingBufferMemory, 0, static_cast<size_t>(bufferSize), 0, &data) != VK_SUCCESS)
				DMK_CORE_FATAL("Failed to map memory!");
			memcpy(data, &font24pixels[0][0], fontWidth * fontHeight);
			vkUnmapMemory(myVulkanCore.logicalDevice, stagingBufferMemory);

			ADGRTransitionImageLayoutInfo transitionInfo;
			transitionInfo.image = overlayContainer.textureContainer.image;
			transitionInfo.format = VK_FORMAT_R8_UNORM;
			transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			transitionInfo.mipLevels = 1;
			transitionInfo.layerCount = 1;
			VulkanFunctions::transitionImageLayout(myVulkanCore.logicalDevice, overlayCommandBuffer.pool, myVulkanCore.graphicsQueue, myVulkanCore.presentQueue, transitionInfo);

			ADGRCopyBufferToImageInfo cpyInfo;
			cpyInfo.buffer = stagingBuffer;
			cpyInfo.image = overlayContainer.textureContainer.image;
			cpyInfo.width = fontWidth;
			cpyInfo.height = fontHeight;
			VulkanFunctions::copyBufferToImage(myVulkanCore.logicalDevice, overlayCommandBuffer.pool, myVulkanCore.graphicsQueue, myVulkanCore.presentQueue, cpyInfo);

			transitionInfo.image = overlayContainer.textureContainer.image;
			transitionInfo.format = VK_FORMAT_R8_UNORM;
			transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			transitionInfo.mipLevels = 1;
			transitionInfo.layerCount = 1;
			VulkanFunctions::transitionImageLayout(myVulkanCore.logicalDevice, overlayCommandBuffer.pool, myVulkanCore.graphicsQueue, myVulkanCore.presentQueue, transitionInfo);

			vkDestroyBuffer(myVulkanCore.logicalDevice, stagingBuffer, nullptr);
			vkFreeMemory(myVulkanCore.logicalDevice, stagingBufferMemory, nullptr);

			ADGRVulkanTextureSamplerInitInfo samplerInitInfo;
			samplerInitInfo.magFilter = VK_FILTER_LINEAR;
			samplerInitInfo.minFilter = VK_FILTER_LINEAR;
			samplerInitInfo.maxMipLevels = 1;
			samplerInitInfo.minMipLevels = 0;
			samplerInitInfo.mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInitInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInitInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInitInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInitInfo.mipLoadBias = 0.0f;
			samplerInitInfo.compareOp = VK_COMPARE_OP_NEVER;
			samplerInitInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
			overlayContainer.textureContainer.imageSampler = VulkanFunctions::createImageSampler(myVulkanCore.logicalDevice, samplerInitInfo);

			ADGRCreateImageViewInfo cinfo2;
			cinfo2.image = overlayContainer.textureContainer.image;
			cinfo2.format = VK_FORMAT_R8_UNORM;
			cinfo2.mipLevels = 1;
			cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			cinfo2.component = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B,	VK_COMPONENT_SWIZZLE_A };;
			overlayContainer.textureContainer.imageView = VulkanFunctions::createImageView(myVulkanCore.logicalDevice, cinfo2);
		}

		void vulkanRenderer::_initializeOverlayPipeline(ADGRVulkanShaderPathContainer shaderContainer)
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

			if (shaderContainer.vertexShaderPath.size() && shaderContainer.vertexShaderPath != "NONE")
			{
				ADGRVulkanShaderInitInfo _initInfo;
				_initInfo.path = shaderContainer.vertexShaderPath;
				_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

				VulkanShader _shader;
				_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
				_shaders.pushBack(_shader);
			}
			if (shaderContainer.tessellationShaderPath.size() && shaderContainer.tessellationShaderPath != "NONE")
			{
				ADGRVulkanShaderInitInfo _initInfo;
				_initInfo.path = shaderContainer.tessellationShaderPath;
				_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

				VulkanShader _shader;
				_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
				_shaders.pushBack(_shader);
			}
			if (shaderContainer.geometryShaderPath.size() && shaderContainer.geometryShaderPath != "NONE")
			{
				ADGRVulkanShaderInitInfo _initInfo;
				_initInfo.path = shaderContainer.geometryShaderPath;
				_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

				VulkanShader _shader;
				_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
				_shaders.pushBack(_shader);
			}
			if (shaderContainer.fragmentShaderPath.size() && shaderContainer.fragmentShaderPath != "NONE")
			{
				ADGRVulkanShaderInitInfo _initInfo;
				_initInfo.path = shaderContainer.fragmentShaderPath;
				_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

				VulkanShader _shader;
				_shader.initialize(myVulkanCore.logicalDevice, _initInfo);
				_shaders.pushBack(_shader);
			}

			VulkanRenderableObject _object;
			_object.setSwapChainContainer(&mySwapChain3D.swapChainContainer);
			ADGRVulkanRenderableObjectInitInfo objInitInfo;
			objInitInfo.logicalDevice = myVulkanCore.logicalDevice;
			objInitInfo.physicalDevice = myVulkanCore.physicalDevice;
			objInitInfo.commandPool = overlayCommandBuffer.pool;
			objInitInfo.graphicsQueue = myVulkanCore.graphicsQueue;
			objInitInfo.presentQueue = myVulkanCore.presentQueue;
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
			initInfo.multisamplerMsaaSamples = myVulkanCore.msaaSamples;
			initInfo.dynamicStateEnable = true;
			initInfo.dynamicStateFlags = 0;
			initInfo.vertexBindingDescription = VertexP2N2::getBindingDescription(1);
			initInfo.vertexAttributeDescription = VertexP2N2::getAttributeDescriptions();
			initInfo.shaders = _shaders;
			initInfo.renderPass = mySwapChain3D.swapChainContainer.getRenderPass();
			initInfo.pipelineLayout = overlayContainer.pipelineLayout;
			_object.initializePipeline(initInfo);

			for (VulkanShader _shader : _shaders)
				_shader.terminate(myVulkanCore.logicalDevice);

			overlayContainer.pipeline = _object.getRenderData().pipeline;
		}

		void vulkanRenderer::_initializeOverlayDescriptorPool()
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
			if (vkCreateDescriptorPool(myVulkanCore.logicalDevice, &poolInfo, nullptr, &_localDescriptorPool) != VK_SUCCESS)
				DMK_CORE_FATAL("failed to create descriptor pool!");

			overlayContainer.descriptorPool = _localDescriptorPool;
		}

		void vulkanRenderer::_initializeOverlayDescriptorSets()
		{
			VkDescriptorSetLayout _layout = overlayContainer.descriptorSetLayout;

			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = overlayContainer.descriptorPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &_layout;

			VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
			if (vkAllocateDescriptorSets(myVulkanCore.logicalDevice, &allocInfo, &_descriptorSet) != VK_SUCCESS)
				DMK_CORE_FATAL("failed to allocate descriptor sets!");

			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = overlayContainer.textureContainer.imageView;
			imageInfo.sampler = overlayContainer.textureContainer.imageSampler;

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

			vkUpdateDescriptorSets(myVulkanCore.logicalDevice, 1, &_writes, 0, nullptr);

			overlayContainer.descriptorSet = _descriptorSet;
		}

		VkCommandBuffer vulkanRenderer::_drawOverlay(UI32 imageIndex)
		{
			return overlayCommandBuffer.buffers[imageIndex];
		}

		void vulkanRenderer::_beginUpdate()
		{
			overlayContainer.isInitialized = false;
			if (vkMapMemory(myVulkanCore.logicalDevice, overlayContainer.textureContainer.imageMemory, 0, VK_WHOLE_SIZE, 0, (void**)&overlayContainer.mapped) != VK_SUCCESS)
				DMK_CORE_FATAL("failed to map memory!");
			overlayContainer.numberOfLetters = 0;
		}

		void vulkanRenderer::_endUpdate()
		{
			vkUnmapMemory(myVulkanCore.logicalDevice, overlayContainer.textureContainer.imageMemory);
			overlayContainer.mapped = nullptr;

			ADGRVulkanCommandBufferInitInfo initInfo;
			initInfo.count = mySwapChain3D.swapChainContainer.getSwapChainImages().size();
			initInfo.swapChain = mySwapChain3D.swapChainContainer;
			_initializeOverlayCommandBuffers(initInfo);
			overlayContainer.isInitialized = true;
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

		void vulkanRenderer::addText(std::string text, F32 x, F32 y, DMKTextAlign align)
		{
			_beginUpdate();
			const UI32 firstChar = STB_FONT_consolas_24_latin1_FIRST_CHAR;

			assert(overlayContainer.mapped != nullptr);

			UI32 width = mySwapChain3D.swapChainContainer.getSwapChainExtent().width;
			UI32 height = mySwapChain3D.swapChainContainer.getSwapChainExtent().height;

			const float charW = 1.5f / width;
			const float charH = 1.5f / height;

			float fbW = width;
			float fbH = height;
			x = (x / fbW * 2.0f) - 1.0f;
			y = (y / fbH * 2.0f) - 1.0f;

			// Calculate text width
			float textWidth = 0;
			for (auto letter : text)
			{
				stb_fontchar* charData = &overlayContainer.stbFontData[(UI32)letter - firstChar];
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
				stb_fontchar* charData = &overlayContainer.stbFontData[(UI32)letter - firstChar];

				overlayContainer.mapped->x = (x + (float)charData->x0 * charW);
				overlayContainer.mapped->y = (y + (float)charData->y0 * charH);
				overlayContainer.mapped->z = charData->s0;
				overlayContainer.mapped->w = charData->t0;
				overlayContainer.mapped++;

				overlayContainer.mapped->x = (x + (float)charData->x1 * charW);
				overlayContainer.mapped->y = (y + (float)charData->y0 * charH);
				overlayContainer.mapped->z = charData->s1;
				overlayContainer.mapped->w = charData->t0;
				overlayContainer.mapped++;

				overlayContainer.mapped->x = (x + (float)charData->x0 * charW);
				overlayContainer.mapped->y = (y + (float)charData->y1 * charH);
				overlayContainer.mapped->z = charData->s0;
				overlayContainer.mapped->w = charData->t1;
				overlayContainer.mapped++;

				overlayContainer.mapped->x = (x + (float)charData->x1 * charW);
				overlayContainer.mapped->y = (y + (float)charData->y1 * charH);
				overlayContainer.mapped->z = charData->s1;
				overlayContainer.mapped->w = charData->t1;
				overlayContainer.mapped++;

				x += charData->advance * charW;

				overlayContainer.numberOfLetters++;
			}
			_endUpdate();
		}
	}
}