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
			myVulkanCore.initializeInstance(instanceInitInfo);
			myVulkanCore.initializeSurface(&myWindowManager.window);

			myVulkanCore.initializeDevice();

			myVulkanCore.initializeSwapChain(myWindowManager.windowWidth, myWindowManager.windowHeight);

			myVulkanCore.initializeCommandPool();

			// initialize render pass
			ADGRVulkanRenderPassInitInfo renderPassInitInfo;
			renderPassInitInfo.enableDepthAttachment = true;
			myVulkanCore.initializeRenderPass(renderPassInitInfo);

			myColorBuffer.initialize(
				myVulkanCore.getLogicalDevice(),
				myVulkanCore.getPhysicalDevice(),
				myVulkanCore.getCommandPool(),
				myVulkanCore.getGraphicsQueue(),
				myVulkanCore.getPresentQueue(),
				myVulkanCore.getSwapChainImageFormat(),
				myVulkanCore.getSwapChainExtent(),
				myVulkanCore.getMsaaSamples());

			myDepthBuffer.initialize(
				myVulkanCore.getLogicalDevice(),
				myVulkanCore.getPhysicalDevice(),
				myVulkanCore.getCommandPool(),
				myVulkanCore.getGraphicsQueue(),
				myVulkanCore.getPresentQueue(),
				myVulkanCore.getSwapChainExtent(),
				myVulkanCore.getMsaaSamples());

			// initialize frame buffer
			ADGRVulkanFrameBufferInitInfo frameBufferInitInfo;
			frameBufferInitInfo.preAttachments = { myColorBuffer.imageView, myDepthBuffer.imageView };
			myVulkanCore.initializeFrameBuffer(frameBufferInitInfo);
		}

		void VulkanRBL3D::initStageTwo()
		{
			// initialize objects
			initializeObjects();

			// initialize command buffers
			myVulkanCore.initializeCommandBuffers(renderableObjects);
		}

		void VulkanRBL3D::initStageThree()
		{
			myVulkanCore.initializeSyncObjects();
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
				VulkanRenderableObject _renderObject(
					myVulkanCore.getLogicalDevice(),
					myVulkanCore.getPhysicalDevice(),
					myVulkanCore.getCommandPool(),
					myVulkanCore.getGraphicsQueue(),
					myVulkanCore.getPresentQueue());

				// initialize descriptor set layout
				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
				_renderObject.initializeDescriptorSetLayout(layoutInitInfo);

				ADGRVulkanPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.pushConstantCount = 6;
				pipelineLayoutInitInfo.pushConstantOffset = 0;
				pipelineLayoutInitInfo.pushConstantsEnable = true;
				_renderObject.initializePipelineLayout(pipelineLayoutInitInfo);

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
				pipelineInitInfo.renderPass = myVulkanCore.getRenderPass();
				pipelineInitInfo.shaders = _shaders;
				_renderObject.initializePipeline(myVulkanCore.getSwapChainExtent(), pipelineInitInfo);

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
				descriptorPoolInitInfo.poolCount = _object.texturePaths.size();
				_renderObject.initializeDescriptorPool(descriptorPoolInitInfo);

				// initialize descriptor sets
				ADGRVulkanDescriptorSetsInitInfo descriptorSetsInitInfo;
				_renderObject.initializeDescriptorSets(descriptorSetsInitInfo);

				renderableObjects.push_back(_renderObject);
			}
		}
	}
}