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
			//DMK_DEBUGGER_PROFILER_BEGIN_PATH("Dynamik-Startup.json");
			Debugger::benchmark::beginProfiler("TestFile.json");
			DMK_DEBUGGER_PROFILER_TIMER_START(timer);

			// init GameObjects
			initGameObjects();

			// initialize window
			myWindow.init(&myVulkanDataContainer);
			INC_PROGRESS;

			// create Vulkan instance
			myInstance.init(&myVulkanDataContainer);
			INC_PROGRESS;

#ifdef DMK_DEBUG
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

			// init pipelines
			for (int i = 0; i < objCount; i++) {
				// compile shaders
				myShaderManager.compileShaders(vertexShaderSourcePaths[0], compileShaders);
				myShaderManager.compileShaders(fragmentShaderSourcePaths[0], compileShaders);

				// load shaders
				ADGRVulkanShaderDataContainer shaderContainer = {};
				shaderContainer.shaderCodes = { utils::readFile(vertexShaderPaths[shaderCodeIndex]) ,
												{}, {}, utils::readFile(fragmentShaderPaths[shaderCodeIndex]) };
				myShaderManager.init(&myVulkanDataContainer, &shaderContainer);

				// init pipeline
				DMKPipelineInitInfo initInfo;
				initInfo.layouts = { layout };
				initInfo.shaderDataContainer = shaderContainer;
				auto [l_pipeline, l_pipelineLayout] = myPipeline.init(&myVulkanDataContainer, initInfo);

				objectDataStore[i].pipeline = l_pipeline;
				objectDataStore[i].pipelineLayout = l_pipelineLayout;

				// delete shaders
				ADGRVulkanPipelineDataContainer containerData = {};
				containerData.shaderModules = shaderContainer.shaderModules;
				myShaderManager.deleteShaders(myVulkanDataContainer, containerData);
			}
			for (int i = 0; i < 4; i++)
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

			for (int i = 0; i < objCount; i++) {
				objectDataStore[i].textureImages.resize(objectDataStore[i].texturePaths.size());
				objectDataStore[i].textureImageMemories.resize(objectDataStore[i].texturePaths.size());
				objectDataStore[i].textureSamplers.resize(objectDataStore[i].texturePaths.size());
				objectDataStore[i].textureImageViews.resize(objectDataStore[i].texturePaths.size());

				for (int itr = 0; itr < objectDataStore[i].texturePaths.size(); itr++) {
					// texture creation - init
					DMKInitTextureInfo textureInfo;
					textureInfo.path = objectDataStore[i].texturePaths[itr];
					textureInfo.textureImage = &objectDataStore[i].textureImages[itr];
					textureInfo.textureImageMemory = &objectDataStore[i].textureImageMemories[itr];
					textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
					textureInfo.mipLevels = objectDataStore[i].mipLevel;
					myTextureManager.initTexture(&myVulkanDataContainer, textureInfo);

					// texture - imageViews
					DMKInitTextureImageViewsInfo viewInfo;
					viewInfo.textureImage = objectDataStore[i].textureImages[itr];
					viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
					viewInfo.mipLevels = objectDataStore[i].mipLevel;

					DMKCreateImageViewInfo cImgVewinfo;
					cImgVewinfo.device = myVulkanDataContainer.device;
					cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
					cImgVewinfo.image = objectDataStore[i].textureImages[itr];
					cImgVewinfo.mipLevels = objectDataStore[i].mipLevel;
					cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					cImgVewinfo.textureImageView = &objectDataStore[i].textureImageViews[itr];
					myTextureManager.initTextureImageViews(&myVulkanDataContainer, viewInfo, cImgVewinfo);

					// texture - sampler
					DMKInitTextureSamplerInfo samplerInfo;
					samplerInfo.textureSampler = &objectDataStore[i].textureSamplers[itr];
					samplerInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					samplerInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					samplerInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					samplerInfo.magFilter = VK_FILTER_LINEAR;
					samplerInfo.minFilter = VK_FILTER_LINEAR;
					samplerInfo.mipLevel = objectDataStore[i].mipLevel;
					myTextureManager.initTextureSampler(&myVulkanDataContainer, samplerInfo);
				}
			}
			for (int i = 0; i < 4; i++)
				INC_PROGRESS;

			DMK_INFO("Number of cores: " + std::to_string(std::thread::hardware_concurrency()));

			for (int i = 0; i < objCount; i++) {
				objectDataStore[i].vertexBuffers.resize(1);

				DMKObjectData data = {};
				data.path = objectDataStore[i].paths[0];
				data.vertexBufferObject = &objectDataStore[i].vertexBufferObject;
				data.vertexBuffer = &objectDataStore[i].vertexBuffers[0];
				data.vertexBufferMemory = &objectDataStore[i].vertexBufferMemory;
				data.indexBufferObject = &objectDataStore[i].indexBufferObject;
				data.indexBuffer = &objectDataStore[i].indexBuffer;
				data.indexBufferMemory = &objectDataStore[i].indexBufferMemory;
				data.vertexCount = &objectDataStore[i].vertexCount;
				data.indexCount = &objectDataStore[i].indexCount;
				data.offsets = objectDataStore[i].location;

				objDataVector.push_back(data);
			}

			initModels(objDataVector);

			uniformBuffersContainer.resize(uniformCount);
			uniformBufferMemoriesContainer.resize(uniformCount);

			// uniform buffer creation
			for (int itr = 0; itr < uniformCount; itr++) {
				DMKVulkanRendererCreateUniformBufferInfo uBuffInfo;
				uBuffInfo.buffer = &uniformBuffersContainer[itr];
				uBuffInfo.bufferMemory = &uniformBufferMemoriesContainer[itr];
				createUniformBuffer(uBuffInfo);
			}
			INC_PROGRESS;

			// descriptor pool creation
			descriptorPools.resize(objCount);
			for (int i = 0; i < objCount; i++) {
				// init descriptor pool
				uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPools[i]);

				// init descriptor set
				for (int itr = 0; itr < objectDataStore[i].texturePaths.size(); itr++) {
					DMKVulkanRendereCreateDescriptorSetsInfo descInitInfo;
					descInitInfo.uniformBuffers = &uniformBuffersContainer[0];
					descInitInfo.textureImageView = objectDataStore[i].textureImageViews[itr];
					descInitInfo.textureSampler = objectDataStore[i].textureSamplers[itr];
					descInitInfo.descriptorSets = &objectDataStore[i].descriptorSets;
					descInitInfo.layout = &layout;
					descInitInfo.descriptorPool = descriptorPools[i];
					descInitInfo.bindIndexes = { 0, 1 };
					createDescriptorSets(descInitInfo);
				}
			}
			INC_PROGRESS;
			INC_PROGRESS;

			// command buffer initialization
			DMKBindCommandBufferInfo commandInfo;
			commandInfo.objectBindDatas = objectDataStore;
			myCommandBufferManager.bindCommands(&myVulkanDataContainer, commandInfo);
			INC_PROGRESS;

			// initialize sync objects
			initSyncObjects(myVulkanDataContainer.device, &imageAvailableSemaphore,
				&renderFinishedSemaphore, &inFlightFence);
			INC_PROGRESS;

			//DMK_DEBUGGER_PROFILER_TIMER_STOP(timer);
			//DMK_DEBUGGER_PROFILER_END;
			//Debugger::benchmark::endProfiler();
		}

		void vulkanRenderer::shutdown() {
			DMK_DEBUGGER_PROFILER_TIMER_START(timer);

			// idle
			vkDeviceWaitIdle(myVulkanDataContainer.device);

			// clear depth buffer
			myDepthBufferManager.clear(&myVulkanDataContainer);

			// clear color buffer
			myColorBufferManager.clear(&myVulkanDataContainer);

			// clear swapChain
			std::vector<VkPipeline> pipeline;
			std::vector<VkPipelineLayout> pipelineLayout;
			DMKSwapChainCleanUpInfo cleanInfo;
			cleanInfo.uniformBuffers = uniformBuffersContainer;
			cleanInfo.uniformBufferMemories = uniformBufferMemoriesContainer;
			cleanInfo.descriptorPools = descriptorPools;

			for (auto object : objectDataStore) {
				pipeline.push_back(object.pipeline);
				pipelineLayout.push_back(object.pipelineLayout);
			}

			cleanInfo.pipelines = pipeline;
			cleanInfo.pipelineLayouts = pipelineLayout;
			mySwapChain.cleanUp(&myVulkanDataContainer, cleanInfo);

			// clear textures
			for (int itr = 0; itr < objectDataStore.size(); itr++) {
				for (int i = 0; i < objectDataStore[itr].textureImages.size(); i++) {
					DMKTextureDeleteInfo deleteTexInfo;
					deleteTexInfo.sampler = objectDataStore[itr].textureSamplers[i];
					deleteTexInfo.texture = objectDataStore[itr].textureImages[i];
					deleteTexInfo.imageView = objectDataStore[itr].textureImageViews[i];
					deleteTexInfo.textureImageMemory = objectDataStore[itr].textureImageMemories[i];
					myTextureManager.deleteTexture(&myVulkanDataContainer, deleteTexInfo);
				}

				// clear index buffer
				DMKindexBufferDeleteInfo deleteIndInfo;
				deleteIndInfo.buffer = objectDataStore[itr].indexBuffer;
				deleteIndInfo.bufferMemory = objectDataStore[itr].indexBufferMemory;
				myIndexBufferManager.deleteBuffer(&myVulkanDataContainer, deleteIndInfo);

				// clear vertex buffer
				for (int i = 0; i < objectDataStore[itr].vertexBuffers.size(); i++) {
					DMKVertexBufferDeleteInfo deleteVertInfo;
					deleteVertInfo.buffer = objectDataStore[itr].vertexBuffers[i];
					deleteVertInfo.bufferMemory = objectDataStore[itr].vertexBufferMemory;
					myVertexBufferManager.deleteBuffer(&myVulkanDataContainer, deleteVertInfo);
				}
			}

			// destroy descriptorSetLayout
			vkDestroyDescriptorSetLayout(myVulkanDataContainer.device, layout, nullptr);

			// delete frames in flight
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				vkDestroySemaphore(myVulkanDataContainer.device, renderFinishedSemaphore[i], nullptr);
				vkDestroySemaphore(myVulkanDataContainer.device, imageAvailableSemaphore[i], nullptr);
				vkDestroyFence(myVulkanDataContainer.device, inFlightFence[i], nullptr);
			}

			// delete the command pool
			vkDestroyCommandPool(myVulkanDataContainer.device, myVulkanDataContainer.commandBufferContainer.commandPool, nullptr);

			// delete device
			vkDestroyDevice(myVulkanDataContainer.device, nullptr);

			// stop the debugger
			myDebugger.deleteDebugger();

			// destroy surface
			vkDestroySurfaceKHR(myVulkanDataContainer.instance, myVulkanDataContainer.surface, nullptr);

			// clear instance
			myInstance.clear(&myVulkanDataContainer);

			// final
			myWindow.clear(&myVulkanDataContainer);

			Debugger::benchmark::endProfiler();
		}

		void vulkanRenderer::drawFrame() {
			DMK_DEBUGGER_PROFILER_TIMER_START(timer);

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
			for (int itr = 0; itr < uniformCount; itr++) {
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
				DMK_CORE_FATAL("failed to present swap chain image!")

				// current frame select
				currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		}

		void vulkanRenderer::recreateSwapChain() {
			DMK_DEBUGGER_PROFILER_TIMER_START(timer);

			// window resize event
			myWindow.onWindowResizeEvent();

			// idle
			vkDeviceWaitIdle(myVulkanDataContainer.device);

			// clear swapChain
			std::vector<VkPipeline> pipeline;
			std::vector<VkPipelineLayout> pipelineLayout;
			DMKSwapChainCleanUpInfo cleanInfo;
			cleanInfo.uniformBuffers = uniformBuffersContainer;
			cleanInfo.uniformBufferMemories = uniformBufferMemoriesContainer;
			cleanInfo.descriptorPools = descriptorPools;

			for (auto object : objectDataStore) {
				pipeline.push_back(object.pipeline);
				pipelineLayout.push_back(object.pipelineLayout);
			}

			cleanInfo.pipelines = pipeline;
			cleanInfo.pipelineLayouts = pipelineLayout;
			mySwapChain.cleanUp(&myVulkanDataContainer, cleanInfo);

			// clean frame buffer
			myFrameBufferManager.clear(&myVulkanDataContainer);

			// init swapchain
			mySwapChain.init(&myVulkanDataContainer);

			// init swapchain image views
			mySwapChain.initImageViews(&myVulkanDataContainer);

			// init pipeline
			for (int i = 0; i < objCount; i++) {
				// compile shaders
				myShaderManager.compileShaders(vertexShaderSourcePaths[0], compileShaders);
				myShaderManager.compileShaders(fragmentShaderSourcePaths[0], compileShaders);

				// load shaders
				ADGRVulkanShaderDataContainer shaderContainer = {};
				shaderContainer.shaderCodes = { utils::readFile(vertexShaderPaths[shaderCodeIndex]) ,
												{}, {}, utils::readFile(fragmentShaderPaths[shaderCodeIndex]) };
				myShaderManager.init(&myVulkanDataContainer, &shaderContainer);

				// init pipeline
				DMKPipelineInitInfo initInfo;
				initInfo.layouts = { layout };
				initInfo.shaderDataContainer = shaderContainer;
				auto [l_pipeline, l_pipelineLayout] = myPipeline.init(&myVulkanDataContainer, initInfo);

				objectDataStore[i].pipeline = l_pipeline;
				objectDataStore[i].pipelineLayout = l_pipelineLayout;

				// delete shaders
				ADGRVulkanPipelineDataContainer containerData = {};
				containerData.shaderModules = shaderContainer.shaderModules;
				myShaderManager.deleteShaders(myVulkanDataContainer, containerData);
			}

			// TODO: manually initialization
			myColorBufferManager.initResources(&myVulkanDataContainer);

			// TODO: manually initialization
			myDepthBufferManager.init(&myVulkanDataContainer);

			// TODO: manually initialization
			myFrameBufferManager.init(&myVulkanDataContainer);

			// uniform buffer creation
			for (int itr = 0; itr < uniformCount; itr++) {
				DMKVulkanRendererCreateUniformBufferInfo uBuffInfo;
				uBuffInfo.buffer = &uniformBuffersContainer[itr];
				uBuffInfo.bufferMemory = &uniformBufferMemoriesContainer[itr];
				createUniformBuffer(uBuffInfo);
			}

			// init descriptor pool
			for (int itr = 0; itr < objCount; itr++)
				uniformBuffer.initDescriptorPool(&myVulkanDataContainer, &descriptorPools[itr]);

			// init descriptor set
			for (int i = 0; i < objCount; i++) {
				for (int itr = 0; itr < objectDataStore[i].texturePaths.size(); itr++) {
					DMKVulkanRendereCreateDescriptorSetsInfo descInitInfo;
					descInitInfo.uniformBuffers = &uniformBuffersContainer[0];
					descInitInfo.textureImageView = objectDataStore[i].textureImageViews[itr];
					descInitInfo.textureSampler = objectDataStore[i].textureSamplers[itr];
					descInitInfo.descriptorSets = &objectDataStore[i].descriptorSets;
					descInitInfo.layout = &layout;
					descInitInfo.descriptorPool = descriptorPools[i];
					descInitInfo.bindIndexes = { 0, 1 };
					createDescriptorSets(descInitInfo);
				}
			}

			// init command buffers
			DMKBindCommandBufferInfo commandInfo;
			commandInfo.objectBindDatas = objectDataStore;
			myCommandBufferManager.bindCommands(&myVulkanDataContainer, commandInfo);
			INC_PROGRESS;
		}

		// load object
		std::mutex myMutex;
		void vulkanRenderer::loadObjectData(ADGRVulkanDataContainer* vulkanDataContainer,
			DMKObjectDataCreateInfo& info, ADGRObjectRenderData* renderData) {
			DMK_DEBUGGER_PROFILER_TIMER_START(timer);

			// Created buffers and data stores
			std::vector<Vertex> vertexBufferObject = {};
			VkBuffer vertexBuffer = {};
			VkDeviceMemory vertexBufferMemory = {};

			std::vector<uint32_t> indexBufferObject = {};
			VkBuffer indexBuffer = {};
			VkDeviceMemory indexBufferMemory = {};

			std::vector<VkImage> textureImages = {};
			std::vector<VkDeviceMemory> textureImageMemories = {};
			std::vector<VkSampler> textureSamplers = {};
			std::vector<VkImageView> textureImageViews = {};

			VkDescriptorSetLayout layout = VK_NULL_HANDLE;
			VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
			std::vector<VkDescriptorSet> descriptorSets = {};

			VkPipeline pipeline = VK_NULL_HANDLE;
			VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

			core::uniformBufferManager localUniformBufferManager;
			core::shaderManager localShaderManager;
			core::pipeline localPipelineManager;
			core::textureManager localTextureManager;
			core::vertexBufferManager localVertexBufferManager;
			core::indexBufferManager localIndexBufferManager;

			std::lock_guard<std::mutex> guard(myMutex);

			// init descriptorSetLayout
			DMKUniformBufferCreateDescriptorSetLayoutInfo layoutInfo;
			layoutInfo.layout = &layout;
			layoutInfo.bindIndex = { 0, 1 };
			localUniformBufferManager.createDescriptorSetLayout(vulkanDataContainer, layoutInfo);

			// init descriptorPools
			localUniformBufferManager.initDescriptorPool(vulkanDataContainer, &descriptorPool);

			// compile shaders
			for (std::string path : info.rawShaderPaths)
				localShaderManager.compileShaders(path, info.compileShaders);

			// load shaders
			ADGRVulkanShaderDataContainer shaderContainer = {};
			shaderContainer.shaderCodes = { utils::readFile(info.shaderPaths[0]) ,
											{}, {}, utils::readFile(info.shaderPaths[1]) };
			localShaderManager.init(vulkanDataContainer, &shaderContainer);

			// init pipeline
			DMKPipelineInitInfo initInfo;
			initInfo.layouts = { layout };
			initInfo.shaderDataContainer = shaderContainer;
			auto [A, B] = localPipelineManager.init(vulkanDataContainer, initInfo);

			pipeline = A;
			pipelineLayout = B;

			// delete shaders
			ADGRVulkanPipelineDataContainer pipelineContainer = {};
			pipelineContainer.shaderModules = shaderContainer.shaderModules;
			localShaderManager.deleteShaders(*vulkanDataContainer, pipelineContainer);

			// load textures
			textureImages.resize(info.texturePaths.size());
			textureImageMemories.resize(info.texturePaths.size());
			textureImageViews.resize(info.texturePaths.size());
			textureSamplers.resize(info.texturePaths.size());
			for (int itr = 0; itr < info.texturePaths.size(); itr++) {
				// texture creation - init
				DMKInitTextureInfo textureInfo;
				textureInfo.path = info.texturePaths[itr];
				textureInfo.textureImage = &textureImages[itr];
				textureInfo.textureImageMemory = &textureImageMemories[itr];
				textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
				textureInfo.mipLevels = info.mipLevel;
				localTextureManager.initTexture(vulkanDataContainer, textureInfo);

				// texture - imageViews
				DMKInitTextureImageViewsInfo viewInfo;
				viewInfo.textureImage = textureImages[itr];
				viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
				viewInfo.mipLevels = info.mipLevel;

				DMKCreateImageViewInfo cImgVewinfo;
				cImgVewinfo.device = vulkanDataContainer->device;
				cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
				cImgVewinfo.image = textureImages[itr];
				cImgVewinfo.mipLevels = info.mipLevel;
				cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				cImgVewinfo.textureImageView = &textureImageViews[itr];
				localTextureManager.initTextureImageViews(vulkanDataContainer, viewInfo, cImgVewinfo);

				// texture - sampler
				DMKInitTextureSamplerInfo samplerInfo;
				samplerInfo.textureSampler = &textureSamplers[itr];
				samplerInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.magFilter = VK_FILTER_LINEAR;
				samplerInfo.minFilter = VK_FILTER_LINEAR;
				samplerInfo.mipLevel = info.mipLevel;
				localTextureManager.initTextureSampler(vulkanDataContainer, samplerInfo);
			}

			DMKModelLoadInfo loadInfo = {};
			loadInfo.path = info.modelPath;
			loadInfo.vertices = &vertexBufferObject;
			loadInfo.indices = &indexBufferObject;
			loadInfo.vertexOffset = info.offsets;
			loadModel(loadInfo);

			// init vertexBuffer
			DMKVertexBufferCreateInfo vertexBufferInfo;
			vertexBufferInfo.buffer = &vertexBuffer;
			vertexBufferInfo.buffereMemory = &vertexBufferMemory;
			vertexBufferInfo.vertices = vertexBufferObject;
			localVertexBufferManager.createVertexBuffer(vulkanDataContainer, vertexBufferInfo);

			// init index buffer
			DMKIndexBufferCreateInfo indexBufferInfo;
			indexBufferInfo.buffer = &indexBuffer;
			indexBufferInfo.buffereMemory = &indexBufferMemory;
			indexBufferInfo.indices = indexBufferObject;
			localIndexBufferManager.createIndexBuffer(vulkanDataContainer, indexBufferInfo);

			// init uniform buffers
			DMKUniformBufferCreateInfo unibuffinfo;
			unibuffinfo.buffers = info.uniformBuffers;
			unibuffinfo.bufferMemories = info.uniformBufferMemory;
			localUniformBufferManager.createUniformBuffers(vulkanDataContainer, unibuffinfo);

			// descriptor pool creation
			localUniformBufferManager.initDescriptorPool(vulkanDataContainer, &descriptorPool);

			// init descriptor set
			for (int itr = 0; itr < info.texturePaths.size(); itr++) {
				DMKDescriptorSetsInitInfo descripInfo;
				descripInfo.uniformBuffers = info.uniformBuffers;
				descripInfo.textureImageView = textureImageViews[itr];
				descripInfo.textureSampler = textureSamplers[itr];
				descripInfo.descriptorSets = &descriptorSets;
				descripInfo.layout = &layout;
				descripInfo.descriptorPool = descriptorPool;
				descripInfo.bindIndex = { 0, 1 };

				localUniformBufferManager.initDescriptorSets(vulkanDataContainer, descripInfo);
			}

			// Pack data to be submitted
			//renderData->vertexBuffers = { vertexBuffer };
			//renderData->vertexCount = vertexBufferObject.size();
			//renderData->indexBuffer = indexBuffer;
			//renderData->indexCount = indexBufferObject.size();
			//renderData->descriptorSets = descriptorSets;
			//renderData->pipeline = pipeline;
			//renderData->pipelineLayout = pipelineLayout;
			//renderData->renderingType = info.renderingType;
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
		void vulkanRenderer::setModelPaths(std::vector<std::string>& object, std::vector<std::vector<std::string>>& texture) {
			modelPaths = object;
			texturePaths = texture;
		}

		// set shader paths
		void vulkanRenderer::setShaderPaths(std::vector<std::string>& vertex, std::vector<std::string>& fragment) {
			vertexShaderPaths = vertex;
			fragmentShaderPaths = fragment;
		}

		void vulkanRenderer::initGameObjects() {
			objCount = gameObjectStore.size();

			objectDataStore.resize(objCount);

			for (int i = 0; i < objectDataStore.size(); i++) {
				objectDataStore[i].paths = gameObjectStore[i].myProperties.objectPath;
				objectDataStore[i].texturePaths = gameObjectStore[i].myProperties.texturePaths;
				objectDataStore[i].location = gameObjectStore[i].myProperties.transformProperties.location;
			}
		}

		void vulkanRenderer::initPipeline(DMK_ADGR_CreatePipelineInfo info) {
			// compile shaders
			myShaderManager.compileShaders(vertexShaderSourcePaths[shaderCodeIndex], compileShaders);
			myShaderManager.compileShaders(fragmentShaderSourcePaths[shaderCodeIndex], compileShaders);

			// load shaders
			ADGRVulkanShaderDataContainer shaderContainer = {};
			shaderContainer.shaderCodes = { utils::readFile(vertexShaderPaths[shaderCodeIndex]) ,
											{}, {}, utils::readFile(fragmentShaderPaths[shaderCodeIndex]) };
			myShaderManager.init(&myVulkanDataContainer, &shaderContainer);

			// init pipeline
			DMKPipelineInitInfo initInfo;
			initInfo.layouts = { layout };
			initInfo.shaderDataContainer = shaderContainer;
			auto [pipeline, pipelineLayout] = myPipeline.init(&myVulkanDataContainer, initInfo);

			// delete shaders
			myShaderManager.deleteShaders(myVulkanDataContainer, myVulkanDataContainer.pipelineContainers[0]);
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

		}

		void vulkanRenderer::thread_third(DMKVulkanRendererLoadObjectInfo createInfo) {
			DMK_DEBUGGER_PROFILER_TIMER_START(timer);

			DMKModelLoadInfo info;
			info.path = createInfo.objectDataContainer.path;
			info.vertices = createInfo.objectDataContainer.vertexBufferObject;
			info.indices = createInfo.objectDataContainer.indexBufferObject;
			info.vertexOffset = createInfo.objectDataContainer.offsets;

			loadModel(info);
		}

		void vulkanRenderer::initModels(std::vector<DMKObjectData> data) {
			DMK_DEBUGGER_PROFILER_TIMER_START(timer);
			{
				std::vector<std::future<void>> futures;

				for (auto i = 0; i < data.size(); i++) {
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

				*data[i].vertexCount = (uint32_t)data[i].vertexBufferObject->size();
				data[i].vertexBufferObject->clear();

				// create indexBuffer
				DMKVulkanRendererCreateIndexBufferInfo idxBuffInfo;
				idxBuffInfo.indexBufferObject = *data[i].indexBufferObject;
				idxBuffInfo.buffer = data[i].indexBuffer;
				idxBuffInfo.bufferMemory = data[i].indexBufferMemory;
				createIndexBuffer(idxBuffInfo);

				*data[i].indexCount = (uint32_t)data[i].indexBufferObject->size();
				data[i].indexBufferObject->clear();

				INC_PROGRESS;
			}
		}
	}
}