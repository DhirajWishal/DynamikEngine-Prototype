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
			// initialize the instance
			ADGRVulkanInstanceInitInfo instanceInitInfo;
			instanceInitInfo.applicationName = "Dynamik Engine";
			instanceInitInfo.engineName = "Dynamik";
			myVulkanCore.initializeInstance(instanceInitInfo);
			myVulkanCore.initializeSurface(myWindowHandle.windowPointer);

			myVulkanCore.initializeDevice();

			myVulkanCore.initializeSwapChain(myWindowHandle.windowWidth, myWindowHandle.windowHeight);

			myVulkanCore.initializeCommandPool();

			// initialize render pass
			initializeRenderPass();

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
			ADGRVulkanCommandBufferInitInfo commandBufferInitInfo;
			commandBufferInitInfo.objects = renderableObjects;
			myVulkanCore.initializeCommandBuffers(commandBufferInitInfo);
		}

		void VulkanRBL3D::initStageThree()
		{
			myVulkanCore.initializeSyncObjects();
		}

		void VulkanRBL3D::drawFrame(std::deque<DMKEventContainer> container)
		{
			// wait for fences
			myVulkanCore.syncFence(currentFrame);

			// get image index
			imageIndex = 0;
			result = myVulkanCore.getNextImage(&imageIndex, currentFrame);

			// recreate swachain if needed
			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
				DMK_CORE_FATAL("failed to acquire swap chain image!");

			// draw call
			// uniform buffer object update
			for (VulkanRenderableObject _object : renderableObjects)
			{
				DMKUpdateInfo info;
				info.aspectRatio = (F32)myWindowHandle.windowWidth / (F32)myWindowHandle.windowHeight;
				_object.updateUniformBuffer(myCamera.updateCamera(container, info), imageIndex);
			}

			result = myVulkanCore.submitQueues(imageIndex, currentFrame);

			// frame buffer resize event
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
				recreateSwapChain();
			}
			else if (result != VK_SUCCESS)
				DMK_CORE_FATAL("failed to present swap chain image!");

			// current frame select
			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		}

		void VulkanRBL3D::recreateSwapChain()
		{
			shutDownStageOne();

			myVulkanCore.initializeCommandPool();

			for (VulkanRenderableObject& _object : renderableObjects)
			{
				_object.terminatePipeline();

				_object.terminateUniformBuffer();
			}

			myVulkanCore.initializeSwapChain(myWindowHandle.windowWidth, myWindowHandle.windowHeight);

			// initialize render pass
			initializeRenderPass();

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

			for (UI32 itr = 0; itr < rawObjectStore.size(); itr++)
			{
				ADGRVulkan3DObjectData _object = rawObjectStore[itr];
				VulkanRenderableObject _renderObject = renderableObjects[itr];

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
				pipelineInitInfo.multisamplerMsaaSamples = myVulkanCore.getMsaaSamples();
				pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
				pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
				_renderObject.initializePipeline(myVulkanCore.getSwapChainExtent(), pipelineInitInfo);

				for (VulkanShader _shader : _shaders)
					_shader.terminate(myVulkanCore.getLogicalDevice());

				// initialize uniform buffers
				_renderObject.initializeUniformBuffer(myVulkanCore.getSwapChainImages().size());

				// initialize descriptor pool
				ADGRVulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
				_renderObject.initializeDescriptorPool(descriptorPoolInitInfo);

				// initialize descriptor sets
				ADGRVulkanDescriptorSetsInitInfo descriptorSetsInitInfo;
				_renderObject.initializeDescriptorSets(descriptorSetsInitInfo);
			}

			// initialize command buffers
			ADGRVulkanCommandBufferInitInfo commandBufferInitInfo;
			commandBufferInitInfo.objects = renderableObjects;
			myVulkanCore.initializeCommandBuffers(commandBufferInitInfo);
		}

		void VulkanRBL3D::shutDown()
		{
			shutDownStageOne();
			shutDownStageTwo();
			shutDownStageThree();
		}

		void VulkanRBL3D::shutDownStageOne()
		{
			myDepthBuffer.terminate(myVulkanCore.getLogicalDevice());
			myColorBuffer.terminate(myVulkanCore.getLogicalDevice());

			myVulkanCore.terminateFrameBuffer();

			myVulkanCore.terminateCommandBuffers();

			myVulkanCore.terminateRenderPass();

			myVulkanCore.terminateSwapChain();
		}

		void VulkanRBL3D::shutDownStageTwo()
		{
			for (VulkanRenderableObject& _object : renderableObjects)
			{
				_object.terminatePipeline();

				_object.terminateUniformBuffer();

				_object.terminateTextures();

				_object.terminateIndexBuffer();

				_object.terminateVertexBuffer();

				_object.terminateDescriptorSetLayout();
			}
		}

		void VulkanRBL3D::shutDownStageThree()
		{
			myVulkanCore.terminateSyncObjects();

			myVulkanCore.terminateDevice();

			myVulkanCore.terminateDebugger();

			myVulkanCore.terminateInstance();
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
				pipelineInitInfo.multisamplerMsaaSamples = myVulkanCore.getMsaaSamples();
				pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
				pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
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
				_renderObject.initializeUniformBuffer(myVulkanCore.getSwapChainImages().size());

				// initialize descriptor pool
				ADGRVulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
				_renderObject.initializeDescriptorPool(descriptorPoolInitInfo);

				// initialize descriptor sets
				ADGRVulkanDescriptorSetsInitInfo descriptorSetsInitInfo;
				_renderObject.initializeDescriptorSets(descriptorSetsInitInfo);

				renderableObjects.push_back(_renderObject);
			}
		}

		void VulkanRBL3D::initializeRenderPass()
		{
			ARRAY<VkAttachmentDescription> attachments;

			// attachment descriptions
			VkAttachmentDescription colorAttachment = {};
			colorAttachment.format = myVulkanCore.getSwapChainImageFormat();
			colorAttachment.samples = myVulkanCore.getMsaaSamples();
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
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
			depthAttachment.format = VulkanFunctions::findDepthFormat(myVulkanCore.getPhysicalDevice());
			depthAttachment.samples = myVulkanCore.getMsaaSamples();
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
			colorAttachmentResolve.format = myVulkanCore.getSwapChainImageFormat();
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
			myVulkanCore.initializeRenderPass(renderPassInitInfo);
		}
	}
}