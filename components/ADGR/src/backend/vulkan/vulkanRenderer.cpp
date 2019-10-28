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

#include "debugger.h"

namespace Dynamik {
	namespace ADGR {
		using namespace core;
		using namespace functions;

#ifdef DMK_DEBUG
		Debugger::benchmark::FPS myFPSCal;

		// ----------
#endif

		vulkanRenderer::vulkanRenderer() {
		}

		vulkanRenderer::~vulkanRenderer() {
		}

		void vulkanRenderer::init() {
			// initialize window
			myWindow.init(&myVulkanDataContainer);
			INC_PROGRESS;

			// create Vulkan instance
			myInstance.init(&myVulkanDataContainer);
			INC_PROGRESS;

#if defined(DMK_RELEASE)
			// init debugger
			myDebugger.setupDebugMessenger();
			INC_PROGRESS;

			// ----------
#endif

			// create window surface
			myWindow.createWindowSurface(&myVulkanDataContainer);
			INC_PROGRESS;

			// init Vulkan device
			myDevice.init(&myVulkanDataContainer);
			INC_PROGRESS;

			// init swapchain
			mySwapChain.init(&myVulkanDataContainer);
			mySwapChain.initImageViews(&myVulkanDataContainer);
			INC_PROGRESS;

			// first layout
			DMKUniformBufferCreateDescriptorSetLayoutInfo layoutInfo;
			layoutInfo.layout = &layout;
			layoutInfo.bindIndex = { 0, 1 };
			uniformBuffer.createDescriptorSetLayout(&myVulkanDataContainer, layoutInfo);
			INC_PROGRESS;

			// second layout
			//DMKUniformBufferCreateDescriptorSetLayoutInfo layoutInfo2;
			//layoutInfo2.layout = &layout2;
			//layoutInfo2.bindIndex = { 0, 1 };
			//uniformBuffer.createDescriptorSetLayout(&myVulkanDataContainer, layoutInfo2);
			//INC_PROGRESS;

			// init descriptor pool
			uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPool);
			uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPool2);
			INC_PROGRESS;

			// compile shaders
			myShaderManager.compileShaders(vertexShaderSourcePaths[0], compileShaders);
			myShaderManager.compileShaders(fragmentShaderSourcePaths[0], compileShaders);
			INC_PROGRESS;

			// load shaders
			ADGRVulkanShaderDataContainer shaderContainer = {};
			shaderContainer.shaderCodes = { utils::readFile(vertexShaderPaths[shaderCodeIndex]) ,
											{}, {}, utils::readFile(fragmentShaderPaths[shaderCodeIndex]) };
			myShaderManager.init(&myVulkanDataContainer, &shaderContainer);
			INC_PROGRESS;

			// init pipeline
			DMKPipelineInitInfo initInfo;
			initInfo.layouts = { layout/*, layout2*/ };
			initInfo.shaderDataContainer = shaderContainer;
			myPipeline.init(&myVulkanDataContainer, initInfo);
			INC_PROGRESS;

			// delete shaders
			myShaderManager.deleteShaders(myVulkanDataContainer, myVulkanDataContainer.pipelineContainers[0]);
			INC_PROGRESS;

			// init command pool
			myCommandBufferManager.initCommandPool(&myVulkanDataContainer);
			INC_PROGRESS;

			// TODO: manually initialization
			myColorBufferManager.initResources(&myVulkanDataContainer);
			INC_PROGRESS;

			// TODO: manually initialization
			myDepthBufferManager.init(&myVulkanDataContainer);
			INC_PROGRESS;

			// TODO: manually initialization
			myFrameBufferManager.init(&myVulkanDataContainer);
			INC_PROGRESS;

			// texture creation - init
			DMKInitTextureInfo textureInfo;
			textureInfo.path = texturePaths[0];
			textureInfo.textureImage = &texImage;
			textureInfo.textureImageMemory = &texImageMemory;
			textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			textureInfo.mipLevels = myMipLevel;
			myTextureManager.initTexture(&myVulkanDataContainer, textureInfo);
			INC_PROGRESS;

			// texture - imageViews
			DMKInitTextureImageViewsInfo viewInfo;
			viewInfo.textureImage = texImage;
			viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			viewInfo.mipLevels = myMipLevel;
			INC_PROGRESS;

			DMKCreateImageViewInfo cImgVewinfo;
			cImgVewinfo.device = myVulkanDataContainer.device;
			cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			cImgVewinfo.image = texImage;
			cImgVewinfo.mipLevels = myMipLevel;
			cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			cImgVewinfo.textureImageView = &textureImageView;
			myTextureManager.initTextureImageViews(&myVulkanDataContainer, viewInfo, cImgVewinfo);
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
			myTextureManager.initTextureSampler(&myVulkanDataContainer, samplerInfo);
			INC_PROGRESS;

			// texture creation - init 2
			//textureInfo.path = texturePaths[0];
			//textureInfo.textureImage = &texImage2;
			//textureInfo.textureImageMemory = &texImageMemory2;
			//textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			//textureInfo.mipLevels = myMipLevel;
			//myTextureManager.initTexture(&myVulkanDataContainer, textureInfo);
			//INC_PROGRESS;
			//
			// texture - imageViews
			//viewInfo.textureImage = texImage2;
			//viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			//viewInfo.mipLevels = myMipLevel;
			//INC_PROGRESS;
			//
			//cImgVewinfo.device = myVulkanDataContainer.device;
			//cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			//cImgVewinfo.image = texImage2;
			//cImgVewinfo.mipLevels = myMipLevel;
			//cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			//cImgVewinfo.textureImageView = &textureImageView2;
			//myTextureManager.initTextureImageViews(&myVulkanDataContainer, viewInfo, cImgVewinfo);
			//INC_PROGRESS;
			//
			//// texture - sampler
			//samplerInfo.textureSampler = &textureSampler2;
			//samplerInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			//samplerInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			//samplerInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			//samplerInfo.magFilter = VK_FILTER_LINEAR;
			//samplerInfo.minFilter = VK_FILTER_LINEAR;
			//samplerInfo.mipLevel = myMipLevel;
			//myTextureManager.initTextureSampler(&myVulkanDataContainer, samplerInfo);
			//INC_PROGRESS;

			std::vector<std::vector<float>> offsets = {
				{0.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{0.0f, -1.0f, 0.0f}
			};

			DMKObjectData data1 = {};
			data1.path = modelPaths[0];
			data1.vertexBufferObject = &terrainVBO;
			data1.vertexBuffer = &terrainVertexBuffer;
			data1.vertexBufferMemory = &terrainVertexBufferMemory;
			data1.indexBufferObject = &terrainIBO;
			data1.indexBuffer = &terrainIndexBuffer;
			data1.indexBufferMemory = &terrainIndexMemory;
			data1.offsets = offsets[0];

			DMKObjectData data2 = {};
			data2.path = modelPaths[0];
			data2.vertexBufferObject = &vbo;
			data2.vertexBuffer = &vertexBuffer;
			data2.vertexBufferMemory = &vertexBufferMemory;
			data2.indexBufferObject = &ibo;
			data2.indexBuffer = &indexBuffer;
			data2.indexBufferMemory = &indexBufferMemory;
			data2.offsets = offsets[1];

			DMKObjectData data3 = {};
			data3.path = modelPaths[0];
			data3.vertexBufferObject = &vbo2;
			data3.vertexBuffer = &vertexBuffer2;
			data3.vertexBufferMemory = &vertexBufferMemory2;
			data3.indexBufferObject = &ibo2;
			data3.indexBuffer = &indexBuffer2;
			data3.indexBufferMemory = &indexBufferMemory2;
			data3.offsets = offsets[2];

			std::vector<DMKObjectData> objDataVector = {
				data1, data2, data3
			};

			initModels(objDataVector);

			// uniform buffer creation
			DMKVulkanRendererCreateUniformBufferInfo uBuffInfo;
			uBuffInfo.buffer = &uniformBuffers;
			uBuffInfo.bufferMemory = &uniformBufferMemories;
			createUniformBuffer(uBuffInfo);
			INC_PROGRESS;

			// second uniform buffer
			//DMKVulkanRendererCreateUniformBufferInfo uBuffInfo2;
			//uBuffInfo2.buffer = &uniformBuffers2;
			//uBuffInfo2.bufferMemory = &uniformBufferMemories2;
			//createUniformBuffer(uBuffInfo2);
			//INC_PROGRESS;

			// descriptor pool creation
			uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPool);
			//uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPool2);
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
			//DMKVulkanRendereCreateDescriptorSetsInfo descInitInfo2;
			//descInitInfo2.uniformBuffers = &uniformBuffers2;
			//descInitInfo2.textureImageView = textureImageView2;
			//descInitInfo2.textureSampler = textureSampler2;
			//descInitInfo2.descriptorSets = &descriptorSets2;
			//descInitInfo2.layout = &layout;
			//descInitInfo2.descriptorPool = descriptorPool2;
			//descInitInfo2.bindIndexes = { 0, 1 };
			//createDescriptorSets(descInitInfo2);
			//INC_PROGRESS;

			// command buffer initialization
			DMKBindCommandBufferInfo commandInfo;
			commandInfo.vertexBuffers = { terrainVertexBuffer };
			commandInfo.indexBuffers = { terrainIndexBuffer };
			commandInfo.descriptorSets = { &descriptorSets };
			commandInfo.indices = { terrainIBO };
			myCommandBufferManager.bindCommands(&myVulkanDataContainer, commandInfo);
			INC_PROGRESS;

			// initialize sync objects
			initSyncObjects(myVulkanDataContainer.device, &imageAvailableSemaphore,
				&renderFinishedSemaphore, &inFlightFence);
			INC_PROGRESS;
		}

		void vulkanRenderer::shutdown() {
			// idle
			vkDeviceWaitIdle(myVulkanDataContainer.device);

			// clear color buffer
			myColorBufferManager.clear(&myVulkanDataContainer);

			// clear depth buffer
			myDepthBufferManager.clear(&myVulkanDataContainer);

			// clear swapChain
			DMKSwapChainCleanUpInfo cleanInfo;
			cleanInfo.uniformBuffers = uniformBuffers;
			cleanInfo.uniformBufferMemories = uniformBufferMemories;
			cleanInfo.descriptorPool = descriptorPool;
			mySwapChain.cleanUp(&myVulkanDataContainer, cleanInfo);

			// clear textures
			DMKTextureDeleteInfo deleteTexInfo;
			deleteTexInfo.sampler = textureSampler;
			deleteTexInfo.texture = texImage;
			deleteTexInfo.imageView = textureImageView;
			deleteTexInfo.textureImageMemory = texImageMemory;
			myTextureManager.deleteTexture(&myVulkanDataContainer, deleteTexInfo);

			// clear buffers
			// clear index buffer
			DMKindexBufferDeleteInfo deleteIndInfo;
			deleteIndInfo.buffer = indexBuffer;
			deleteIndInfo.bufferMemory = indexBufferMemory;
			myIndexBufferManager.deleteBuffer(&myVulkanDataContainer, deleteIndInfo);

			// clear vertex buffer
			DMKVertexBufferDeleteInfo deleteVertInfo;
			deleteVertInfo.buffer = terrainVertexBuffer;
			deleteVertInfo.bufferMemory = terrainVertexBufferMemory;
			myVertexBufferManager.deleteBuffer(&myVulkanDataContainer, deleteVertInfo);

			// delete frames in flight
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				vkDestroySemaphore(myVulkanDataContainer.device, renderFinishedSemaphore[i], nullptr);
				vkDestroySemaphore(myVulkanDataContainer.device, imageAvailableSemaphore[i], nullptr);
			}

			// stop the debugger
			myDebugger.deleteDebugger();

			// delete the command pool
			vkDestroyCommandPool(myVulkanDataContainer.device, myVulkanDataContainer.commandBufferContainer.commandPool, nullptr);

			// delete device and surface
			vkDestroyDevice(myVulkanDataContainer.device, nullptr);
			vkDestroySurfaceKHR(myInstance.getInstance(), myVulkanDataContainer.surface, nullptr);

			// clear instance
			myInstance.clear(&myVulkanDataContainer);

			// final
			myWindow.clear(&myVulkanDataContainer);
		}

		void vulkanRenderer::drawFrame() {
#ifdef DMK_DEBUG
			myFPSCal.getFPS();	// FPS calculator

 // ----------
#endif

			// wait for fences
			vkWaitForFences(myVulkanDataContainer.device, 1, &inFlightFence[currentFrame],
				VK_TRUE, std::numeric_limits<uint64_t>::max());

			// get image index
			uint32_t imageIndex;
			VkResult result = vkAcquireNextImageKHR(myVulkanDataContainer.device, myVulkanDataContainer.swapchainContainer.swapchain, std::numeric_limits<uint64_t>::max(),
				imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

			// recreate swachain if needed
			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
				DMK_CORE_FATAL("failed to acquire swap chain image!");

			// draw call
			myEvent = myWindow.getKeyEvent();
			myCEvent = myWindow.getCursorEvent();

			// reload shaders event
			if (myEvent.reCompileShaders && shaderCodeIndex == 0)
				shaderCodeIndex = 1;

			else if (myEvent.reCompileShaders && shaderCodeIndex == 1)
				shaderCodeIndex = 0;

			// uniform buffer object update
			DMKUniformBufferUpdateInfo updateInfo;
			updateInfo.bufferMemory = uniformBufferMemories;
			updateInfo.currentImage = imageIndex;
			updateInfo.turn = { myEvent.turnEventL , myEvent.turnEventR };
			updateInfo.move = { myEvent.moveEventU , myEvent.moveEventD };
			updateInfo.upDown = { myEvent.rotEventD , myEvent.rotEventU };
			updateInfo.rotation = { myEvent.rotEventL , myEvent.rotEventR };
			updateInfo.cPos = { myCEvent.x, myCEvent.y };
			uniformBuffer.updateBuffer3D(&myVulkanDataContainer, updateInfo);

			//second
			//DMKUniformBufferUpdateInfo updateInfo2;
			//updateInfo2.bufferMemory = uniformBufferMemories2;
			//updateInfo2.currentImage = imageIndex;
			//updateInfo2.turn = { myEvent.turnEventL , myEvent.turnEventR };
			//updateInfo2.move = { myEvent.moveEventU , myEvent.moveEventD };
			//updateInfo2.upDown = { myEvent.rotEventD , myEvent.rotEventU };
			//updateInfo2.rotation = { myEvent.rotEventL , myEvent.rotEventR };
			//updateInfo2.cPos = { myCEvent.x, myCEvent.y };
			//uniformBuffer.updateBuffer3D(&myVulkanDataContainer, updateInfo2);

			// submit info
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			// wait for semaphores
			VkSemaphore waitSemaphores[] = { imageAvailableSemaphore[currentFrame] };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &myVulkanDataContainer.commandBufferContainer.buffers[imageIndex];

			// signal semaphores
			VkSemaphore signalSemaphores[] = { renderFinishedSemaphore[currentFrame] };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			// reset fences
			vkResetFences(myVulkanDataContainer.device, 1, &inFlightFence[currentFrame]);

			// submit command queue
			if (vkQueueSubmit(myVulkanDataContainer.graphicsQueue, 1, &submitInfo, inFlightFence[currentFrame]) != VK_SUCCESS)
				DMK_CORE_FATAL("failed to submit draw command buffer!");

			// present queue info
			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			// swapchain info
			VkSwapchainKHR swapChains[] = { myVulkanDataContainer.swapchainContainer.swapchain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &imageIndex;

			// submit queue
			result = vkQueuePresentKHR(myVulkanDataContainer.presentQueue, &presentInfo);

			// frame buffer resize event
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || myWindow.isFrameBufferResized() || myEvent.reCompileShaders) {
				myWindow.frameBufferResizedUpdate(false);
				recreateSwapChain();
			}
			else if (result != VK_SUCCESS)
				DMK_CORE_FATAL("failed to present swap chain image!");

			// current frame select
			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		}

		void vulkanRenderer::recreateSwapChain() {
			// window resize event
			myWindow.onWindowResizeEvent();

			// idle
			vkDeviceWaitIdle(myVulkanDataContainer.device);

			// clean swapchain
			DMKSwapChainCleanUpInfo cleanInfo;
			cleanInfo.uniformBuffers = uniformBuffers;
			cleanInfo.uniformBufferMemories = uniformBufferMemories;
			cleanInfo.descriptorPool = descriptorPool;
			mySwapChain.cleanUp(&myVulkanDataContainer, cleanInfo);

			myFrameBufferManager.clear(&myVulkanDataContainer);

			// clean swapchain 2
			//DMKSwapChainCleanUpInfo cleanInfo2;
			//cleanInfo2.uniformBuffers = uniformBuffers2;
			//cleanInfo2.uniformBufferMemories = uniformBufferMemories2;
			//mySwapChain.cleanUp(cleanInfo2);

			// clear uniform buffers
			uniformBuffers.clear();
			uniformBufferMemories.clear();

			// clear uniform buffers 2
			uniformBuffers2.clear();
			uniformBufferMemories2.clear();

			// init swapchain
			mySwapChain.init(&myVulkanDataContainer);

			// init swapchain image views
			mySwapChain.initImageViews(&myVulkanDataContainer);

			// init pipeline render pass
			myPipeline.initRenderPass(&myVulkanDataContainer);

			// load shaders
			ADGRVulkanShaderDataContainer shaderContainer = {};
			shaderContainer.shaderCodes = { utils::readFile(vertexShaderPaths[shaderCodeIndex]) ,
											{}, {}, utils::readFile(fragmentShaderPaths[shaderCodeIndex]) };
			myShaderManager.init(&myVulkanDataContainer, &shaderContainer);
			INC_PROGRESS;

			// init pipeline
			DMKPipelineInitInfo initInfo;
			initInfo.layouts = { layout, layout2 };
			initInfo.shaderDataContainer = shaderContainer;
			myPipeline.init(&myVulkanDataContainer, initInfo);
			INC_PROGRESS;

			// delete shaders
			myShaderManager.deleteShaders(myVulkanDataContainer, myVulkanDataContainer.pipelineContainers[0]);
			INC_PROGRESS;

			// TODO: manually initialization
			myColorBufferManager.initResources(&myVulkanDataContainer);

			// TODO: manually initialization
			myDepthBufferManager.init(&myVulkanDataContainer);

			// TODO: manually initialization
			myFrameBufferManager.init(&myVulkanDataContainer);

			// uniform buffer creation
			DMKUniformBufferCreateInfo unibuffinfo;
			unibuffinfo.buffers = &uniformBuffers;
			unibuffinfo.bufferMemories = &uniformBufferMemories;
			uniformBuffer.createUniformBuffers(&myVulkanDataContainer, unibuffinfo);

			// second uniform buffer
			DMKUniformBufferCreateInfo unibuffinfo2;
			unibuffinfo2.buffers = &uniformBuffers2;
			unibuffinfo2.bufferMemories = &uniformBufferMemories2;
			uniformBuffer.createUniformBuffers(&myVulkanDataContainer, unibuffinfo2);

			// init descriptor pool
			uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPool);

			// init second descriptor pool
			uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPool2);

			// descriptor sets clear
			descriptorSets.clear();
			descriptorSets2.clear();

			// init descriptor set
			DMKDescriptorSetsInitInfo descripInfo;
			descripInfo.uniformBuffers = &uniformBuffers;
			descripInfo.textureImageView = textureImageView;
			descripInfo.textureSampler = textureSampler;
			descripInfo.descriptorSets = &descriptorSets;
			descripInfo.layout = &layout;
			descripInfo.descriptorPool = descriptorPool;
			descripInfo.bindIndex = { 0, 1 };
			uniformBuffer.initDescriptorSets(&myVulkanDataContainer, descripInfo);

			// second descriptor set
			DMKDescriptorSetsInitInfo descripInfo2;
			descripInfo2.uniformBuffers = &uniformBuffers2;
			descripInfo2.textureImageView = textureImageView;
			descripInfo2.textureSampler = textureSampler;
			descripInfo2.descriptorSets = &descriptorSets2;
			descripInfo2.layout = &layout2;
			descripInfo2.descriptorPool = descriptorPool2;
			descripInfo2.bindIndex = { 0, 1 };
			uniformBuffer.initDescriptorSets(&myVulkanDataContainer, descripInfo2);

			terrainVBO = vertexBuffers[0];
			ibo = indexBuffers[0];

			// command buffer initialization
			DMKBindCommandBufferInfo commandInfo;
			commandInfo.vertexBuffers = { terrainVertexBuffer };
			commandInfo.indices = { ibo };
			commandInfo.indexBuffers = { indexBuffer };
			commandInfo.descriptorSets = { &descriptorSets, &descriptorSets2 };
			myCommandBufferManager.bindCommands(&myVulkanDataContainer, commandInfo);
			//INC_PROGRESS;
		}

		// load object 
		void vulkanRenderer::loadObjectData() {
			// texture creation
			DMKInitTextureInfo textureInfo;
			textureInfo.path = "";
			textureInfo.textureImage = &texImage;
			textureInfo.textureImageMemory = &texImageMemory;
			textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			myTextureManager.initTexture(&myVulkanDataContainer, textureInfo);

			// init texture image views
			DMKInitTextureImageViewsInfo viewInfo;
			viewInfo.textureImage = texImage;
			viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			viewInfo.mipLevels = 1;

			DMKCreateImageViewInfo cImgVewinfo;
			cImgVewinfo.device = myVulkanDataContainer.device;
			cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			cImgVewinfo.image = texImage;
			cImgVewinfo.mipLevels = 1;
			cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			myTextureManager.initTextureImageViews(&myVulkanDataContainer, viewInfo, cImgVewinfo);

			// texture - sampler
			DMKInitTextureSamplerInfo samplerInfo;
			samplerInfo.textureSampler = &textureSampler;
			samplerInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.mipLevel = myMipLevel;
			myTextureManager.initTextureSampler(&myVulkanDataContainer, samplerInfo);
			INC_PROGRESS;

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
			myVertexBufferManager.createVertexBuffer(&myVulkanDataContainer, vertexBufferInfo);

			// index buff creation
			DMKIndexBufferCreateInfo indexBufferInfo;
			indexBufferInfo.buffer = &indexBuffer;
			indexBufferInfo.buffereMemory = &indexBufferMemory;
			indexBufferInfo.indices = ibo;
			myIndexBufferManager.createIndexBuffer(&myVulkanDataContainer, indexBufferInfo);

			// uniform buffer creation
			DMKUniformBufferCreateInfo unibuffinfo;
			unibuffinfo.buffers = &uniformBuffers;
			unibuffinfo.bufferMemories = &uniformBufferMemories;
			uniformBuffer.createUniformBuffers(&myVulkanDataContainer, unibuffinfo);

			// init descriptor pool
			uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPool);
			uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPool2);

			// init descriptor set
			DMKDescriptorSetsInitInfo descripInfo;
			descripInfo.uniformBuffers = &uniformBuffers;
			descripInfo.textureImageView = textureImageView;
			descripInfo.textureSampler = textureSampler;
			descripInfo.descriptorSets = &descriptorSets;
			uniformBuffer.initDescriptorSets(&myVulkanDataContainer, descripInfo);
		}

		// load an object
		void vulkanRenderer::loadObject(DMKVulkanRendererLoadObjectInfo info) {
			DMKModelLoadInfo modelInfo;
			modelInfo.path = info.objectDataContainer.path;
			modelInfo.vertices = info.objectDataContainer.vertexBufferObject;
			modelInfo.indices = info.objectDataContainer.indexBufferObject;
			modelInfo.vertexOffset = info.objectDataContainer.offsets;
			myModelManager.loadModel(modelInfo);
		}

		// set model paths
		void vulkanRenderer::setModelPaths(std::vector<std::string>& object, std::vector<std::string>& texture) {
			modelPaths = object;
			texturePaths = texture;
		}

		// set shader paths
		void vulkanRenderer::setShaderPaths(std::vector<std::string>& vertex, std::vector<std::string>& fragment) {
			vertexShaderPaths = vertex;
			fragmentShaderPaths = fragment;
		}

		void vulkanRenderer::initPipeline(DMK_ADGR_CreatePipelineInfo info) {
			// compile shaders
			myShaderManager.compileShaders(vertexShaderSourcePaths[shaderCodeIndex], compileShaders);
			myShaderManager.compileShaders(fragmentShaderSourcePaths[shaderCodeIndex], compileShaders);
			INC_PROGRESS;

			// load shaders
			ADGRVulkanShaderDataContainer shaderContainer = {};
			shaderContainer.shaderCodes = { utils::readFile(vertexShaderPaths[shaderCodeIndex]) ,
											{}, {}, utils::readFile(fragmentShaderPaths[shaderCodeIndex]) };
			myShaderManager.init(&myVulkanDataContainer, &shaderContainer);
			INC_PROGRESS;

			// init pipeline
			DMKPipelineInitInfo initInfo;
			initInfo.layouts = { layout, layout2 };
			initInfo.shaderDataContainer = shaderContainer;
			myPipeline.init(&myVulkanDataContainer, initInfo);
			INC_PROGRESS;

			// delete shaders
			myShaderManager.deleteShaders(myVulkanDataContainer, myVulkanDataContainer.pipelineContainers[0]);
			INC_PROGRESS;
		}

		// create vertex buffer
		void vulkanRenderer::createVertexBuffer(DMKVulkanRendererCreateVertexBufferInfo info) {
			DMKVertexBufferCreateInfo vertexBufferInfo;
			vertexBufferInfo.buffer = info.buffer;
			vertexBufferInfo.buffereMemory = info.bufferMemory;
			vertexBufferInfo.vertices = info.vertexBufferObject;
			myVertexBufferManager.createVertexBuffer(&myVulkanDataContainer, vertexBufferInfo);
		}

		// create index buffer
		void vulkanRenderer::createIndexBuffer(DMKVulkanRendererCreateIndexBufferInfo info) {
			DMKIndexBufferCreateInfo indexBufferInfo;
			indexBufferInfo.buffer = info.buffer;
			indexBufferInfo.buffereMemory = info.bufferMemory;
			indexBufferInfo.indices = info.indexBufferObject;
			myIndexBufferManager.createIndexBuffer(&myVulkanDataContainer, indexBufferInfo);
		}

		// create uniform buffer
		void vulkanRenderer::createUniformBuffer(DMKVulkanRendererCreateUniformBufferInfo info) {
			DMKUniformBufferCreateInfo unibuffinfo;
			unibuffinfo.buffers = info.buffer;
			unibuffinfo.bufferMemories = info.bufferMemory;
			uniformBuffer.createUniformBuffers(&myVulkanDataContainer, unibuffinfo);
		}

		// init descriptor sets
		void vulkanRenderer::createDescriptorSets(DMKVulkanRendereCreateDescriptorSetsInfo info) {
			DMKDescriptorSetsInitInfo descripInfo;
			descripInfo.uniformBuffers = info.uniformBuffers;
			descripInfo.textureImageView = info.textureImageView;
			descripInfo.textureSampler = info.textureSampler;
			descripInfo.descriptorSets = info.descriptorSets;
			descripInfo.layout = info.layout;
			descripInfo.descriptorPool = info.descriptorPool;
			descripInfo.bindIndex = { (int)info.bindIndexes[0], (int)info.bindIndexes[1] };
			uniformBuffer.initDescriptorSets(&myVulkanDataContainer, descripInfo);
		}

		// load another shader
		void vulkanRenderer::includeShader() {
			// load shaders
			ADGRVulkanShaderDataContainer shaderContainer = {};
			shaderContainer.shaderCodes = { utils::readFile(vertexShaderPaths[shaderCodeIndex]) ,
											{}, {}, utils::readFile(fragmentShaderPaths[shaderCodeIndex]) };
			myShaderManager.init(&myVulkanDataContainer, &shaderContainer);
			INC_PROGRESS;

			// init pipeline
			DMKPipelineInitInfo initInfo;
			initInfo.layouts = { layout, layout2 };
			initInfo.shaderDataContainer = shaderContainer;
			myPipeline.init(&myVulkanDataContainer, initInfo);
			INC_PROGRESS;

			// delete shaders
			myShaderManager.deleteShaders(myVulkanDataContainer, myVulkanDataContainer.pipelineContainers[0]);
			INC_PROGRESS;
		}

		// texture creation - init
		void vulkanRenderer::thread_second() {
			DMKInitTextureInfo textureInfo;
			textureInfo.path = texturePaths[0];
			textureInfo.textureImage = &texImage;
			textureInfo.textureImageMemory = &texImageMemory;
			textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			textureInfo.mipLevels = myMipLevel;
			myTextureManager.initTexture(&myVulkanDataContainer, textureInfo);
			INC_PROGRESS;
			// texture - imageViews
			DMKInitTextureImageViewsInfo viewInfo;
			viewInfo.textureImage = texImage;
			viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			viewInfo.mipLevels = myMipLevel;
			INC_PROGRESS;
			DMKCreateImageViewInfo cImgVewinfo;
			cImgVewinfo.device = myVulkanDataContainer.device;
			cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			cImgVewinfo.image = texImage;
			cImgVewinfo.mipLevels = myMipLevel;
			cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			cImgVewinfo.textureImageView = &textureImageView;
			myTextureManager.initTextureImageViews(&myVulkanDataContainer, viewInfo, cImgVewinfo);
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
			myTextureManager.initTextureSampler(&myVulkanDataContainer, samplerInfo);
			INC_PROGRESS;
		}

		void vulkanRenderer::thread_third(DMKVulkanRendererLoadObjectInfo createInfo) {
			DMKModelLoadInfo info;
			info.path = createInfo.objectDataContainer.path;
			info.vertices = createInfo.objectDataContainer.vertexBufferObject;
			info.indices = createInfo.objectDataContainer.indexBufferObject;
			info.vertexOffset = createInfo.objectDataContainer.offsets;

			loadModel(info);
		}

		void vulkanRenderer::thread_basket_1_() {

		}

		void vulkanRenderer::initModels(std::vector<DMKObjectData> data) {
			{
				std::vector<std::future<void>> futures;

				for (int i = 0; i < data.size(); i++) {
					DMKVulkanRendererLoadObjectInfo loadModelInfo;
					loadModelInfo.objectDataContainer = data.at(i);

					futures.push_back(std::async(std::launch::async, thread_third, loadModelInfo));
				}
			}

			for (auto i = 1; i < data.size(); i++) {
				data[0].vertexBufferObject->insert(data[0].vertexBufferObject->end(),
					data[i].vertexBufferObject->begin(), data[i].vertexBufferObject->end());

				data[0].indexBufferObject->insert(data[0].indexBufferObject->end(),
					data[i].indexBufferObject->begin(), data[i].indexBufferObject->end());
			}

			//for (int i = 0; i < data.size(); i++) {
				// create vertexBuffer
			DMKVulkanRendererCreateVertexBufferInfo vertBuffInfo;
			vertBuffInfo.vertexBufferObject = *data[0].vertexBufferObject;
			vertBuffInfo.buffer = data[0].vertexBuffer;
			vertBuffInfo.bufferMemory = data[0].vertexBufferMemory;
			createVertexBuffer(vertBuffInfo);

			// create indexBuffer
			DMKVulkanRendererCreateIndexBufferInfo idxBuffInfo;
			idxBuffInfo.indexBufferObject = *data[0].indexBufferObject;
			idxBuffInfo.buffer = data[0].indexBuffer;
			idxBuffInfo.bufferMemory = data[0].indexBufferMemory;
			createIndexBuffer(idxBuffInfo);
			INC_PROGRESS;
			//}
		}
	}
}
