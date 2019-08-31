/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "vulkanRenderer.h"

#include "Platform/Windows.h"
#include "keyCodes.h"

#include "core/data structures/vertex.h"

#include "core/functions/textureFunctions.h"
#include "core/controlHeader.h"

namespace Dynamik {
	namespace ADGR {
		using namespace core;
		using namespace functions;

		FPS myFPSCal;

		vulkanRenderer::vulkanRenderer() {
		}

		vulkanRenderer::~vulkanRenderer() {
		}

		void vulkanRenderer::init() {
			myWindow.init();

			myInstance.init();

			myDebugger.setupDebugMessenger();

			myWindow.createWindowSurface();

			myDevice.init();

			mySwapChain.init();
			mySwapChain.initImageViews();

			myPipeline.initRenderPass();

			// first layout
			DMKUniformBufferCreateDescriptorSetLayoutInfo layoutInfo;
			layoutInfo.layout = &layout;
			layoutInfo.bindIndex = { 0, 1 };
			uniformBuffer.createDescriptorSetLayout(layoutInfo);

			// second layout
			DMKUniformBufferCreateDescriptorSetLayoutInfo layoutInfo2;
			layoutInfo2.layout = &layout2;
			layoutInfo2.bindIndex = { 0, 1 };
			uniformBuffer.createDescriptorSetLayout(layoutInfo2);

			uniformBuffer.initDescriptorPool(&descriptorPool);
			uniformBuffer.initDescriptorPool(&descriptorPool2);

			myShaderManager.loadShader(utils::readFile("E:/Projects/Dynamik Engine/Dynamik/components/Shaders/vert.spv"),
				VERTEX_SHADER);
			myShaderManager.loadShader(utils::readFile("E:/Projects/Dynamik Engine/Dynamik/components/Shaders/frag.spv"),
				FRAGMENT_SHADER);
			myShaderManager.init();

			std::vector<VkDescriptorSetLayout_T*> layouts = { layout, layout2 };
			DMKPipelineInitInfo initInfo;
			initInfo.layouts = layouts;
			myPipeline.init(initInfo);

			myShaderManager.deleteShaders();

			myCommandBufferManager.initCommandPool();

			// TODO: manually initialization
			myColorBufferManager.initResources();

			// TODO: manually initialization
			myDepthBufferManager.init();

			// TODO: manually initialization
			myFrameBufferManager.init();

			// texture creation - init
			DMKInitTextureInfo textureInfo;
			textureInfo.path = "E:/Projects/Dynamik Engine/Dynamik/core assets/textures/chalet.jpg";
			textureInfo.textureImage = &texImage;
			textureInfo.textureImageMemory = &texImageMemory;
			textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			textureInfo.mipLevels = myMipLevel;
			myTextureManager.initTexture(textureInfo);

			// texture - imageViews
			DMKInitTextureImageViewsInfo viewInfo;
			viewInfo.textureImage = texImage;
			viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			viewInfo.mipLevels = myMipLevel;

			DMKCreateImageViewInfo cImgVewinfo;
			cImgVewinfo.device = myDevice.getDeviceCpy();
			cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			cImgVewinfo.image = texImage;
			cImgVewinfo.mipLevels = myMipLevel;
			cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			cImgVewinfo.textureImageView = &textureImageView;
			myTextureManager.initTextureImageViews(viewInfo, cImgVewinfo);

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

			// load Model
			DMKVulkanRendererLoadObjectInfo loadModelInfo;
			loadModelInfo.path = "E:/Projects/Dynamik Engine/Dynamik/core assets/models/chalet.obj";
			loadModelInfo.vertexBufferObject = &terrainVBO;
			loadModelInfo.indexBufferObject = &ibo;
			loadModelInfo.offsets = { 0.0f, 0.0f, 0.0f };
			loadObject(loadModelInfo);

			// load second Model
			DMKVulkanRendererLoadObjectInfo loadModelInfo2;
			loadModelInfo2.path = "E:/Projects/Dynamik Engine/Dynamik/core assets/models/chalet.obj";
			loadModelInfo2.vertexBufferObject = &terrainVBO;
			loadModelInfo2.indexBufferObject = &ibo;
			loadModelInfo2.offsets = { 0.0f, -2.0f, 0.0f };
			loadObject(loadModelInfo2);

			// load third Model
			DMKVulkanRendererLoadObjectInfo loadModelInfo3;
			loadModelInfo3.path = "E:/Projects/Dynamik Engine/Dynamik/core assets/models/chalet.obj";
			loadModelInfo3.vertexBufferObject = &vbo2;
			loadModelInfo3.indexBufferObject = &ibo;
			loadModelInfo3.offsets = { 0.0f, 0.0f, 0.0f };
			loadObject(loadModelInfo3);

			// init vertexBuffer
			DMKVulkanRendererCreateVertexBufferInfo vertBuffInfo;
			vertBuffInfo.buffer = &terrainVertexBuffer;
			vertBuffInfo.bufferMemory = &terrainVertexBufferMemory;
			vertBuffInfo.vertexBufferObject = terrainVBO;
			createVertexBuffer(vertBuffInfo);

			// init secons vertexBuffer
			DMKVulkanRendererCreateVertexBufferInfo vertBuffInfo2;
			vertBuffInfo2.buffer = &vertexBuffer2;
			vertBuffInfo2.bufferMemory = &vertexBufferMemory2;
			vertBuffInfo2.vertexBufferObject = vbo2;
			createVertexBuffer(vertBuffInfo2);

			// init indexBuffer
			DMKVulkanRendererCreateIndexBufferInfo idxBuffInfo;
			idxBuffInfo.buffer = &indexBuffer;
			idxBuffInfo.bufferMemory = &indexBufferMemory;
			idxBuffInfo.indexBufferObject = ibo;
			createIndexBuffer(idxBuffInfo);

			// uniform buffer creation
			DMKVulkanRendererCreateUniformBufferInfo uBuffInfo;
			uBuffInfo.buffer = &uniformBuffers;
			uBuffInfo.bufferMemory = &uniformBufferMemories;
			createUniformBuffer(uBuffInfo);

			// second uniform buffer
			DMKVulkanRendererCreateUniformBufferInfo uBuffInfo2;
			uBuffInfo2.buffer = &uniformBuffers2;
			uBuffInfo2.bufferMemory = &uniformBufferMemories2;
			createUniformBuffer(uBuffInfo2);

			// descriptor pool creation
			uniformBuffer.initDescriptorPool(&descriptorPool);
			uniformBuffer.initDescriptorPool(&descriptorPool2);

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

			// command buffer initialization
			DMKBindCommandBufferInfo commandInfo;
			commandInfo.indices = ibo;
			commandInfo.vertexBuffers = { terrainVertexBuffer, vertexBuffer2 };
			commandInfo.indexBuffer = indexBuffer;
			commandInfo.descriptorSets = { &descriptorSets, &descriptorSets2 };
			myCommandBufferManager.bindCommands(commandInfo);

			// initialize sync objects
			initSyncObjects(myDevice.getDeviceCpy(), &imageAvailableSemaphore,
				&renderFinishedSemaphore, &inFlightFence);
		}

		void vulkanRenderer::shutdown() {
			vkDeviceWaitIdle(myDevice.getDeviceCpy());

			// clear color buffer
			myColorBufferManager.clear();

			// clear depth buffer
			myDepthBufferManager.clear();

			// clear swapChain
			DMKSwapChainCleanUpInfo cleanInfo;
			cleanInfo.uniformBuffers = uniformBuffers;
			cleanInfo.uniformBufferMemories = uniformBufferMemories;

			mySwapChain.cleanUp(cleanInfo);

			// clear textures
			DMKTextureDeleteInfo deleteTexInfo;
			deleteTexInfo.sampler = textureSampler;
			deleteTexInfo.texture = texImage;
			deleteTexInfo.imageView = textureImageView;
			deleteTexInfo.textureImageMemory = texImageMemory;

			myTextureManager.deleteTexture(deleteTexInfo);

			// clear buffers
			// clear index buffer
			DMKindexBufferDeleteInfo deleteIndInfo;
			deleteIndInfo.buffer = indexBuffer;
			deleteIndInfo.bufferMemory = indexBufferMemory;

			myIndexBufferManager.deleteBuffer(deleteIndInfo);

			// clear vertex buffer
			DMKVertexBufferDeleteInfo deleteVertInfo;
			deleteVertInfo.buffer = terrainVertexBuffer;
			deleteVertInfo.bufferMemory = terrainVertexBufferMemory;

			myVertexBufferManager.deleteBuffer(deleteVertInfo);

			// delete frames in flight
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				vkDestroySemaphore(myDevice.getDeviceCpy(), renderFinishedSemaphore[i], nullptr);
				vkDestroySemaphore(myDevice.getDeviceCpy(), imageAvailableSemaphore[i], nullptr);
			}

			// stop the debugger
			myDebugger.deleteDebugger();

			// delete the command pool
			vkDestroyCommandPool(myDevice.getDeviceCpy(), myCommandBufferManager.getCommandPool(), nullptr);

			// delete device and surface
			vkDestroyDevice(myDevice.getDeviceCpy(), nullptr);
			vkDestroySurfaceKHR(myInstance.getInstance(), myInstance.getSurface(), nullptr);

			// clear instance
			myInstance.clear();

			// final
			myWindow.clear();
		}

		void vulkanRenderer::drawFrame() {
			myFPSCal.getFPS();

			vkWaitForFences(myDevice.getDeviceCpy(), 1, &inFlightFence[currentFrame],
				VK_TRUE, std::numeric_limits<uint64_t>::max());

			uint32_t imageIndex;
			VkResult result = vkAcquireNextImageKHR(myDevice.getDeviceCpy(), mySwapChain.getSwapChainCpy(), std::numeric_limits<uint64_t>::max(),
				imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
				std::runtime_error("failed to acquire swap chain image!");

			// draw call

			myEvent = myWindow.getKeyEvent();

			DMKUniformBufferUpdateInfo updateInfo;
			updateInfo.bufferMemory = uniformBufferMemories;
			updateInfo.currentImage = imageIndex;
			updateInfo.turn = { myEvent.turnEventL , myEvent.turnEventR };
			updateInfo.move = { myEvent.moveEventU , myEvent.moveEventD };
			updateInfo.upDown = { myEvent.rotEventD , myEvent.rotEventU };
			updateInfo.rotation = { myEvent.rotEventL , myEvent.rotEventR };

			uniformBuffer.updateBuffer(updateInfo);

			//second
			DMKUniformBufferUpdateInfo updateInfo2;
			updateInfo2.bufferMemory = uniformBufferMemories2;
			updateInfo2.currentImage = imageIndex;
			updateInfo2.turn = { myEvent.turnEventL , myEvent.turnEventR };
			updateInfo2.move = { myEvent.moveEventU , myEvent.moveEventD };
			updateInfo2.upDown = { myEvent.rotEventD , myEvent.rotEventU };
			updateInfo2.rotation = { myEvent.rotEventL , myEvent.rotEventR };

			uniformBuffer.updateBuffer(updateInfo2);

			// others
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { imageAvailableSemaphore[currentFrame] };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = myCommandBufferManager.getBuffer(imageIndex);

			VkSemaphore signalSemaphores[] = { renderFinishedSemaphore[currentFrame] };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			vkResetFences(myDevice.getDeviceCpy(), 1, &inFlightFence[currentFrame]);

			if (vkQueueSubmit(*myDevice.getGraphicsQueue(), 1, &submitInfo, inFlightFence[currentFrame]) != VK_SUCCESS)
				std::runtime_error("failed to submit draw command buffer!");

			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			VkSwapchainKHR swapChains[] = { mySwapChain.getSwapChainCpy() };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &imageIndex;

			result = vkQueuePresentKHR(*myDevice.getPresentQueue(), &presentInfo);

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || myWindow.isFrameBufferResized()) {
				myWindow.frameBufferResizedUpdate(false);
				recreateSwapChain();
			}
			else if (result != VK_SUCCESS)
				std::runtime_error("failed to present swap chain image!");

			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		}

		void vulkanRenderer::recreateSwapChain() {
			myWindow.onWindowResizeEvent();

			vkDeviceWaitIdle(myDevice.getDeviceCpy());

			DMKSwapChainCleanUpInfo cleanInfo;
			cleanInfo.uniformBuffers = uniformBuffers;
			cleanInfo.uniformBufferMemories = uniformBufferMemories;

			mySwapChain.cleanUp(cleanInfo);

			mySwapChain.init();
			mySwapChain.initImageViews();

			myPipeline.initRenderPass();

			myShaderManager.loadShader(utils::readFile("E:/Projects/Dynamik Engine/Dynamik/components/Shaders/vert.spv"),
				VERTEX_SHADER);
			myShaderManager.loadShader(utils::readFile("E:/Projects/Dynamik Engine/Dynamik/components/Shaders/frag.spv"),
				FRAGMENT_SHADER);
			myShaderManager.init();

			std::vector<VkDescriptorSetLayout_T*> layouts = { layout, layout2 };
			DMKPipelineInitInfo initInfo;
			initInfo.layouts = layouts;

			myPipeline.init(initInfo);
			myShaderManager.deleteShaders();

			// TODO: manually initialization
			myColorBufferManager.initResources();

			// TODO: manually initialization
			myDepthBufferManager.init();

			// TODO: manually initialization
			myFrameBufferManager.init();

			// uniform buffer creation
			DMKUniformBufferCreateInfo unibuffinfo;
			unibuffinfo.buffers = &uniformBuffers;
			unibuffinfo.bufferMemories = &uniformBufferMemories;

			uniformBuffer.createUniformBuffers(unibuffinfo);

			// second uniform buffer
			DMKUniformBufferCreateInfo unibuffinfo2;
			unibuffinfo2.buffers = &uniformBuffers2;
			unibuffinfo2.bufferMemories = &uniformBufferMemories2;

			uniformBuffer.createUniformBuffers(unibuffinfo2);

			uniformBuffer.initDescriptorPool(&descriptorPool);
			uniformBuffer.initDescriptorPool(&descriptorPool2);

			DMKDescriptorSetsInitInfo descripInfo;
			descripInfo.uniformBuffers = &uniformBuffers;
			descripInfo.textureImageView = textureImageView;
			descripInfo.textureSampler = textureSampler;
			descripInfo.descriptorSets = &descriptorSets;
			descripInfo.layout = &layout;
			descripInfo.descriptorPool = descriptorPool;
			descripInfo.bindIndex = { 0, 1 };

			uniformBuffer.initDescriptorSets(descripInfo);

			// second descriptor set
			DMKDescriptorSetsInitInfo descripInfo2;
			descripInfo2.uniformBuffers = &uniformBuffers2;
			descripInfo2.textureImageView = textureImageView;
			descripInfo2.textureSampler = textureSampler;
			descripInfo2.descriptorSets = &descriptorSets2;
			descripInfo2.layout = &layout2;
			descripInfo2.descriptorPool = descriptorPool2;
			descripInfo2.bindIndex = { 0, 1 };

			uniformBuffer.initDescriptorSets(descripInfo2);

			// command buffer initialization

			// command buffer initialization
			DMKBindCommandBufferInfo commandInfo;
			commandInfo.indices = ibo;
			commandInfo.vertexBuffers = { terrainVertexBuffer, vertexBuffer2 };
			commandInfo.indexBuffer = indexBuffer;
			commandInfo.descriptorSets = { &descriptorSets, &descriptorSets2 };

			myCommandBufferManager.bindCommands(commandInfo);
		}

		void vulkanRenderer::loadObjectData() {
			// texture creation
			DMKInitTextureInfo textureInfo;
			textureInfo.path = "";
			textureInfo.textureImage = &texImage;
			textureInfo.textureImageMemory = &texImageMemory;
			textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;

			myTextureManager.initTexture(textureInfo);

			DMKInitTextureImageViewsInfo viewInfo;
			viewInfo.textureImage = texImage;
			viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			viewInfo.mipLevels = 1;

			DMKCreateImageViewInfo cImgVewinfo;
			cImgVewinfo.device = myDevice.getDeviceCpy();
			cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			cImgVewinfo.image = texImage;
			cImgVewinfo.mipLevels = 1;
			cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

			myTextureManager.initTextureImageViews(viewInfo, cImgVewinfo);
			myTextureManager.initTextureSampler();

			// model creation

			DMKModelLoadInfo modelInfo;
			modelInfo.path = "";
			modelInfo.vertices = &terrainVBO;
			modelInfo.indices = &ibo;

			myModelManager.loadModel(modelInfo);

			// vertex buff creation
			DMKVertexBufferCreateInfo vertexBufferInfo;
			vertexBufferInfo.buffer = &terrainVertexBuffer;
			vertexBufferInfo.buffereMemory = &terrainVertexBufferMemory;
			vertexBufferInfo.vertices = terrainVBO;

			myVertexBufferManager.createVertexBuffer(vertexBufferInfo);

			// index buff creation
			DMKIndexBufferCreateInfo indexBufferInfo;
			indexBufferInfo.buffer = &indexBuffer;
			indexBufferInfo.buffereMemory = &indexBufferMemory;
			indexBufferInfo.indices = ibo;

			myIndexBufferManager.createIndexBuffer(indexBufferInfo);

			// uniform buffer creation
			DMKUniformBufferCreateInfo unibuffinfo;
			unibuffinfo.buffers = &uniformBuffers;
			unibuffinfo.bufferMemories = &uniformBufferMemories;

			uniformBuffer.createUniformBuffers(unibuffinfo);

			uniformBuffer.initDescriptorPool(&descriptorPool);
			uniformBuffer.initDescriptorPool(&descriptorPool2);

			DMKDescriptorSetsInitInfo descripInfo;
			descripInfo.uniformBuffers = &uniformBuffers;
			descripInfo.textureImageView = textureImageView;
			descripInfo.textureSampler = textureSampler;
			descripInfo.descriptorSets = &descriptorSets;

			uniformBuffer.initDescriptorSets(descripInfo);
		}

		void vulkanRenderer::loadObject(DMKVulkanRendererLoadObjectInfo info) {
			DMKModelLoadInfo modelInfo;
			modelInfo.path = info.path;
			modelInfo.vertices = info.vertexBufferObject;
			modelInfo.indices = info.indexBufferObject;
			modelInfo.vertexOffset = info.offsets;

			myModelManager.loadModel(modelInfo);
		}

		void vulkanRenderer::createVertexBuffer(DMKVulkanRendererCreateVertexBufferInfo info) {
			DMKVertexBufferCreateInfo vertexBufferInfo;
			vertexBufferInfo.buffer = info.buffer;
			vertexBufferInfo.buffereMemory = info.bufferMemory;
			vertexBufferInfo.vertices = info.vertexBufferObject;

			myVertexBufferManager.createVertexBuffer(vertexBufferInfo);
		}

		void vulkanRenderer::createIndexBuffer(DMKVulkanRendererCreateIndexBufferInfo info) {
			DMKIndexBufferCreateInfo indexBufferInfo;
			indexBufferInfo.buffer = info.buffer;
			indexBufferInfo.buffereMemory = info.bufferMemory;
			indexBufferInfo.indices = info.indexBufferObject;

			myIndexBufferManager.createIndexBuffer(indexBufferInfo);
		}

		void vulkanRenderer::createUniformBuffer(DMKVulkanRendererCreateUniformBufferInfo info) {
			DMKUniformBufferCreateInfo unibuffinfo;
			unibuffinfo.buffers = info.buffer;
			unibuffinfo.bufferMemories = info.bufferMemory;

			uniformBuffer.createUniformBuffers(unibuffinfo);
		}

		void vulkanRenderer::createDescriptorSets(DMKVulkanRendereCreateDescriptorSetsInfo info) {
			DMKDescriptorSetsInitInfo descripInfo;
			descripInfo.uniformBuffers = info.uniformBuffers;
			descripInfo.textureImageView = info.textureImageView;
			descripInfo.textureSampler = info.textureSampler;
			descripInfo.descriptorSets = info.descriptorSets;
			descripInfo.layout = info.layout;
			descripInfo.descriptorPool = info.descriptorPool;
			descripInfo.bindIndex = { (int)info.bindIndexes[0], (int)info.bindIndexes[1] };

			uniformBuffer.initDescriptorSets(descripInfo);
		}
	}
}
