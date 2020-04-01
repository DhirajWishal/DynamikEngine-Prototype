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
			myInstance.initialize(&myCoreObject, instanceInitInfo);

			// initialize debugger
			myDebugger.initialize(&myCoreObject);

			// initialize window surface
			myWindowManager.createWindowSurface(&myCoreObject);

			// initialize devices
			myDevice.initialize(&myCoreObject);

			// initialize swap chain
			mySwapChain.initialize(&myCoreObject, myWindowManager.windowWidth, myWindowManager.windowHeight);	// TODO::

			// initialize command pool
			myCommandBuffer.initializeCommandPool(&myCoreObject);

			// initialize color buffer
			auto attachment1 = myColorBuffer.initialize(&myCoreObject);

			// initialize depth buffer
			auto attachment2 = myDepthBuffer.initialize(&myCoreObject);

			// initialize render pass
			ADGRVulkanRenderPassInitInfo renderPassInitInfo;
			renderPassInitInfo.enableDepthAttachment = true;
			myRenderPass.initialize(&myCoreObject, renderPassInitInfo);

			// initialize frame buffer
			ADGRVulkanFrameBufferInitInfo frameBufferInitInfo;
			frameBufferInitInfo.preAttachments = { attachment1.imageView, attachment2.imageView };
			myFrameBuffer.initialize(&myCoreObject, frameBufferInitInfo);
		}

		void VulkanRBL3D::initStageTwo()
		{
			// initialize objects
			initializeObjects();

			// initialize command buffers
			ADGRVulkanCommandBufferInitInfo commandBufferInitInfo;
			commandBufferInitInfo.objects = &renderableObjects;
			myCommandBuffer.initialize(&myCoreObject, commandBufferInitInfo);
		}

		void VulkanRBL3D::initStageThree()
		{
			mySyncObjects.initialize(myCoreObject.logicalDevice);
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
				_renderObject.renderPass = &myCoreObject.renderPass;

				// initialize descriptor set layout
				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
				myDescriptorManager.initializeLayout(&myCoreObject, layoutInitInfo, &_renderObject);

				// initialize shaders
				/*
				ARRAY<VulkanShaderManager> shaders;
				if (_object.vertexShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _shaderInitInfo;
					_shaderInitInfo.device = myDevice;
					_shaderInitInfo.path = _object.vertexShaderPath;
					_shaderInitInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

					VulkanShaderManager _shader;
					_shader.initialize(_shaderInitInfo);

					shaders.push_back(_shader);
				}
				if (_object.tessellationShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _shaderInitInfo;
					_shaderInitInfo.device = myDevice;
					_shaderInitInfo.path = _object.tessellationShaderPath;
					_shaderInitInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

					VulkanShaderManager _shader;
					_shader.initialize(_shaderInitInfo);

					shaders.push_back(_shader);
				}
				if (_object.geometryShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _shaderInitInfo;
					_shaderInitInfo.device = myDevice;
					_shaderInitInfo.path = _object.geometryShaderPath;
					_shaderInitInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

					VulkanShaderManager _shader;
					_shader.initialize(_shaderInitInfo);

					shaders.push_back(_shader);
				}
				if (_object.fragmentShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _shaderInitInfo;
					_shaderInitInfo.device = myDevice;
					_shaderInitInfo.path = _object.fragmentShaderPath;
					_shaderInitInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

					VulkanShaderManager _shader;
					_shader.initialize(_shaderInitInfo);

					shaders.push_back(_shader);
				}*/

				// initialize pipeline
				ADGRVulkanPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.pushConstantsEnable = true;
				pipelineInitInfo.pushConstantCount = 1;
				pipelineInitInfo.vertex = _object.vertexShaderPath;
				pipelineInitInfo.fragment = _object.fragmentShaderPath;
				myPipelineManager.initialize(&myCoreObject, pipelineInitInfo, &_renderObject);

				// terminate shaders
				//for (VulkanShaderManager _shader : shaders)
				//	_shader.terminate(myDevice);

				// initialize textures
				for (UI32 _itr = 0; _itr < _object.texturePaths.size(); _itr++)
				{
					VulkanTextureManager _texture;

					ADGRVulkanTextureInitInfo initInfo;
					initInfo.path = _object.texturePaths[_itr];
					initInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
					initInfo.mipLevels = 1;
					_texture.initialize(&myCoreObject, initInfo, &_renderObject);
				}

				// initialize vertex buffers
				for (UI32 _itr = 0; _itr < _object.vertexBufferObjects->size(); _itr++)
					myVertexBufferManager.initialize(&myCoreObject, &_object.vertexBufferObjects->at(_itr), &_renderObject);

				// initialize index buffers
				for (UI32 _itr = 0; _itr < _object.indexBufferObjects->size(); _itr++)
					myIndexBufferManager.initialize(&myCoreObject, &_object.indexBufferObjects->at(_itr), &_renderObject);

				// initialize uniform buffers
				myUniformBufferManager.initialize(&myCoreObject, &_renderObject);

				// initialize descriptor pool
				ADGRVulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
				descriptorPoolInitInfo.poolCount = _renderObject.textures.size();
				myDescriptorManager.initializeDescriptorPool(&myCoreObject, descriptorPoolInitInfo, &_renderObject);

				// initialize descriptor sets
				ADGRVulkanDescriptorSetsInitInfo descriptorSetsInitInfo;
				myDescriptorManager.initializeDescriptorSets(&myCoreObject, descriptorSetsInitInfo, &_renderObject);

				renderableObjects.push_back(_renderObject);
			}
		}
	}
}