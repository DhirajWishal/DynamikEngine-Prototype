/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer (2D)

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		22/09/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "vulkanRenderer2D.h"

#include "Platform/Windows.h"
#include "keyCodes.h"

#include "core/data structures/vertex.h"
#include "core/functions/textureFunctions.h"
#include "core/controlHeader.h"

#include "debugger.h"

namespace Dynamik {
	namespace ADGR {
		using namespace core;
		using namespace functions;

		vulkanRenderer2D::vulkanRenderer2D() {
		}

		vulkanRenderer2D::~vulkanRenderer2D() {
		}

		void vulkanRenderer2D::init() {/*
			// initialize window
			myWindow.init();
			INC_PROGRESS;

			// create Vulkan instance
			myInstance.init();
			INC_PROGRESS;

#if defined(DMK_DEBUG)
			// init debugger
			myDebugger.setupDebugMessenger();
			INC_PROGRESS;
			// ----------
#endif

			// create window surface
			myWindow.createWindowSurface();
			INC_PROGRESS;

			// init Vulkan device
			myDevice.init();
			INC_PROGRESS;

			// init swapchain
			mySwapChain.init();
			mySwapChain.initImageViews();
			INC_PROGRESS;

			// init render pass
			myPipeline.initRenderPass();
			INC_PROGRESS;

			// first layout
			DMKUniformBufferCreateDescriptorSetLayoutInfo layoutInfo;
			layoutInfo.layout = &layout;
			layoutInfo.bindIndex = { 0, 1 };
			uniformBuffer.createDescriptorSetLayout(layoutInfo);
			INC_PROGRESS;

			// second layout
			DMKUniformBufferCreateDescriptorSetLayoutInfo layoutInfo2;
			layoutInfo2.layout = &layout2;
			layoutInfo2.bindIndex = { 0, 1 };
			uniformBuffer.createDescriptorSetLayout(layoutInfo2);
			INC_PROGRESS;

			// init descriptor pool
			uniformBuffer.initDescriptorPool(&descriptorPool);
			uniformBuffer.initDescriptorPool(&descriptorPool2);
			INC_PROGRESS;

			// load shaders
			myShaderManager.loadShader(utils::readFile("E:/Projects/Dynamik Engine/Dynamik/components/Shaders/vert.spv"),
				VERTEX_SHADER);
			myShaderManager.loadShader(utils::readFile("E:/Projects/Dynamik Engine/Dynamik/components/Shaders/frag.spv"),
				FRAGMENT_SHADER);
			myShaderManager.init();
			INC_PROGRESS;

			// init pipeline
			DMKPipelineInitInfo initInfo;
			initInfo.layouts = { layout, layout2 };
			myPipeline.init(initInfo);
			INC_PROGRESS;

			// delete shaders
			myShaderManager.deleteShaders();
			INC_PROGRESS;

			// init command pool
			myCommandBufferManager.initCommandPool();
			INC_PROGRESS;

			// TODO: manually initialization
			myColorBufferManager.initResources();
			INC_PROGRESS;

			// TODO: manually initialization
			//myDepthBufferManager.init();
			//INC_PROGRESS;

			// TODO: manually initialization
			myFrameBufferManager.init();
			INC_PROGRESS;

			// texture creation - init
			DMKInitTextureInfo textureInfo;
			textureInfo.path = texturePaths[0];
			textureInfo.textureImage = &texImage;
			textureInfo.textureImageMemory = &texImageMemory;
			textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			textureInfo.mipLevels = myMipLevel;
			myTextureManager.initTexture(textureInfo);
			INC_PROGRESS;

			// texture - imageViews
			DMKInitTextureImageViewsInfo viewInfo;
			viewInfo.textureImage = texImage;
			viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			viewInfo.mipLevels = myMipLevel;
			INC_PROGRESS;

			DMKCreateImageViewInfo cImgVewinfo;
			cImgVewinfo.device = myDevice.getDeviceCpy();
			cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			cImgVewinfo.image = texImage;
			cImgVewinfo.mipLevels = myMipLevel;
			cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			cImgVewinfo.textureImageView = &textureImageView;
			myTextureManager.initTextureImageViews(viewInfo, cImgVewinfo);
			INC_PROGRESS;

			// texture - sampler
			DMKInitTextureSamplerInfo samplerInfo;
			samplerInfo.textureSampler = &textureSampler;
			samplerInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.mipLevel = myMipLevel;
			myTextureManager.initTextureSampler(samplerInfo);
			INC_PROGRESS;

			// load Model
			DMKVulkanRendererLoadObjectInfo loadModelInfo;
			loadModelInfo.path = modelPaths[0];
			loadModelInfo.vertexBufferObject = &terrainVBO;
			loadModelInfo.indexBufferObject = &ibo;
			loadModelInfo.offsets = { 0.0f, 0.0f, 0.0f };
			loadObject(loadModelInfo);
			INC_PROGRESS;

			// load second Model
			DMKVulkanRendererLoadObjectInfo loadModelInfo2;
			loadModelInfo2.path = modelPaths[1];
			loadModelInfo2.vertexBufferObject = &terrainVBO;
			loadModelInfo2.indexBufferObject = &ibo;
			loadModelInfo2.offsets = { 0.0f, -2.0f, 0.0f };
			loadObject(loadModelInfo2);
			INC_PROGRESS;

			// load third Model
			DMKVulkanRendererLoadObjectInfo loadModelInfo3;
			loadModelInfo3.path = modelPaths[0];
			loadModelInfo3.vertexBufferObject = &vbo2;
			loadModelInfo3.indexBufferObject = &ibo;
			loadModelInfo3.offsets = { 0.0f, 0.0f, 0.0f };
			loadObject(loadModelInfo3);
			INC_PROGRESS;

			// init vertexBuffer
			DMKVulkanRendererCreateVertexBufferInfo vertBuffInfo;
			vertBuffInfo.buffer = &terrainVertexBuffer;
			vertBuffInfo.bufferMemory = &terrainVertexBufferMemory;
			vertBuffInfo.vertexBufferObject = terrainVBO;
			createVertexBuffer(vertBuffInfo);
			INC_PROGRESS;

			// init secons vertexBuffer
			DMKVulkanRendererCreateVertexBufferInfo vertBuffInfo2;
			vertBuffInfo2.buffer = &vertexBuffer2;
			vertBuffInfo2.bufferMemory = &vertexBufferMemory2;
			vertBuffInfo2.vertexBufferObject = vbo2;
			createVertexBuffer(vertBuffInfo2);
			INC_PROGRESS;

			// init indexBuffer
			DMKVulkanRendererCreateIndexBufferInfo idxBuffInfo;
			idxBuffInfo.buffer = &indexBuffer;
			idxBuffInfo.bufferMemory = &indexBufferMemory;
			idxBuffInfo.indexBufferObject = ibo;
			createIndexBuffer(idxBuffInfo);
			INC_PROGRESS;

			// uniform buffer creation
			DMKVulkanRendererCreateUniformBufferInfo uBuffInfo;
			uBuffInfo.buffer = &uniformBuffers;
			uBuffInfo.bufferMemory = &uniformBufferMemories;
			createUniformBuffer(uBuffInfo);
			INC_PROGRESS;

			// second uniform buffer
			DMKVulkanRendererCreateUniformBufferInfo uBuffInfo2;
			uBuffInfo2.buffer = &uniformBuffers2;
			uBuffInfo2.bufferMemory = &uniformBufferMemories2;
			createUniformBuffer(uBuffInfo2);
			INC_PROGRESS;

			// descriptor pool creation
			uniformBuffer.initDescriptorPool(&descriptorPool);
			uniformBuffer.initDescriptorPool(&descriptorPool2);
			INC_PROGRESS;

			// init descriptor set
			DMKVulkanRendereCreateDescriptorSetsInfo descInitInfo;
			descInitInfo.uniformBuffers = &uniformBuffers;
			descInitInfo.textureImageView = textureImageView;
			descInitInfo.textureSampler = textureSampler;
			descInitInfo.descriptorSets = &descriptorSets;
			descInitInfo.layout = &layout;
			descInitInfo.descriptorPool = descriptorPool;
			descInitInfo.bindIndexes = { 0, 1 };
			createDescriptorSets(descInitInfo);
			INC_PROGRESS;

			// second descriptor set
			DMKVulkanRendereCreateDescriptorSetsInfo descInitInfo2;
			descInitInfo2.uniformBuffers = &uniformBuffers;
			descInitInfo2.textureImageView = textureImageView;
			descInitInfo2.textureSampler = textureSampler;
			descInitInfo2.descriptorSets = &descriptorSets2;
			descInitInfo2.layout = &layout2;
			descInitInfo2.descriptorPool = descriptorPool2;
			descInitInfo2.bindIndexes = { 0, 1 };
			createDescriptorSets(descInitInfo2);
			INC_PROGRESS;

			// command buffer initialization
			DMKBindCommandBufferInfo commandInfo;
			commandInfo.indices = ibo;
			commandInfo.vertexBuffers = { terrainVertexBuffer, vertexBuffer2 };
			commandInfo.indexBuffer = indexBuffer;
			commandInfo.descriptorSets = { &descriptorSets, &descriptorSets2 };
			myCommandBufferManager.bindCommands(commandInfo);
			INC_PROGRESS;

			// initialize sync objects
			initSyncObjects(myDevice.getDeviceCpy(), &imageAvailableSemaphore,
				&renderFinishedSemaphore, &inFlightFence);
			INC_PROGRESS;*/
		}

		void vulkanRenderer2D::drawFrame() {
		}

		void vulkanRenderer2D::shutDown() {
		}

		// load an object
		void vulkanRenderer2D::loadObject(DMKVulkanRendererLoadObjectInfo info) {
			DMKModelLoadInfo modelInfo;
			modelInfo.path = info.objectDataContainer.path;
			modelInfo.vertices = info.objectDataContainer.vertexBufferObject;
			modelInfo.indices = info.objectDataContainer.indexBufferObject;
			modelInfo.vertexOffset = info.objectDataContainer.offsets;
			myModelManager.loadModel(modelInfo);
		}

		// create vertex buffer
		void vulkanRenderer2D::createVertexBuffer(DMKVulkanRendererCreateVertexBufferInfo info) {/*
			DMKVertexBufferCreateInfo vertexBufferInfo;
			vertexBufferInfo.buffer = info.buffer;
			vertexBufferInfo.buffereMemory = info.bufferMemory;
			vertexBufferInfo.vertices = info.vertexBufferObject;
			myVertexBufferManager.createVertexBuffer(vertexBufferInfo);*/
		}

		// create index buffer
		void vulkanRenderer2D::createIndexBuffer(DMKVulkanRendererCreateIndexBufferInfo info) {/*
			DMKIndexBufferCreateInfo indexBufferInfo;
			indexBufferInfo.buffer = info.buffer;
			indexBufferInfo.buffereMemory = info.bufferMemory;
			indexBufferInfo.indices = info.indexBufferObject;
			myIndexBufferManager.createIndexBuffer(indexBufferInfo);*/
		}

		// create uniform buffer
		void vulkanRenderer2D::createUniformBuffer(DMKVulkanRendererCreateUniformBufferInfo info) {/*
			DMKUniformBufferCreateInfo unibuffinfo;
			unibuffinfo.buffers = info.buffer;
			unibuffinfo.bufferMemories = info.bufferMemory;
			uniformBuffer.createUniformBuffers(unibuffinfo);*/
		}

		// init descriptor sets
		void vulkanRenderer2D::createDescriptorSets(DMKVulkanRendereCreateDescriptorSetsInfo info) {/*
			DMKDescriptorSetsInitInfo descripInfo;
			descripInfo.uniformBuffers = info.uniformBuffers;
			descripInfo.textureImageView = info.textureImageView;
			descripInfo.textureSampler = info.textureSampler;
			descripInfo.descriptorSets = info.descriptorSets;
			descripInfo.layout = info.layout;
			descripInfo.descriptorPool = info.descriptorPool;
			descripInfo.bindIndex = { (int)info.bindIndexes[0], (int)info.bindIndexes[1] };
			uniformBuffer.initDescriptorSets(descripInfo);*/
		}
	}
}