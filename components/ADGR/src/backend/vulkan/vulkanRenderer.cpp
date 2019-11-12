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

#if defined(DMK_DEBUG) || defined(DMK_RELEASE)
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

			// init descriptor pool
			uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPool);
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
			for (int i = 0; i < 1; i++) {
				DMKPipelineInitInfo initInfo;
				initInfo.layouts = { layout };
				initInfo.shaderDataContainer = shaderContainer;
				myPipeline.init(&myVulkanDataContainer, initInfo);
				INC_PROGRESS;
			}

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

			textureImages.resize(textureSetPaths.size());
			textureImageMemories.resize(textureSetPaths.size());
			textureSamplers.resize(textureSetPaths.size());
			textureImageViews.resize(textureSetPaths.size());

			for (int itr = 0; itr < textureSetPaths.size(); itr++) {
				DMK_INFO(std::to_string(itr));
				// texture creation - init
				DMKInitTextureInfo textureInfo;
				textureInfo.path = textureSetPaths[itr];
				textureInfo.textureImage = &textureImages[itr];
				textureInfo.textureImageMemory = &textureImageMemories[itr];
				textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
				textureInfo.mipLevels = myMipLevel;
				myTextureManager.initTexture(&myVulkanDataContainer, textureInfo);

				// texture - imageViews
				DMKInitTextureImageViewsInfo viewInfo;
				viewInfo.textureImage = textureImages[itr];
				viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
				viewInfo.mipLevels = myMipLevel;

				DMKCreateImageViewInfo cImgVewinfo;
				cImgVewinfo.device = myVulkanDataContainer.device;
				cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
				cImgVewinfo.image = textureImages[itr];
				cImgVewinfo.mipLevels = myMipLevel;
				cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				cImgVewinfo.textureImageView = &textureImageViews[itr];
				myTextureManager.initTextureImageViews(&myVulkanDataContainer, viewInfo, cImgVewinfo);

				// texture - sampler
				DMKInitTextureSamplerInfo samplerInfo;
				samplerInfo.textureSampler = &textureSamplers[itr];
				samplerInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.magFilter = VK_FILTER_LINEAR;
				samplerInfo.minFilter = VK_FILTER_LINEAR;
				samplerInfo.mipLevel = myMipLevel;
				myTextureManager.initTextureSampler(&myVulkanDataContainer, samplerInfo);
			}
			INC_PROGRESS;
			INC_PROGRESS;
			INC_PROGRESS;
			INC_PROGRESS;

			std::vector<std::vector<float>> offsets = {
				{0.0f,	0.0f,	0.0f},
				{0.0f,	2.5f,	0.0f},
				{0.0f,	-2.5f,	0.0f},
				{2.5f,	0.0f,	0.0f},
				{2.5f,	2.5f,	0.0f},
				{2.5f,	-2.5f,	0.0f},
				{-2.5f,	0.0f,	0.0f},
				{-2.5f, 2.5f,	0.0f},
				{-2.5f, -2.5f,	0.0f},

				{0.0f,	-4.0f,	2.0f},
				{0.0f,	0.0f,	-2.0f},
				{0.0f,	2.0f,	-2.0f},
				{0.0f,	-2.0f,	-2.0f},
				{0.0f,	4.0f,	-2.0f},
				{0.0f,	-4.0f,	-2.0f},
				{0.0f,	0.0f,	4.0f},
				{0.0f,	2.0f,	4.0f},
				{0.0f,	-2.0f,	4.0f},
				{0.0f,	4.0f,	4.0f},
				{0.0f,	-4.0f,	4.0f},
				{0.0f,	0.0f,	-4.0f},
				{0.0f,	2.0f,	-4.0f},
				{0.0f,	-2.0f,	-4.0f},
				{0.0f,	4.0f,	-4.0f},
				{0.0f,	-4.0f,	-4.0f}
			};

			std::vector<DMKObjectData> objDataVector = {};

			DMK_INFO(std::to_string(std::thread::hardware_concurrency()));

			vertexBufferObjects.resize(count);
			vertexBuffers.resize(count);
			vertexBufferMemories.resize(count);

			indexBufferObjects.resize(count);
			indexBuffers.resize(count);
			indexBufferMemories.resize(count);

			for (int i = 0; i < count; i++) {
				DMKObjectData data = {};
				data.path = modelPaths[0];
				data.vertexBufferObject = &vertexBufferObjects[i];
				data.vertexBuffer = &vertexBuffers[i];
				data.vertexBufferMemory = &vertexBufferMemories[i];
				data.indexBufferObject = &indexBufferObjects[i];
				data.indexBuffer = &indexBuffers[i];
				data.indexBufferMemory = &indexBufferMemories[i];
				data.offsets = offsets[i];

				objDataVector.push_back(data);
			}

			initModels(objDataVector);

			count = 1;

			uniformBuffersContainer.resize(count);
			uniformBufferMemoriesContainer.resize(count);

			// uniform buffer creation
			for (int itr = 0; itr < count; itr++) {
				DMKVulkanRendererCreateUniformBufferInfo uBuffInfo;
				uBuffInfo.buffer = &uniformBuffersContainer[itr];
				uBuffInfo.bufferMemory = &uniformBufferMemoriesContainer[itr];
				createUniformBuffer(uBuffInfo);
			}
			INC_PROGRESS;

			// descriptor pool creation
			descriptorPools.resize(textureSetPaths.size());
			for (int itr = 0; itr < textureSetPaths.size(); itr++)
				uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPools[itr]);
			INC_PROGRESS;

			// init descriptor set
			descriptorSetsContainer.resize(textureSetPaths.size());
			for (int itr = 0; itr < textureSetPaths.size(); itr++) {
				DMKVulkanRendereCreateDescriptorSetsInfo descInitInfo;
				descInitInfo.uniformBuffers = &uniformBuffersContainer[0];
				descInitInfo.textureImageView = textureImageViews[itr];
				descInitInfo.textureSampler = textureSamplers[itr];
				descInitInfo.descriptorSets = &descriptorSetsContainer[0];
				descInitInfo.layout = &layout;
				descInitInfo.descriptorPool = descriptorPools[itr];
				descInitInfo.bindIndexes = { 0, 1 };
				createDescriptorSets(descInitInfo);
			}
			INC_PROGRESS;

			// command buffer initialization
			DMKBindCommandBufferInfo commandInfo;

			for (int itr = 0; itr < 1; itr++) {
				ADGRObjectRenderData objBindData = {};
				objBindData.vertexBuffers = { vertexBuffers[itr] };
				objBindData.indexBuffer = indexBuffers[itr];
				objBindData.descriptorSets = descriptorSetsContainer[0];
				objBindData.indexCount = indexCounts[itr];
				objBindData.pipeline = myVulkanDataContainer.pipelineContainers[0].pipeline;
				objBindData.pipelineLayout = myVulkanDataContainer.pipelineContainers[0].pipelineLayout;

				commandInfo.objectBindDatas.push_back(objBindData);
			}

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
			vkDestroySurfaceKHR(myVulkanDataContainer.instance, myVulkanDataContainer.surface, nullptr);

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
			VkResult result = vkAcquireNextImageKHR(myVulkanDataContainer.device,
				myVulkanDataContainer.swapchainContainer.swapchain, std::numeric_limits<uint64_t>::max(),
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
			for (int itr = 0; itr < count; itr++) {
				DMKUniformBufferUpdateInfo updateInfo;
				updateInfo.bufferMemory = uniformBufferMemoriesContainer[itr];
				updateInfo.currentImage = imageIndex;
				updateInfo.turn = { myEvent.turnEventL , myEvent.turnEventR };
				updateInfo.move = { myEvent.moveEventU , myEvent.moveEventD };
				updateInfo.upDown = { myEvent.rotEventD , myEvent.rotEventU };
				updateInfo.rotation = { myEvent.rotEventL , myEvent.rotEventR };
				updateInfo.cPos = { myCEvent.x + itr, myCEvent.y + itr };
				uniformBuffer.updateBuffer3D(&myVulkanDataContainer, updateInfo);
			}

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

			// clean frame buffer
			myFrameBufferManager.clear(&myVulkanDataContainer);

			// clear uniform buffers
			uniformBuffers.clear();
			uniformBufferMemories.clear();

			// init swapchain
			mySwapChain.init(&myVulkanDataContainer);

			// init swapchain image views
			mySwapChain.initImageViews(&myVulkanDataContainer);

			// init pipeline render pass
			myPipeline.initRenderPass(&myVulkanDataContainer);

			//myShaderManager.compileShaders(vertexShaderSourcePaths[1], true);
			//myShaderManager.compileShaders(fragmentShaderSourcePaths[1], true);

			// load shaders
			ADGRVulkanShaderDataContainer shaderContainer = {};
			shaderContainer.shaderCodes = { utils::readFile(vertexShaderPaths[shaderCodeIndex]) ,
											{}, {}, utils::readFile(fragmentShaderPaths[shaderCodeIndex]) };
			myShaderManager.init(&myVulkanDataContainer, &shaderContainer);
			INC_PROGRESS;

			// init pipeline
			for (int i = 0; i < 1; i++) {
				DMKPipelineInitInfo initInfo;
				initInfo.layouts = { layout };
				initInfo.shaderDataContainer = shaderContainer;
				myPipeline.init(&myVulkanDataContainer, initInfo);
				INC_PROGRESS;
			}

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
			for (int itr = 0; itr < count; itr++) {
				DMKVulkanRendererCreateUniformBufferInfo uBuffInfo;
				uBuffInfo.buffer = &uniformBuffersContainer[itr];
				uBuffInfo.bufferMemory = &uniformBufferMemoriesContainer[itr];
				createUniformBuffer(uBuffInfo);
			}

			// init descriptor pool
			for (int itr = 0; itr < textureSetPaths.size(); itr++)
				uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPools[itr]);

			// descriptor sets clear
			descriptorSets.clear();

			// init descriptor set
			for (int itr = 0; itr < textureSetPaths.size(); itr++) {
				DMKVulkanRendereCreateDescriptorSetsInfo descInitInfo;
				descInitInfo.uniformBuffers = &uniformBuffersContainer[0];
				descInitInfo.textureImageView = textureImageViews[itr];
				descInitInfo.textureSampler = textureSamplers[itr];
				descInitInfo.descriptorSets = &descriptorSetsContainer[0];
				descInitInfo.layout = &layout;
				descInitInfo.descriptorPool = descriptorPools[itr];
				descInitInfo.bindIndexes = { 0, 1 };
				createDescriptorSets(descInitInfo);
			}

			// init command buffers
			DMKBindCommandBufferInfo commandInfo;

			for (int itr = 0; itr < 1; itr++) {
				ADGRObjectRenderData objBindData = {};
				objBindData.vertexBuffers = { vertexBuffers[itr] };
				objBindData.indexBuffer = indexBuffers[itr];
				objBindData.descriptorSets = descriptorSetsContainer[0];
				objBindData.indexCount = indexCounts[itr];
				objBindData.pipeline = myVulkanDataContainer.pipelineContainers[0].pipeline;
				objBindData.pipelineLayout = myVulkanDataContainer.pipelineContainers[0].pipelineLayout;

				commandInfo.objectBindDatas.push_back(objBindData);
			}

			myCommandBufferManager.bindCommands(&myVulkanDataContainer, commandInfo);
			INC_PROGRESS;
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
			//vertexBufferInfo.vertices = terrainVBO;
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
			initInfo.layouts = { layout };
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
			initInfo.layouts = { layout };
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

		void vulkanRenderer::setDataToSimulation() {
			for (float i = 0; i < 1; i += 0.0005) {
				core::PointVertex vertexData;

				vertexData.Position = {
					i + 0.0001,
					i + 0.0002,
					i + 0.0003
				};

				vertexData.Color = {
					1.0f,
					1.0f,
					1.0f
				};

				simulationVBO.push_back(vertexData);
			}

			DMKVulkanRendererCreateVertexBufferInfo vertBuffInfo;
			//vertBuffInfo.vertexBufferObject = simulationVBO;
			vertBuffInfo.buffer = &simulationVertexBuffer;
			vertBuffInfo.bufferMemory = &simulationVertexBufferMemory;
			createVertexBuffer(vertBuffInfo);
		}

		void vulkanRenderer::initModels(std::vector<DMKObjectData> data) {
			{
				std::vector<std::future<void>> futures;

				for (int i = 0; i < data.size(); i++) {
					DMKVulkanRendererLoadObjectInfo loadModelInfo;
					loadModelInfo.objectDataContainer = data.at(i);

					futures.push_back(std::async(std::launch::async, thread_third, loadModelInfo));
					INC_PROGRESS;
				}
			}

			for (int i = 0; i < data.size(); i++) {
				// create vertexBuffer
				DMKVulkanRendererCreateVertexBufferInfo vertBuffInfo;
				vertBuffInfo.vertexBufferObject = *data[i].vertexBufferObject;
				vertBuffInfo.buffer = data[i].vertexBuffer;
				vertBuffInfo.bufferMemory = data[i].vertexBufferMemory;
				createVertexBuffer(vertBuffInfo);

				// create indexBuffer
				DMKVulkanRendererCreateIndexBufferInfo idxBuffInfo;
				idxBuffInfo.indexBufferObject = *data[i].indexBufferObject;
				idxBuffInfo.buffer = data[i].indexBuffer;
				idxBuffInfo.bufferMemory = data[i].indexBufferMemory;
				createIndexBuffer(idxBuffInfo);
				INC_PROGRESS;
			}

			for (auto odc : data) {
				vertexCounts.push_back(odc.vertexBufferObject->size());
				indexCounts.push_back(odc.indexBufferObject->size());
			}

			for (int i = 0; i < data.size(); i++) {
				data[i].vertexBufferObject->clear();
				data[i].indexBufferObject->clear();
			}
		}
	}
}