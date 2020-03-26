#include "adgrafx.h"
#include "VulkanRBL3D.h"

namespace Dynamik {
	namespace ADGR {
		void VulkanRBL3D::init()
		{
			initStageOne();
			initStageTwo();
			initStageThree();
		}

		void VulkanRBL3D::initStageOne()
		{
			// initialize the window
			DMKWindowManagerInitInfo windowInitInfo;
			windowInitInfo.iconPaths = "E:/Projects/Dynamik Engine/Versions/Dynamik (Prototype)/Dependencies/Assets/icons/Dynamik.jpg";
			myWindowManager.initialize(windowInitInfo);

			// initialize the instance
			ADGRVulkanInstanceInitInfo instanceInitInfo;
			instanceInitInfo.applicationName = "Dynamik Engine";
			instanceInitInfo.engineName = "Dynamik";
			myInstance.initialize(instanceInitInfo);

			// initialize debugger
			myDebugger.initialize(myInstance);

			// initialize window surface
			myWindowManager.createWindowSurface(&myInstance);

			// initialize devices
			myDevice.initialize(myInstance, &myQueue, &myGlobals);

			// initialize swap chain
			mySwapChain.initialize(myInstance, myDevice, myWindowManager.windowWidth, myWindowManager.windowHeight);	// TODO::

			// initialize command pool
			myCommandBuffer.initializeCommandPool(myInstance, myDevice);

			// initialize color buffer
			ADGRVulkanColorBufferInitInfo colorBufferInitInfo;
			colorBufferInitInfo.commandBuffer = myCommandBuffer;
			colorBufferInitInfo.device = myDevice;
			colorBufferInitInfo.global = &myGlobals;
			colorBufferInitInfo.queue = myQueue;
			colorBufferInitInfo.swapChain = mySwapChain;
			myColorBuffer.initialize(colorBufferInitInfo);

			// initialize depth buffer
			ADGRVulkanDepthBufferInitInfo depthBufferInitInfo;
			depthBufferInitInfo.commandBuffer = myCommandBuffer;
			depthBufferInitInfo.device = myDevice;
			depthBufferInitInfo.global = &myGlobals;
			depthBufferInitInfo.queue = myQueue;
			depthBufferInitInfo.swapChain = mySwapChain;
			myDepthBuffer.initialize(depthBufferInitInfo);

			// initiailze render pass
			ADGRVulkanRenderPassInitInfo renderPassInitInfo;
			renderPassInitInfo.device = myDevice;
			renderPassInitInfo.global = myGlobals;
			renderPassInitInfo.enableDepthAttachment = true;
			renderPassInitInfo.swapChain = mySwapChain;
			myRenderPass.initialize(renderPassInitInfo);

			// initialize frame buffer
			ADGRVulkanFrameBufferInitInfo frameBufferInitInfo;
			frameBufferInitInfo.preAttachments = { myColorBuffer.imageView, myDepthBuffer.imageView };
			frameBufferInitInfo.device = myDevice;
			frameBufferInitInfo.renderPass = myRenderPass;
			frameBufferInitInfo.swapChain = mySwapChain;
			myFrameBuffer.initialize(frameBufferInitInfo);
		}

		void VulkanRBL3D::initStageTwo()
		{
			// initialize objects
			initializeObjects();

			// initialize command buffers
			ADGRVulkanCommandBufferInitInfo commandBufferInitInfo;
			commandBufferInitInfo.device = myDevice;
			commandBufferInitInfo.frameBuffer = myFrameBuffer;
			commandBufferInitInfo.renderPass = myRenderPass;
			commandBufferInitInfo.swapChain = mySwapChain;
			commandBufferInitInfo.objects = &renderableObjects;
			myCommandBuffer.initialize(commandBufferInitInfo);
		}

		void VulkanRBL3D::initStageThree()
		{
			mySyncObjects.initialize(myDevice);
		}

		void VulkanRBL3D::drawFrame()
		{
		}

		void VulkanRBL3D::shutDown()
		{
		}

		void VulkanRBL3D::shutDownStageOne()
		{
		}

		void VulkanRBL3D::shutDownStageTwo()
		{
		}

		void VulkanRBL3D::shutDownStageThree()
		{
		}

		void VulkanRBL3D::getObjects(ARRAY<ADGRVulkan3DObjectData> objectDatas)
		{
			rawObjectStore = objectDatas;
		}

		void VulkanRBL3D::update()
		{
			initializeObjects();
		}

		void VulkanRBL3D::initializeObjects()
		{
			for (ADGRVulkan3DObjectData _object : rawObjectStore)
			{
				VulkanRenderObject _renderObject;

				// set the render pass
				_renderObject.renderPass = myRenderPass;

				// initialize descriptor set layout
				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
				layoutInitInfo.device = myDevice;
				_renderObject.descriptorSet.initializeLayout(layoutInitInfo);

				// initialize shaders
				ARRAY<VulkanShader> shaders;
				if (_object.vertexShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _shaderInitInfo;
					_shaderInitInfo.device = myDevice;
					_shaderInitInfo.path = _object.vertexShaderPath;
					_shaderInitInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

					VulkanShader _shader;
					_shader.initialize(_shaderInitInfo);

					shaders.pushBack(_shader);
				}
				if (_object.tessellationShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _shaderInitInfo;
					_shaderInitInfo.device = myDevice;
					_shaderInitInfo.path = _object.tessellationShaderPath;
					_shaderInitInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

					VulkanShader _shader;
					_shader.initialize(_shaderInitInfo);

					shaders.pushBack(_shader);
				}
				if (_object.geometryShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _shaderInitInfo;
					_shaderInitInfo.device = myDevice;
					_shaderInitInfo.path = _object.geometryShaderPath;
					_shaderInitInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

					VulkanShader _shader;
					_shader.initialize(_shaderInitInfo);

					shaders.pushBack(_shader);
				}
				if (_object.fragmentShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _shaderInitInfo;
					_shaderInitInfo.device = myDevice;
					_shaderInitInfo.path = _object.fragmentShaderPath;
					_shaderInitInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

					VulkanShader _shader;
					_shader.initialize(_shaderInitInfo);

					shaders.pushBack(_shader);
				}

				// initialize pipeline
				ADGRVulkanPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.device = myDevice;
				pipelineInitInfo.renderPass = myRenderPass;
				pipelineInitInfo.swapChain = mySwapChain;
				pipelineInitInfo.global = &myGlobals;
				pipelineInitInfo.layouts = { &_renderObject.descriptorSet };
				pipelineInitInfo.pushConstantsEnable = true;
				pipelineInitInfo.pushConstantCount = 1;
				pipelineInitInfo.shaders = shaders;
				_renderObject.pipeline.initialize(pipelineInitInfo);

				// terminate shaders
				for (VulkanShader _shader : shaders)
					_shader.terminate(myDevice);

				// initialize textures
				for (UI32 _itr = 0; _itr < _object.texturePaths.size(); _itr++)
				{
					VulkanTexture _texture;

					ADGRVulkanTextureInitInfo initInfo;
					initInfo.path = _object.texturePaths[_itr];
					initInfo.device = myDevice;
					initInfo.commandPool = myCommandBuffer.commandPool;
					initInfo.global = myGlobals;
					initInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
					initInfo.mipLevels = 1;
					_texture.initialize(initInfo);

					_renderObject.textures.pushBack(_texture);
				}

				// initialize vertex buffers
				for (UI32 _itr = 0; _itr < _object.vertexBufferObjects->size(); _itr++)
				{
					VulkanVertexBuffer _vertexBuffer;
					_vertexBuffer.initialize(myDevice, myCommandBuffer, myQueue, &_object.vertexBufferObjects->at(_itr));

					_renderObject.vertexBuffers.pushBack(_vertexBuffer);
				}

				// initialize index buffers
				for (UI32 _itr = 0; _itr < _object.indexBufferObjects->size(); _itr++)
				{
					VulkanIndexBuffer _indexBuffer;
					_indexBuffer.initialize(myDevice, myCommandBuffer, myQueue, &_object.indexBufferObjects->at(_itr));

					_renderObject.indexBuffers.pushBack(_indexBuffer);
				}

				// initialize uniform buffers
				_renderObject.uniformBuffer.initialize(myDevice, mySwapChain);

				// initialize descriptor pool
				ADGRVulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
				descriptorPoolInitInfo.device = myDevice;
				descriptorPoolInitInfo.swapChain = mySwapChain;
				descriptorPoolInitInfo.poolCount = _renderObject.textures.size();
				_renderObject.descriptorSet.initializeDescriptorPool(descriptorPoolInitInfo);

				// initialize descriptor sets
				ADGRVulkanDescriptorSetsInitInfo descriptorSetsInitInfo;
				descriptorSetsInitInfo.device = myDevice;
				descriptorSetsInitInfo.swapChain = mySwapChain;
				descriptorSetsInitInfo.textures = _renderObject.textures;
				descriptorSetsInitInfo.uniformBuffers = _renderObject.uniformBuffer;
				_renderObject.descriptorSet.initializeDescriptorSets(descriptorSetsInitInfo);

				renderableObjects.pushBack(_renderObject);
			}
		}
	}
}