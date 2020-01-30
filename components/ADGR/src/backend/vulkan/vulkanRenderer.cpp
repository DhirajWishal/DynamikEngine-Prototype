/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 vulkanRenderer.cpp file
*/

#include "adgrafx.h"
#include "vulkanRenderer.h"
#include "Platform/windows.h"
#include "keyCodes.h"

#include "core/functions/textureFunctions.h"
#include "core/controlHeader.h"

#include "debugger.h"
#include "CentralDataHub.h"
#include "Platform.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		using namespace core;
		using namespace functions;

#ifdef DMK_DEBUG
		Debugger::benchmark::FPS myFPSCal;

		// ----------
#endif

		// default constructor
		vulkanRenderer::vulkanRenderer() {
		}

		// default destructor
		vulkanRenderer::~vulkanRenderer() {
		}

		// initialize the renderer
		void vulkanRenderer::init() {	// 14 inc
			initStageOne();
			initStageTwo();
			initStageThree();
		}

		// Basic one-time initializations
		void vulkanRenderer::initStageOne() {
			// Global manager allocations
			initManagerFunctions();
			INC_PROGRESS;

			// initialize window
			initWindow();
			INC_PROGRESS;

			// create Vulkan instance
			initInstance();
			INC_PROGRESS;

			// init debugger
			initDebugger();
			INC_PROGRESS;

			// create window surface
			initWindowSurface();
			INC_PROGRESS;

			// init Vulkan device
			initDevice();
			INC_PROGRESS;

			// init swapchain
			initSwapChain();
			INC_PROGRESS;

			// init command pool
			initCommandPool();
			INC_PROGRESS;

			// TODO: manually initialization
			initColorBuffer();
			INC_PROGRESS;

			// TODO: manually initialization
			initDepthBuffer();
			INC_PROGRESS;

			// init render pass
			initRenderPass();
			INC_PROGRESS;

			// TODO: manually initialization
			initFrameBuffers();
			INC_PROGRESS;

			// other initialization functions
			otherInitFunctions();
		}

		// object loading and initialization
		void vulkanRenderer::initStageTwo() {
			// Per-Object functions
			initObjectBasedFunctions(&myVulkanFormats);
			INC_PROGRESS;
		}

		// final initialization
		void vulkanRenderer::initStageThree() {
			// initialize sync objects
			initSemaphoresAndFences();
			INC_PROGRESS;
		}

		// shutdown the renderer
		void vulkanRenderer::shutDown() {
			// idle vulkanFormat*
			rendererWait();

			// clear depth buffer
			myDepthBufferManager.clear(&myVulkanDataContainers[vulkanContainerIndex]);

			// clear color buffer
			myColorBufferManager.clear(&myVulkanDataContainers[vulkanContainerIndex]);

			for (int itr = 0; itr < myFormatsCount; itr++) {
				vulkanFormat* _localVulkanFormat = &myVulkanFormats[itr];

				// clear swapChain, Pipeline, Uniform buffers and descriptorPool
				DMKSwapChainCleanUpInfo cleanInfo = {};
				cleanInfo.uniformBuffers = { _localVulkanFormat->myUniformBuffers };
				cleanInfo.uniformBufferMemories = { _localVulkanFormat->myUniformBufferMemories };
				cleanInfo.descriptorPools = _localVulkanFormat->myDescriptorPools;
				cleanInfo.pipelines = { _localVulkanFormat->myPipeline };
				cleanInfo.pipelineLayouts = { _localVulkanFormat->myPipelineLayout };
				mySwapChainManager.cleanUp(&myVulkanDataContainers[vulkanContainerIndex], cleanInfo);

				// clear textures
				for (int i = 0; i < _localVulkanFormat->myTextureImages.size(); i++) {
					DMKTextureDeleteInfo deleteTexInfo = {};
					deleteTexInfo.texture = _localVulkanFormat->myTextureImages[i];
					deleteTexInfo.textureImageMemory = _localVulkanFormat->myTextureImageMemories[i];
					deleteTexInfo.sampler = _localVulkanFormat->myTextureImageSamplers[i];
					deleteTexInfo.imageView = _localVulkanFormat->myTextureImageViews[i];
					myTextureManager.deleteTexture(&myVulkanDataContainers[vulkanContainerIndex], deleteTexInfo);
				}

				// clear index buffer
				if (_localVulkanFormat->myRendererFormat->myRenderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED)
					myIndexBufferManager.deleteBuffer(&myVulkanDataContainers[vulkanContainerIndex], _localVulkanFormat);

				// clear vertex buffer
				myVertexBufferManager.deleteBuffer(&myVulkanDataContainers[vulkanContainerIndex], _localVulkanFormat);

				// destroy descriptorSetLayout
				vkDestroyDescriptorSetLayout(myVulkanDataContainers[vulkanContainerIndex].device,
					_localVulkanFormat->myDescriptorSetLayout, nullptr);
			}

			// delete frames in flight
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				vkDestroySemaphore(myVulkanDataContainers[vulkanContainerIndex].device, myImageAvailableSemaphores[i], nullptr);
				vkDestroySemaphore(myVulkanDataContainers[vulkanContainerIndex].device, myRenderFinishedSemaphores[i], nullptr);
				vkDestroyFence(myVulkanDataContainers[vulkanContainerIndex].device, myFencesInFlight[i], nullptr);
			}

			// delete the command pool
			vkDestroyCommandPool(myVulkanDataContainers[vulkanContainerIndex].device,
				myVulkanDataContainers[vulkanContainerIndex].commandBufferContainer.commandPool, nullptr);

			// delete device
			vkDestroyDevice(myVulkanDataContainers[vulkanContainerIndex].device, nullptr);

			// stop the debugger
			myDebugger.deleteDebugger();

			// destroy surface
			vkDestroySurfaceKHR(myVulkanDataContainers[vulkanContainerIndex].instance,
				myVulkanDataContainers[vulkanContainerIndex].surface, nullptr);

			// clear instance
			myInstanceManager.clear(&myVulkanDataContainers[vulkanContainerIndex]);

			// final
			myWindowManager.clear(&myVulkanDataContainers[vulkanContainerIndex]);
		}

		// draw frame
		void vulkanRenderer::drawFrame() {
#ifdef DMK_DEBUG
			myFPSCal.getFPS();	// FPS calculator

 // ----------
#endif

			// wait for fences
			vkWaitForFences(myVulkanDataContainers[vulkanContainerIndex].device, 1, &myFencesInFlight[currentFrame],
				VK_TRUE, std::numeric_limits<uint64_t>::max());

			// get image index
			imageIndex = 0;
			VkResult result = vkAcquireNextImageKHR(myVulkanDataContainers[vulkanContainerIndex].device,
				myVulkanDataContainers[vulkanContainerIndex].swapchainContainer.swapchain, std::numeric_limits<uint64_t>::max(),
				myImageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

			// recreate swachain if needed
			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
				DMK_CORE_FATAL("failed to acquire swap chain image!");

			// draw call
			// uniform buffer object update
			for (int itr = 0; itr < myFormatsCount; itr++)
				myUniformBufferManager.updateBuffer3D(&myVulkanDataContainers[vulkanContainerIndex],
					myEventContainers, &myVulkanFormats[itr], imageIndex);

			// submit info
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			// wait for semaphores
			VkSemaphore waitSemaphores[] = { myImageAvailableSemaphores[currentFrame] };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &myVulkanDataContainers[vulkanContainerIndex].commandBufferContainer.buffers[imageIndex];

			// signal semaphores
			VkSemaphore signalSemaphores[] = { myRenderFinishedSemaphores[currentFrame] };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			// reset fences
			vkResetFences(myVulkanDataContainers[vulkanContainerIndex].device, 1, &myFencesInFlight[currentFrame]);

			// submit command queue
			if (vkQueueSubmit(myVulkanDataContainers[vulkanContainerIndex].graphicsQueue, 1, &submitInfo,
				myFencesInFlight[currentFrame]) != VK_SUCCESS)
				DMK_CORE_FATAL("failed to submit draw command buffer!");

			// present queue info
			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			// swapchain info
			VkSwapchainKHR swapChains[] = { myVulkanDataContainers[vulkanContainerIndex].swapchainContainer.swapchain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &imageIndex;

			// submit queue
			result = vkQueuePresentKHR(myVulkanDataContainers[vulkanContainerIndex].presentQueue, &presentInfo);

			// frame buffer resize event
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || myWindowManager.isFrameBufferResized()) {
				myWindowManager.frameBufferResizedUpdate(false);
				recreateSwapChain();
			}
			else if (result != VK_SUCCESS)
				DMK_CORE_FATAL("failed to present swap chain image!");

			// current frame select
			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		}

		// recreate the swapchain
		void vulkanRenderer::recreateSwapChain() {
			// window resize event
			myWindowManager.onWindowResizeEvent(&myVulkanDataContainers[vulkanContainerIndex]);

			// idle
			vkDeviceWaitIdle(myVulkanDataContainers[vulkanContainerIndex].device);

			// clear swapChain, Pipeline, Uniform buffers and descriptorPool
			for (int itr = 0; itr < myFormatsCount; itr++) {
				vulkanFormat* _localVulkanFormat = &myVulkanFormats[itr];

				DMKSwapChainCleanUpInfo cleanInfo = {};
				cleanInfo.uniformBuffers = { _localVulkanFormat->myUniformBuffers };
				cleanInfo.uniformBufferMemories = { _localVulkanFormat->myUniformBufferMemories };
				cleanInfo.descriptorPools = _localVulkanFormat->myDescriptorPools;
				cleanInfo.pipelines = { _localVulkanFormat->myPipeline };
				cleanInfo.pipelineLayouts = { _localVulkanFormat->myPipelineLayout };
				mySwapChainManager.cleanUp(&myVulkanDataContainers[vulkanContainerIndex], cleanInfo);
			}

			// clean frame buffer
			myFrameBufferManager.clear(&myVulkanDataContainers[vulkanContainerIndex]);

			// init swapchain
			initSwapChain();

			// TODO: manually initialization
			initColorBuffer();

			// TODO: manually initialization
			initDepthBuffer();

			// initialize the render pass
			initRenderPass();

			// TODO: manually initialization
			initFrameBuffers();

			for (uint32_t _itr = 0; _itr < myFormatsCount; _itr++) {
				vulkanFormat* _localVulkanFormat = &myVulkanFormats[_itr];

				// init pipelines
				initPipelines(_localVulkanFormat);

				// uniform buffer creation
				initUniformBuffers(_localVulkanFormat);

				// descriptor pool creation
				initDescriptorPoolsAndSets(_localVulkanFormat);
			}

			// init command buffers
			initCommandBuffers(&myVulkanFormats);
		}

		// events
		std::deque<DMKEventContainer>* vulkanRenderer::events() {
			myWindowManager.pollEvents();
			myEventContainers = myWindowManager.getEventContainer();

			return &myEventContainers;
		}

		void vulkanRenderer::setFormats(std::vector<RendererFormat>& rendererFormats) {
			_addVulkanFormatsToManager(rendererFormats);
		}

		void vulkanRenderer::updateFormats(std::vector<RendererFormat>& rendererFormats) {
			_addVulkanFormatsToManager(rendererFormats);

			initObjectBasedFunctions(&myVulkanFormats);
		}

		// initialize models
		void vulkanRenderer::initModels(std::vector<DMKObjectData> data) {
			{
				std::vector<std::future<void>> futures;

				for (auto i = 0; i < data.size(); i++) {
					DMKVulkanRendererLoadObjectInfo loadModelInfo;
					loadModelInfo.objectDataContainer = data.at(i);

					//futures.push_back(std::async(std::launch::async, thread_third, loadModelInfo));
				}
			}

			for (int i = 0; i < data.size(); i++) {
				// create vertexBuffer
				DMKVulkanRendererCreateVertexBufferInfo vertBuffInfo;
				vertBuffInfo.vertexBufferObject = data[i].vertexBufferObject;
				vertBuffInfo.buffer = &data[i].vertexBuffers->at(0);
				vertBuffInfo.bufferMemory = &data[i].vertexBufferMemories->at(0);

				*data[i].vertexCount = (uint32_t)data[i].vertexBufferObject->size();

				// create indexBuffer
				DMKVulkanRendererCreateIndexBufferInfo idxBuffInfo;
				idxBuffInfo.indexBufferObject = data[i].indexBufferObject;
				idxBuffInfo.buffer = data[i].indexBuffer;
				idxBuffInfo.bufferMemory = data[i].indexBufferMemory;

				*data[i].indexCount = (uint32_t)data[i].indexBufferObject->size();

				if (enableVertexAndIndexClear) {
					data[i].vertexBufferObject->clear();
					data[i].indexBufferObject->clear();
				}
			}
		}

		// initialize the skybox
		void vulkanRenderer::initCubemap(DMKObjectData* data) {
			std::vector<Vertex> _localVertexBufferObject = {};

			float S_SIZE = 2048.0f;

			std::vector<std::vector<float>> locations =
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

			for (int itr = 0; itr < 6; itr++) {
				for (int x = itr * 6; x < (itr * 6 + 6); x++) {
					Vertex v;
					v.Position = { locations[x][0], locations[x][1], locations[x][2] };
					v.Color = { 1.0f, 1.0f, 1.0f };
					v.TexCoordinates = { 0.0f, 0.0f };

					_localVertexBufferObject.push_back(v);
				}

				DMKVulkanRendererCreateVertexBufferInfo vertBuffInfo;
				vertBuffInfo.vertexBufferObject = &_localVertexBufferObject;
				vertBuffInfo.buffer = &data->vertexBuffers->at(itr);
				vertBuffInfo.bufferMemory = &data->vertexBufferMemories->at(itr);
				//createVertexBuffer(vertBuffInfo);

				*data->vertexCount = (uint32_t)_localVertexBufferObject.size();
			}
		}

		void vulkanRenderer::_addVulkanFormatsToManager(std::vector<RendererFormat>& rendererFormats) {
			myFormatsCount = rendererFormats.size();
			myVulkanFormats.clear();
			for (int i = 0; i < myFormatsCount; i++)
				myVulkanFormats.push_back(vulkanFormat(&rendererFormats[i]));
		}

		/* STAGE BASED INITIALIZATION */
		// initialize the manager variables locally
		void vulkanRenderer::initManagerFunctions() {
			myVulkanDataContainers.resize(vulkanContainerIndex + 1);
			myImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
			myRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
			myFencesInFlight.resize(MAX_FRAMES_IN_FLIGHT);
		}

		// initialize the window
		void vulkanRenderer::initWindow() {
			myWindowManager.init(&myVulkanDataContainers[vulkanContainerIndex]);

			std::string _localTexturePath = "E:/Projects/Dynamik Engine/Dynamik/core assets/icons/Dynamik.jpg";
			myWindowManager.setWindowIcon(&myVulkanDataContainers[vulkanContainerIndex], { _localTexturePath });
		}

		// initialize the instance
		void vulkanRenderer::initInstance() {
			myInstanceManager.init(&myVulkanDataContainers[vulkanContainerIndex]);
		}

		// initialize the debugger
		void vulkanRenderer::initDebugger() {
#ifdef DMK_DEBUG
			myDebugger.initDebugger(&myVulkanDataContainers[vulkanContainerIndex]);
			myDebugger.setupDebugMessenger();
#endif
		}

		// initialize the window surface
		void vulkanRenderer::initWindowSurface() {
			myWindowManager.createWindowSurface(&myVulkanDataContainers[vulkanContainerIndex]);
		}

		// initialize the device
		void vulkanRenderer::initDevice() {
			myDeviceManager.init(&myVulkanDataContainers[vulkanContainerIndex]);
		}

		// initialize the swap chain
		void vulkanRenderer::initSwapChain() {
			mySwapChainManager.init(&myVulkanDataContainers[vulkanContainerIndex]);
			mySwapChainManager.initImageViews(&myVulkanDataContainers[vulkanContainerIndex]);
		}

		// initialize the descriptor set layout
		void vulkanRenderer::initDescriptorSetLayout(vulkanFormat* myVulkanFormat) {
			myUniformBufferManager.createDescriptorSetLayout(&myVulkanDataContainers[vulkanContainerIndex], myVulkanFormat);
		}

		void vulkanRenderer::initRenderPass() {
			myPipelineManager.initRenderPass(&myVulkanDataContainers[vulkanContainerIndex]);
		}

		// initialize the pipelines
		// Customizable rendering support
		void vulkanRenderer::initPipelines(vulkanFormat* myVulkanFormat) {
			// compile shaders
			if (compileShaders)
				for (int itr = 0; itr < myShaderPaths.size(); itr++)
					myShaderManager.compileShaders(myShaderPaths[itr], true);

			// load shaders
			ADGRVulkanShaderDataContainer shaderContainer = {};
			shaderContainer.shaderCodes.resize(4);

			// vertex shader
			if (myVulkanFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.vertexShaderPath != "NONE")
				shaderContainer.shaderCodes[0] = utils::readFile(myVulkanFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.vertexShaderPath);
			else
				shaderContainer.shaderCodes[0] = {};
			// tessellation shader
			if (myVulkanFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.tessellationShaderPath != "NONE")
				shaderContainer.shaderCodes[1] = utils::readFile(myVulkanFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.tessellationShaderPath);
			else
				shaderContainer.shaderCodes[1] = {};
			// geometry shader
			if (myVulkanFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.geometryShaderPath != "NONE")
				shaderContainer.shaderCodes[2] = utils::readFile(myVulkanFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.geometryShaderPath);
			else
				shaderContainer.shaderCodes[2] = {};
			// fragment shader
			if (myVulkanFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.fragmentShaderPath != "NONE")
				shaderContainer.shaderCodes[3] = utils::readFile(myVulkanFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.fragmentShaderPath);
			else
				shaderContainer.shaderCodes[3] = {};

			// init shaders
			myShaderManager.init(&myVulkanDataContainers[vulkanContainerIndex], &shaderContainer);

			// init pipeline
			DMKPipelineInitInfo initInfo;
			initInfo.layouts = { myVulkanFormat->myDescriptorSetLayout };
			initInfo.shaderDataContainer = shaderContainer;
			auto [_localPipeline, _localPipelineLayout] = myPipelineManager.init(&myVulkanDataContainers[vulkanContainerIndex], initInfo);

			// add pipelines and pipelineLayouts
			myVulkanFormat->myPipeline = _localPipeline;
			myVulkanFormat->myPipelineLayout = _localPipelineLayout;

			// delete shaders
			ADGRVulkanPipelineDataContainer containerData = {};
			containerData.shaderModules = shaderContainer.shaderModules;
			myShaderManager.deleteShaders(myVulkanDataContainers[vulkanContainerIndex], containerData);
		}

		// initialize the command pool
		void vulkanRenderer::initCommandPool() {
			myCommandBufferManager.initCommandPool(&myVulkanDataContainers[vulkanContainerIndex]);
		}

		// initialize the color buffer
		void vulkanRenderer::initColorBuffer() {
			myColorBufferManager.initResources(&myVulkanDataContainers[vulkanContainerIndex]);
		}

		// initialixe the depth buffer
		void vulkanRenderer::initDepthBuffer() {
			myDepthBufferManager.init(&myVulkanDataContainers[vulkanContainerIndex]);
		}

		// initialize the frame buffers
		void vulkanRenderer::initFrameBuffers() {
			myFrameBufferManager.init(&myVulkanDataContainers[vulkanContainerIndex]);
		}

		// other initializing functions
		void vulkanRenderer::otherInitFunctions() {
			//myUniformBufferManager.createDescriptorSetLayout(&myVulkanDataContainers[vulkanContainerIndex], &myTerrainDescriptorSetLayout);
			//myUniformBufferManager.createUniformBuffers(&myVulkanDataContainers[vulkanContainerIndex], &myTerrainUniformBuffers, &myTerrainUniformBufferMamories);
		}

		// initialize skyboxes and textures
		void vulkanRenderer::initSkyboxsAndTextures(vulkanFormat* myVulkanFormat) {
			std::vector<bool> invert = {
				false,
				false,
				false,
				false,
				false,
				true
			};

			// textures and skyboxes
			int texturePathSize = myVulkanFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.texturePaths.size();
			float mipLevels = myVulkanFormat->myRendererFormat->myInternalFormat->myMipLevel;
			myVulkanFormat->myTextureImages.resize(texturePathSize);
			myVulkanFormat->myTextureImageMemories.resize(texturePathSize);
			myVulkanFormat->myTextureImageSamplers.resize(texturePathSize);
			myVulkanFormat->myTextureImageViews.resize(texturePathSize);

			if (myVulkanFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_SKYBOX) {
				for (int itr = 0; itr < texturePathSize; itr++) {
					// skybox
					DMKInitCubemapInfo cubemapInfo = {};
					cubemapInfo.path = myVulkanFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.texturePaths[itr];
					cubemapInfo.textureImage = &myVulkanFormat->myTextureImages[itr];
					cubemapInfo.textureImageMemory = &myVulkanFormat->myTextureImageMemories[itr];
					cubemapInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
					cubemapInfo.mipLevels = mipLevels;
					cubemapInfo.imageView = &myVulkanFormat->myTextureImageViews[itr];
					cubemapInfo.imageSampler = &myVulkanFormat->myTextureImageSamplers[itr];
					cubemapInfo.flipImage = invert[itr];
					mySkyboxManager.loadCubemap(&myVulkanDataContainers[vulkanContainerIndex], cubemapInfo);
				}
			}
			else {
				// texture
				for (int itr = 0; itr < texturePathSize; itr++) {
					// texture creation - init
					DMKInitTextureInfo textureInfo;
					textureInfo.path = myVulkanFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.texturePaths[itr];
					textureInfo.textureImage = &myVulkanFormat->myTextureImages[itr];
					textureInfo.textureImageMemory = &myVulkanFormat->myTextureImageMemories[itr];
					textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
					textureInfo.mipLevels = mipLevels;
					myTextureManager.initTexture(&myVulkanDataContainers[vulkanContainerIndex], textureInfo);

					// texture - imageViews
					DMKInitTextureImageViewsInfo viewInfo;
					viewInfo.textureImage = myVulkanFormat->myTextureImages[itr];
					viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
					viewInfo.mipLevels = mipLevels;

					DMKCreateImageViewInfo cImgVewinfo;
					cImgVewinfo.device = myVulkanDataContainers[vulkanContainerIndex].device;
					cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
					cImgVewinfo.image = myVulkanFormat->myTextureImages[itr];
					cImgVewinfo.mipLevels = mipLevels;
					cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					cImgVewinfo.textureImageView = &myVulkanFormat->myTextureImageViews[itr];
					myTextureManager.initTextureImageViews(&myVulkanDataContainers[vulkanContainerIndex], viewInfo, cImgVewinfo);

					// texture - sampler
					DMKInitTextureSamplerInfo samplerInfo;
					samplerInfo.textureSampler = &myVulkanFormat->myTextureImageSamplers[itr];
					samplerInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					samplerInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					samplerInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					samplerInfo.magFilter = VK_FILTER_LINEAR;
					samplerInfo.minFilter = VK_FILTER_LINEAR;
					samplerInfo.mipLevel = mipLevels;
					myTextureManager.initTextureSampler(&myVulkanDataContainers[vulkanContainerIndex], samplerInfo);
				}
			}
		}

		// initialize vertex and index buffers
		void vulkanRenderer::initVertexAndIndexBuffers(vulkanFormat* myVulkanFormat) {
			myVertexBufferManager.createVertexBuffer(&myVulkanDataContainers[vulkanContainerIndex], myVulkanFormat);

			if (myVulkanFormat->myRendererFormat->myRenderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED)
				myIndexBufferManager.createIndexBuffer(&myVulkanDataContainers[vulkanContainerIndex], myVulkanFormat);
		}

		// initialize uniform buffers
		void vulkanRenderer::initUniformBuffers(vulkanFormat* myVulkanFormat) {
			myUniformBufferManager.createUniformBuffers(&myVulkanDataContainers[vulkanContainerIndex], myVulkanFormat);
		}

		// initialize descriptor pools and sets
		void vulkanRenderer::initDescriptorPoolsAndSets(vulkanFormat* myVulkanFormat) {
			myUniformBufferManager.initDescriptorPool(&myVulkanDataContainers[vulkanContainerIndex], myVulkanFormat);

			// init descriptor set
			myUniformBufferManager.initDescriptorSets(&myVulkanDataContainers[vulkanContainerIndex], myVulkanFormat);
		}

		// initialize command buffers
		void vulkanRenderer::initCommandBuffers(std::vector<vulkanFormat>* myVulkanFormats) {
			myCommandBufferManager.bindCommands(&myVulkanDataContainers[vulkanContainerIndex], myVulkanFormats);
		}

		// initialize semaphores and fences
		void vulkanRenderer::initSemaphoresAndFences() {
			initSyncObjects(myVulkanDataContainers[vulkanContainerIndex].device, &myImageAvailableSemaphores, &myRenderFinishedSemaphores, &myFencesInFlight);
		}

		// initialize individual objects
		void vulkanRenderer::initObjectBasedFunctions(std::vector<vulkanFormat>* myVulkanFormats) {
			for (uint32_t _itr = 0; _itr < myVulkanFormats->size(); _itr++) {
				vulkanFormat* _localVulkanFormat = &myVulkanFormats->at(_itr);
				if (_localVulkanFormat->isInitialized)
					continue;

				// first layout
				initDescriptorSetLayout(_localVulkanFormat);

				// init pipelines
				initPipelines(_localVulkanFormat);

				// textures and skyboxes
				initSkyboxsAndTextures(_localVulkanFormat);

				// load models
				initVertexAndIndexBuffers(_localVulkanFormat);

				// uniform buffer creation
				initUniformBuffers(_localVulkanFormat);

				// descriptor pool creation
				initDescriptorPoolsAndSets(_localVulkanFormat);

				_localVulkanFormat->isInitialized = true;
			}

			// command buffer initialization
			initCommandBuffers(myVulkanFormats);
		}

		/* STAGE BASED SHUTDOWN */
		// wait idle
		void vulkanRenderer::rendererWait() {
			vkDeviceWaitIdle(myVulkanDataContainers[vulkanContainerIndex].device);
		}
	}
}
#endif