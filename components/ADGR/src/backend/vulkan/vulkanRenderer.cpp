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

#include "Platform/Windows.h"
#include "keyCodes.h"

#include "core/functions/textureFunctions.h"
#include "core/controlHeader.h"

#include "debugger.h"
#include "CentralDataHub.h"

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
		void vulkanRenderer::init() {
			updateInfo.resize(uniformCount);
			// Global manager allocations
			initManagerFunctions();

			// init GameObjects
			initGameObjects();

			// initialize window
			initWindow();
			INC_PROGRESS;

			//glTexture _localTexture(_localTexturePath);
			//_localTexture.bind();

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

			// first layout
			initDescriptorSetLayout();
			INC_PROGRESS;

			// init pipelines
			initPipelines();
			for (int i = 0; i < 4; i++)
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

			// TODO: manually initialization
			initFrameBuffers();
			INC_PROGRESS;

			// textures and skyboxes
			initSkyboxsAndTextures();
			for (int i = 0; i < 4; i++)
				INC_PROGRESS;

			DMK_INFO("Number of cores: " + std::to_string(std::thread::hardware_concurrency()));
			DMK_INFO("Manager size: " + std::to_string(sizeof(myManager)));

			// load models
			initCubemapAndModels();

			// uniform buffer creation
			initUniformBuffers();
			INC_PROGRESS;

			// descriptor pool creation
			initDescriptorPoolsAndSets();
			INC_PROGRESS;

			// command buffer initialization
			initCommandBuffers();
			INC_PROGRESS;

			// initialize sync objects
			initSemaphoresAndFences();
			INC_PROGRESS;
		}

		// shutdown the renderer
		void vulkanRenderer::shutdown() {
			// idle
			rendererWait();

			// clear depth buffer
			myDepthBufferManager.clear(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			// clear color buffer
			myColorBufferManager.clear(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			// clear swapChain
			std::vector<VkPipeline> pipeline;
			std::vector<VkPipelineLayout> pipelineLayout;
			DMKSwapChainCleanUpInfo cleanInfo;
			cleanInfo.uniformBuffers = myManager.getFullResource<std::vector<VkBuffer>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER);
			cleanInfo.uniformBufferMemories = myManager.getFullResource<std::vector<VkDeviceMemory>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER_MEMORIES);
			cleanInfo.descriptorPools = myManager.getFullResource<VkDescriptorPool>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_POOL);

			for (auto object : *myManager.getFullResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER)) {
				pipeline.push_back(object.pipeline);
				pipelineLayout.push_back(object.pipelineLayout);
			}

			cleanInfo.pipelines = pipeline;
			cleanInfo.pipelineLayouts = pipelineLayout;
			mySwapChain.cleanUp(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), cleanInfo);

			// clear textures
			for (int itr = 0; itr < myManager.getFullResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER)->size(); itr++) {
				for (int i = 0; i < myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, itr)->textureImages.size(); i++) {
					DMKTextureDeleteInfo deleteTexInfo;
					deleteTexInfo.sampler = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, itr)->textureSamplers[i];
					deleteTexInfo.texture = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, itr)->textureImages[i];
					deleteTexInfo.imageView = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, itr)->textureImageViews[i];
					deleteTexInfo.textureImageMemory = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, itr)->textureImageMemories[i];
					myTextureManager.deleteTexture(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), deleteTexInfo);
				}

				// clear index buffer
				DMKindexBufferDeleteInfo deleteIndInfo;
				deleteIndInfo.buffer = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, itr)->indexBuffer;
				deleteIndInfo.bufferMemory = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, itr)->indexBufferMemory;
				myIndexBufferManager.deleteBuffer(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), deleteIndInfo);

				// clear vertex buffer
				for (int i = 0; i < myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, itr)->vertexBuffers.size(); i++) {
					DMKVertexBufferDeleteInfo deleteVertInfo;
					deleteVertInfo.buffer = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, itr)->vertexBuffers[i];
					deleteVertInfo.bufferMemory = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, itr)->vertexBufferMemories[i];
					myVertexBufferManager.deleteBuffer(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), deleteVertInfo);
				}
			}

			// destroy descriptorSetLayout
			vkDestroyDescriptorSetLayout(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device,
				myManager.getResource<VkDescriptorSetLayout>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET_LAYOUT, 0), nullptr);

			// delete frames in flight
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				vkDestroySemaphore(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device,
					myManager.getResource<VkSemaphore>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_IMAGE_AVAILABLE, i), nullptr);
				vkDestroySemaphore(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device,
					myManager.getResource<VkSemaphore>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_RENDER_FINISHED, i), nullptr);
				vkDestroyFence(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device,
					myManager.getResource<VkFence>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_FENCE_IN_FLIGHT, i), nullptr);
			}

			// delete the command pool
			vkDestroyCommandPool(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device,
				myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).commandBufferContainer.commandPool, nullptr);

			// delete device
			vkDestroyDevice(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device, nullptr);

			// stop the debugger
			myDebugger.deleteDebugger();

			// destroy surface
			vkDestroySurfaceKHR(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).instance,
				myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).surface, nullptr);

			// clear instance
			myInstance.clear(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			// final
			myWindow.clear(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			Debugger::benchmark::endProfiler();
		}

		// draw frame
		void vulkanRenderer::drawFrame() {
#ifdef DMK_DEBUG
			myFPSCal.getFPS();	// FPS calculator

 // ----------
#endif

			// wait for fences
			vkWaitForFences(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device, 1,
				myManager.getResourceAddr<VkFence>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_FENCE_IN_FLIGHT, currentFrame),
				VK_TRUE, std::numeric_limits<uint64_t>::max());

			// get image index
			uint32_t imageIndex;
			VkResult result = vkAcquireNextImageKHR(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device,
				myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).swapchainContainer.swapchain, std::numeric_limits<uint64_t>::max(),
				myManager.getResource<VkSemaphore>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_IMAGE_AVAILABLE, currentFrame), VK_NULL_HANDLE, &imageIndex);

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

			// uniform buffer object update
			for (int itr = 0; itr < uniformCount; itr++) {
				if (itr == 0) {
					updateInfo[itr].bufferMemory = myManager.getResource<std::vector<VkDeviceMemory>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER_MEMORIES, itr);
					updateInfo[itr].currentImage = imageIndex;
					updateInfo[itr].turn = { myEvent.turnEventL , myEvent.turnEventR };
					updateInfo[itr].move = { myEvent.moveEventU , myEvent.moveEventD };
					updateInfo[itr].upDown = { myEvent.rotEventD , myEvent.rotEventU };
					updateInfo[itr].rotation = { myEvent.rotEventL , myEvent.rotEventR };
					updateInfo[itr].cPos = { myCEvent.x, myCEvent.y };
					uniformBuffer.updateBuffer3D(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), &updateInfo[itr]);
				}
				else if(itr == 1) {
					updateInfo[itr].bufferMemory = myManager.getResource<std::vector<VkDeviceMemory>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER_MEMORIES, itr);
					updateInfo[itr].currentImage = imageIndex;
					updateInfo[itr].turn = { false , false };
					updateInfo[itr].move = { true , false };
					updateInfo[itr].upDown = { false , false };
					updateInfo[itr].rotation = { false , false };
					updateInfo[itr].cPos = { myCEvent.x, myCEvent.y };
					updateInfo[itr].movementBias = 0.05;
					updateInfo[itr].verticalLock = true;
					updateInfo[itr].up = 0.75f;
					uniformBuffer.updateBuffer3D(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), &updateInfo[itr]);
				}
				else {
					updateInfo[itr].bufferMemory = myManager.getResource<std::vector<VkDeviceMemory>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER_MEMORIES, itr);
					updateInfo[itr].currentImage = imageIndex;
					updateInfo[itr].turn = { false , false };
					updateInfo[itr].move = { true , false };
					updateInfo[itr].upDown = { true , false };
					updateInfo[itr].rotation = { false , false };
					updateInfo[itr].cPos = { myCEvent.x, myCEvent.y };
					updateInfo[itr].verticalLock = true;
					updateInfo[itr].movementBias = 0.0625;
					updateInfo[itr].up = -1.25f;
					uniformBuffer.updateBuffer3D(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), &updateInfo[itr]);
				}
			}
			// submit info
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			// wait for semaphores
			VkSemaphore waitSemaphores[] = { myManager.getResource<VkSemaphore>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_IMAGE_AVAILABLE, currentFrame) };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).commandBufferContainer.buffers[imageIndex];

			// signal semaphores
			VkSemaphore signalSemaphores[] = { myManager.getResource<VkSemaphore>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_RENDER_FINISHED, currentFrame) };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			// reset fences
			vkResetFences(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device, 1,
				myManager.getResourceAddr<VkFence>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_FENCE_IN_FLIGHT, currentFrame));

			// submit command queue
			if (vkQueueSubmit(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).graphicsQueue, 1, &submitInfo,
				myManager.getResource<VkFence>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_FENCE_IN_FLIGHT, currentFrame)) != VK_SUCCESS)
				DMK_CORE_FATAL("failed to submit draw command buffer!");

			// present queue info
			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			// swapchain info
			VkSwapchainKHR swapChains[] = { myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).swapchainContainer.swapchain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &imageIndex;

			// submit queue
			result = vkQueuePresentKHR(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).presentQueue, &presentInfo);

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

		// recreate the swapchain
		void vulkanRenderer::recreateSwapChain() {
			DMK_DEBUGGER_PROFILER_TIMER_START(timer);

			// window resize event
			myWindow.onWindowResizeEvent();

			// idle
			vkDeviceWaitIdle(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device);

			// clear swapChain
			std::vector<VkPipeline> pipeline;
			std::vector<VkPipelineLayout> pipelineLayout;
			DMKSwapChainCleanUpInfo cleanInfo;
			cleanInfo.uniformBuffers = myManager.getFullResource<std::vector<VkBuffer>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER);
			cleanInfo.uniformBufferMemories = myManager.getFullResource<std::vector<VkDeviceMemory>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER_MEMORIES);
			cleanInfo.descriptorPools = myManager.getFullResource<VkDescriptorPool>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_POOL);

			for (auto object : *myManager.getFullResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER)) {
				pipeline.push_back(object.pipeline);
				pipelineLayout.push_back(object.pipelineLayout);
			}

			cleanInfo.pipelines = pipeline;
			cleanInfo.pipelineLayouts = pipelineLayout;
			mySwapChain.cleanUp(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), cleanInfo);

			// clean frame buffer
			myFrameBufferManager.clear(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			// init swapchain
			mySwapChain.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			// init swapchain image views
			mySwapChain.initImageViews(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			// init pipeline
			for (int i = 0; i < myManager.getFullResourceAddr<std::vector<ADGRObjectRenderData>>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER)->size(); i++) {
				// compile shaders
				if (compileShaders)
					for (int itr = 0; itr < myManager.getFullResourceAddr<std::vector<std::string>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_PATH)->size(); itr++)
						myShaderManager.compileShaders(*myManager.getResourceAddr<std::string>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_PATH, itr), true);

				// load shaders
				ADGRVulkanShaderDataContainer shaderContainer = {};
				shaderContainer.shaderCodes.resize(4);

				// vertex shader
				if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.vertexShaderPath != "NONE")
					shaderContainer.shaderCodes[0] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.vertexShaderPath);
				else
					shaderContainer.shaderCodes[0] = {};
				// tessellation shader
				if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.tessellationShaderPath != "NONE")
					shaderContainer.shaderCodes[1] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.tessellationShaderPath);
				else
					shaderContainer.shaderCodes[1] = {};
				// geometry shader
				//if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i).my != "NONE")
				if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.geometryShaderPath != "NONE")
					shaderContainer.shaderCodes[2] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.geometryShaderPath);
				else
					shaderContainer.shaderCodes[2] = {};
				// fragment shader
				if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.fragmentShaderPath != "NONE")
					shaderContainer.shaderCodes[3] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.fragmentShaderPath);
				else
					shaderContainer.shaderCodes[3] = {};

				// init shaders
				myShaderManager.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), &shaderContainer);

				// init pipeline
				DMKPipelineInitInfo initInfo;
				initInfo.layouts = { myManager.getResource<VkDescriptorSetLayout>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET_LAYOUT, 0) };
				initInfo.shaderDataContainer = shaderContainer;
				auto [l_pipeline, l_pipelineLayout] = myPipeline.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), initInfo);

				myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->pipeline = l_pipeline;
				myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->pipelineLayout = l_pipelineLayout;

				// delete shaders
				ADGRVulkanPipelineDataContainer containerData = {};
				containerData.shaderModules = shaderContainer.shaderModules;
				myShaderManager.deleteShaders(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), containerData);
			}

			// TODO: manually initialization
			myColorBufferManager.initResources(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			// TODO: manually initialization
			myDepthBufferManager.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			// TODO: manually initialization
			myFrameBufferManager.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			// uniform buffer creation
			for (int itr = 0; itr < uniformCount; itr++) {
				DMKVulkanRendererCreateUniformBufferInfo uBuffInfo;
				uBuffInfo.buffer = myManager.getResourceAddr<std::vector<VkBuffer>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER, itr);
				uBuffInfo.bufferMemory = myManager.getResourceAddr<std::vector<VkDeviceMemory>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER_MEMORIES, itr);
				createUniformBuffer(uBuffInfo);
			}

			// init descriptor pool
			for (int itr = 0; itr < myManager.getFullResourceAddr<std::vector<ADGRObjectRenderData>>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER)->size(); itr++)
				uniformBuffer.initDescriptorPool(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex),
					myManager.getResourceAddr<VkDescriptorPool>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_POOL, itr));

			// init descriptor set
			for (int i = 0; i < myManager.getFullResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER)->size(); i++) {
				for (int itr = 0; itr < myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->texturePaths.size(); itr++) {
					DMKVulkanRendereCreateDescriptorSetsInfo descInitInfo;
					descInitInfo.uniformBuffers = myManager.getResourceAddr<std::vector<VkBuffer>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER, itr);
					descInitInfo.textureImageView = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImageViews[itr];
					descInitInfo.textureSampler = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureSamplers[itr];
					descInitInfo.descriptorSets = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->descriptorSets[itr];
					descInitInfo.layout = myManager.getResourceAddr<VkDescriptorSetLayout>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET_LAYOUT, 0);
					descInitInfo.descriptorPool = myManager.getResource<VkDescriptorPool>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_POOL, itr);
					descInitInfo.bindIndexes = { 0, 1 };
					createDescriptorSets(descInitInfo);
				}
			}

			// init command buffers
			DMKBindCommandBufferInfo commandInfo;
			commandInfo.objectBindDatas = myManager.getFullResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER);
			myCommandBufferManager.bindCommands(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), commandInfo);
			INC_PROGRESS;
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

		// set game objects locally
		void vulkanRenderer::setGameObjects(std::vector<GameObject>& gameObjects) {
			myManager.setResource<std::vector<GameObject>>(gameObjects, DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT);
		}

		// set model paths
		void vulkanRenderer::setModelPaths(std::vector<std::string>& object, std::vector<std::vector<std::string>>& texture) {
			myManager.setResource<std::vector<std::string>>(object, DMK_CDH_MANAGER_RESOURCE_TYPE_PATH_MODEL);
			myManager.setResource<std::vector<std::string>>(object, DMK_CDH_MANAGER_RESOURCE_TYPE_PATH_TEXTURE);
		}

		// set shader paths
		void vulkanRenderer::setShaderPaths(std::string& vertex, std::string& fragment) {
			std::vector<std::string> _localPathContainer = {
				vertex,
				"",
				"",
				fragment
			};
			myManager.setResource<std::vector<std::string>>(_localPathContainer, DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_PATH);
		}

		// initialize the pipeline (abstraction)
		void vulkanRenderer::initPipeline(DMK_ADGR_CreatePipelineInfo info) {
			// compile shaders
			if (compileShaders)
				for (int itr = 0; itr < myManager.getFullResourceAddr<std::vector<std::string>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_PATH)->size(); itr++)
					myShaderManager.compileShaders(*myManager.getResourceAddr<std::string>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_PATH, itr), true);

			// load shaders
			ADGRVulkanShaderDataContainer shaderContainer = {};
			shaderContainer.shaderCodes.resize(4);

			// vertex shader
			if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.vertexShaderPath != "NONE")
				shaderContainer.shaderCodes[0] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.vertexShaderPath);
			else
				shaderContainer.shaderCodes[0] = {};
			// tessellation shader
			if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.tessellationShaderPath != "NONE")
				shaderContainer.shaderCodes[1] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.tessellationShaderPath);
			else
				shaderContainer.shaderCodes[1] = {};
			// geometry shader
			if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.geometryShaderPath != "NONE")
				shaderContainer.shaderCodes[2] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.geometryShaderPath);
			else
				shaderContainer.shaderCodes[2] = {};
			// fragment shader
			if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.fragmentShaderPath != "NONE")
				shaderContainer.shaderCodes[3] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.fragmentShaderPath);
			else
				shaderContainer.shaderCodes[3] = {};

			// init shaders
			myShaderManager.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), &shaderContainer);

			// init pipeline
			DMKPipelineInitInfo initInfo;
			initInfo.layouts = { myManager.getResource<VkDescriptorSetLayout>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET_LAYOUT, 0) };
			initInfo.shaderDataContainer = shaderContainer;
			auto [pipeline, pipelineLayout] = myPipeline.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), initInfo);

			// delete shaders
			myShaderManager.deleteShaders(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex),
				myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).pipelineContainers[0]);
		}

		// create vertex buffer
		void vulkanRenderer::createVertexBuffer(DMKVulkanRendererCreateVertexBufferInfo info) {
			DMKVertexBufferCreateInfo vertexBufferInfo;
			vertexBufferInfo.buffer = info.buffer;
			vertexBufferInfo.buffereMemory = info.bufferMemory;
			vertexBufferInfo.vertices = info.vertexBufferObject;
			myVertexBufferManager.createVertexBuffer(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), vertexBufferInfo);
		}

		// create index buffer
		void vulkanRenderer::createIndexBuffer(DMKVulkanRendererCreateIndexBufferInfo info) {
			DMKIndexBufferCreateInfo indexBufferInfo;
			indexBufferInfo.buffer = info.buffer;
			indexBufferInfo.buffereMemory = info.bufferMemory;
			indexBufferInfo.indices = info.indexBufferObject;
			myIndexBufferManager.createIndexBuffer(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), indexBufferInfo);
		}

		// create uniform buffer
		void vulkanRenderer::createUniformBuffer(DMKVulkanRendererCreateUniformBufferInfo info) {
			DMKUniformBufferCreateInfo unibuffinfo;
			unibuffinfo.buffers = info.buffer;
			unibuffinfo.bufferMemories = info.bufferMemory;
			uniformBuffer.createUniformBuffers(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), unibuffinfo);
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
			uniformBuffer.initDescriptorSets(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), descripInfo);
		}

		// load another shader
		void vulkanRenderer::includeShader() {
			// load shaders
			ADGRVulkanShaderDataContainer shaderContainer = {};
			shaderContainer.shaderCodes.resize(4);

			// vertex shader
			if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.vertexShaderPath != "NONE")
				shaderContainer.shaderCodes[0] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.vertexShaderPath);
			else
				shaderContainer.shaderCodes[0] = {};
			// tessellation shader
			if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.tessellationShaderPath != "NONE")
				shaderContainer.shaderCodes[1] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.tessellationShaderPath);
			else
				shaderContainer.shaderCodes[1] = {};
			// geometry shader
			if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.geometryShaderPath != "NONE")
				shaderContainer.shaderCodes[2] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.geometryShaderPath);
			else
				shaderContainer.shaderCodes[2] = {};
			// fragment shader
			if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.fragmentShaderPath != "NONE")
				shaderContainer.shaderCodes[3] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->myProperties.renderableObjectProperties.fragmentShaderPath);
			else
				shaderContainer.shaderCodes[3] = {};

			// init shaders
			myShaderManager.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), &shaderContainer);
			INC_PROGRESS;

			// init pipeline
			DMKPipelineInitInfo initInfo;
			initInfo.layouts = { myManager.getResource<VkDescriptorSetLayout>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET_LAYOUT, 0) };
			initInfo.shaderDataContainer = shaderContainer;
			myPipeline.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), initInfo);
			INC_PROGRESS;

			// delete shaders
			myShaderManager.deleteShaders(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex),
				myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).pipelineContainers[0]);
			INC_PROGRESS;
		}

		// third thread
		void vulkanRenderer::thread_third(DMKVulkanRendererLoadObjectInfo createInfo) {
			DMK_DEBUGGER_PROFILER_TIMER_START(timer);

			DMKModelLoadInfo info;
			info.path = createInfo.objectDataContainer.path;
			info.vertices = createInfo.objectDataContainer.vertexBufferObject;
			info.indices = createInfo.objectDataContainer.indexBufferObject;
			info.vertexOffset = createInfo.objectDataContainer.offsets;

			loadModel(info);
		}

		// initialize models
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
				vertBuffInfo.buffer = &data[i].vertexBuffers->at(0);
				vertBuffInfo.bufferMemory = &data[i].vertexBufferMemories->at(0);
				createVertexBuffer(vertBuffInfo);

				*data[i].vertexCount = (uint32_t)data[i].vertexBufferObject->size();

				// create indexBuffer
				DMKVulkanRendererCreateIndexBufferInfo idxBuffInfo;
				idxBuffInfo.indexBufferObject = *data[i].indexBufferObject;
				idxBuffInfo.buffer = data[i].indexBuffer;
				idxBuffInfo.bufferMemory = data[i].indexBufferMemory;
				createIndexBuffer(idxBuffInfo);

				*data[i].indexCount = (uint32_t)data[i].indexBufferObject->size();

				if (enableVertexAndIndexClear) {
					data[i].vertexBufferObject->clear();
					data[i].indexBufferObject->clear();
				}
			}
			INC_PROGRESS;
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
				vertBuffInfo.vertexBufferObject = _localVertexBufferObject;
				vertBuffInfo.buffer = &data->vertexBuffers->at(itr);
				vertBuffInfo.bufferMemory = &data->vertexBufferMemories->at(itr);
				createVertexBuffer(vertBuffInfo);

				*data->vertexCount = (uint32_t)_localVertexBufferObject.size();
			}
		}

		/* STAGE BASED INITIALIZATION */
		// initialize the manager variables locally
		void vulkanRenderer::initManagerFunctions() {
			myManager.allocate<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, (vulkanContainerIndex + 1));
			myManager.allocate<std::vector<VkBuffer>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER, uniformCount);
			myManager.allocate<std::vector<VkDeviceMemory>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER_MEMORIES, uniformCount);
			myManager.allocate<VkDescriptorSetLayout>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET_LAYOUT, 1);
			myManager.allocate<VkSemaphore>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_IMAGE_AVAILABLE, MAX_FRAMES_IN_FLIGHT);
			myManager.allocate<VkSemaphore>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_RENDER_FINISHED, MAX_FRAMES_IN_FLIGHT);
			myManager.allocate<VkFence>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_FENCE_IN_FLIGHT, MAX_FRAMES_IN_FLIGHT);
		}

		// initialize game objects locally
		void vulkanRenderer::initGameObjects() {
			int _localGameObjectsSize = myManager.getFullResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT)->size();
			myManager.allocate<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, _localGameObjectsSize);

			for (int i = 0; i < _localGameObjectsSize; i++) {
				myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->paths =
					myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.objectPath;
				myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->texturePaths =
					myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.texturePaths;
				myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->location =
					myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.transformProperties.location;
			}
		}

		// initialize the window
		void vulkanRenderer::initWindow() {
			myWindow.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			std::string _localTexturePath = "E:/Projects/Dynamik Engine/Dynamik/core assets/icons/icon1.jpg";
			myWindow.setWindowIcon(_localTexturePath);
		}

		// initialize the instance
		void vulkanRenderer::initInstance() {
			myInstance.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));
		}

		// initialize the debugger
		void vulkanRenderer::initDebugger() {
#ifdef DMK_DEBUG
			myDebugger.initDebugger(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));
			myDebugger.setupDebugMessenger();
#endif
		}

		// initialize the window surface
		void vulkanRenderer::initWindowSurface() {
			myWindow.createWindowSurface(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));
		}

		// initialize the device
		void vulkanRenderer::initDevice() {
			myDevice.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));
		}

		// initialize the swap chain
		void vulkanRenderer::initSwapChain() {
			mySwapChain.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));
			mySwapChain.initImageViews(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));
		}

		// initialize the descriptor set layout
		void vulkanRenderer::initDescriptorSetLayout() {
			DMKUniformBufferCreateDescriptorSetLayoutInfo layoutInfo;
			layoutInfo.layout = myManager.getResourceAddr<VkDescriptorSetLayout>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET_LAYOUT, 0);
			layoutInfo.bindIndex = { 0, 1 };
			uniformBuffer.createDescriptorSetLayout(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), layoutInfo);
		}

		// initialize the pipelines
		void vulkanRenderer::initPipelines() {
			for (int i = 0; i < myManager.getFullResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER)->size(); i++) {
				// compile shaders
				if (compileShaders)
					for (int itr = 0; itr < myManager.getFullResourceAddr<std::vector<std::string>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_PATH)->size(); itr++)
						myShaderManager.compileShaders(*myManager.getResourceAddr<std::string>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_PATH, itr), true);

				// load shaders
				ADGRVulkanShaderDataContainer shaderContainer = {};
				shaderContainer.shaderCodes.resize(4);

				// vertex shader
				if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.vertexShaderPath != "NONE")
					shaderContainer.shaderCodes[0] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.vertexShaderPath);
				else
					shaderContainer.shaderCodes[0] = {};
				// tessellation shader
				if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.tessellationShaderPath != "NONE")
					shaderContainer.shaderCodes[1] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.tessellationShaderPath);
				else
					shaderContainer.shaderCodes[1] = {};
				// geometry shader
				if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.geometryShaderPath != "NONE")
					shaderContainer.shaderCodes[2] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.geometryShaderPath);
				else
					shaderContainer.shaderCodes[2] = {};
				// fragment shader
				if (myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.fragmentShaderPath != "NONE")
					shaderContainer.shaderCodes[3] = utils::readFile(myManager.getResourceAddr<GameObject>(DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT, i)->myProperties.renderableObjectProperties.fragmentShaderPath);
				else
					shaderContainer.shaderCodes[3] = {};

				// init shaders
				myShaderManager.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), &shaderContainer);

				// init pipeline
				DMKPipelineInitInfo initInfo;
				initInfo.layouts = { myManager.getResource<VkDescriptorSetLayout>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET_LAYOUT, 0) };
				initInfo.shaderDataContainer = shaderContainer;
				auto [l_pipeline, l_pipelineLayout] = myPipeline.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), initInfo);

				myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->pipeline = l_pipeline;
				myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->pipelineLayout = l_pipelineLayout;

				// delete shaders
				ADGRVulkanPipelineDataContainer containerData = {};
				containerData.shaderModules = shaderContainer.shaderModules;
				myShaderManager.deleteShaders(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), containerData);
			}
		}

		// initialize the command pool
		void vulkanRenderer::initCommandPool() {
			myCommandBufferManager.initCommandPool(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));
		}

		// initialize the color buffer
		void vulkanRenderer::initColorBuffer() {
			myColorBufferManager.initResources(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));
		}

		// initialixe the depth buffer
		void vulkanRenderer::initDepthBuffer() {
			myDepthBufferManager.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));
		}

		// initialize the frame buffers
		void vulkanRenderer::initFrameBuffers() {
			myFrameBufferManager.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));
		}

		// initialize skyboxes and textures
		void vulkanRenderer::initSkyboxsAndTextures() {
			std::vector<bool> invert = {
				false,
				false,
				false,
				false,
				false,
				true
			};

			// textures and skyboxes
			for (int i = 0; i < myManager.getFullResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER)->size(); i++) {
				int texturePathSize = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->texturePaths.size();
				myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImages.resize(texturePathSize);
				myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImageMemories.resize(texturePathSize);
				myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureSamplers.resize(texturePathSize);
				myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImageViews.resize(texturePathSize);

				if (myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->objectType == DMKObjectType::DMK_OBJECT_TYPE_SKYBOX) {
					for (int itr = 0; itr < texturePathSize; itr++) {
						// skybox
						DMKInitCubemapInfo cubemapInfo = {};
						cubemapInfo.path = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->texturePaths[itr];
						cubemapInfo.textureImage = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImages[itr];
						cubemapInfo.textureImageMemory = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImageMemories[itr];
						cubemapInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
						cubemapInfo.mipLevels = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->mipLevel;
						cubemapInfo.imageView = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImageViews[itr];
						cubemapInfo.imageSampler = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureSamplers[itr];
						cubemapInfo.flipImage = invert[itr];
						mySkyboxManager.loadCubemap(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), cubemapInfo);
					}
				}
				else {
					// texture
					for (int itr = 0; itr < texturePathSize; itr++) {
						// texture creation - init
						DMKInitTextureInfo textureInfo;
						textureInfo.path = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->texturePaths[itr];
						textureInfo.textureImage = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImages[itr];
						textureInfo.textureImageMemory = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImageMemories[itr];
						textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
						textureInfo.mipLevels = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->mipLevel;
						myTextureManager.initTexture(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), textureInfo);

						// texture - imageViews
						DMKInitTextureImageViewsInfo viewInfo;
						viewInfo.textureImage = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImages[itr];
						viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
						viewInfo.mipLevels = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->mipLevel;

						DMKCreateImageViewInfo cImgVewinfo;
						cImgVewinfo.device = myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device;
						cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
						cImgVewinfo.image = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImages[itr];
						cImgVewinfo.mipLevels = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->mipLevel;
						cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
						cImgVewinfo.textureImageView = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImageViews[itr];
						myTextureManager.initTextureImageViews(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), viewInfo, cImgVewinfo);

						// texture - sampler
						DMKInitTextureSamplerInfo samplerInfo;
						samplerInfo.textureSampler = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureSamplers[itr];
						samplerInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
						samplerInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
						samplerInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
						samplerInfo.magFilter = VK_FILTER_LINEAR;
						samplerInfo.minFilter = VK_FILTER_LINEAR;
						samplerInfo.mipLevel = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->mipLevel;
						myTextureManager.initTextureSampler(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), samplerInfo);
					}
				}
			}
		}

		// initialize Skyboxes and Models
		void vulkanRenderer::initCubemapAndModels() {
			for (int i = 0; i < myManager.getFullResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER)->size(); i++) {
				if (myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->objectType == DMKObjectType::DMK_OBJECT_TYPE_SKYBOX) {
					myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->renderingType = DMK_ADGR_VULKAN_RENDERER_SKYBOX_VERTEX;

					myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->vertexBuffers.resize(6);
					myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->vertexBufferMemories.resize(6);

					DMKObjectData data = {};
					data.path = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->paths[0];
					data.vertexBufferObject = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->vertexBufferObject;
					data.vertexBuffers = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->vertexBuffers;
					data.vertexBufferMemories = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->vertexBufferMemories;
					data.indexBufferObject = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->indexBufferObject;
					data.indexBuffer = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->indexBuffer;
					data.indexBufferMemory = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->indexBufferMemory;
					data.vertexCount = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->vertexCount;
					data.indexCount = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->indexCount;
					data.offsets = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->location;

					initCubemap(&data);
				}
				else {
					myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->vertexBuffers.resize(1);
					myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->vertexBufferMemories.resize(1);

					DMKObjectData data = {};
					data.path = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->paths[0];
					data.vertexBufferObject = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->vertexBufferObject;
					data.vertexBuffers = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->vertexBuffers;
					data.vertexBufferMemories = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->vertexBufferMemories;
					data.indexBufferObject = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->indexBufferObject;
					data.indexBuffer = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->indexBuffer;
					data.indexBufferMemory = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->indexBufferMemory;
					data.vertexCount = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->vertexCount;
					data.indexCount = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->indexCount;
					data.offsets = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->location;

					myManager.addResource<DMKObjectData>(data, DMK_CDH_MANAGER_RESOURCE_TYPE_OBJECT_DATA_CONTAINER);
				}
			}

			initModels(myManager.getFullResource<DMKObjectData>(DMK_CDH_MANAGER_RESOURCE_TYPE_OBJECT_DATA_CONTAINER));
		}

		// initialize uniform buffers
		void vulkanRenderer::initUniformBuffers() {
			for (int itr = 0; itr < uniformCount; itr++) {
				DMKVulkanRendererCreateUniformBufferInfo uBuffInfo;
				uBuffInfo.buffer = myManager.getResourceAddr<std::vector<VkBuffer>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER, itr);
				uBuffInfo.bufferMemory = myManager.getResourceAddr<std::vector<VkDeviceMemory>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER_MEMORIES, itr);
				createUniformBuffer(uBuffInfo);
			}
		}

		// initialize descriptor pools and sets
		void vulkanRenderer::initDescriptorPoolsAndSets() {
			for (int i = 0; i < myManager.getFullResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER)->size(); i++) {
				int textureImagesSize = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImages.size();
				myManager.allocate<VkDescriptorPool>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_POOL, textureImagesSize);	// wrong method
				myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->descriptorSets.resize(textureImagesSize); // wrong method

				for (int itr = 0; itr < textureImagesSize; itr++) {
					// init descriptor pool
					uniformBuffer.initDescriptorPool(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex),
						myManager.getResourceAddr<VkDescriptorPool>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_POOL, itr));

					// init descriptor set
					DMKVulkanRendereCreateDescriptorSetsInfo descInitInfo;
					descInitInfo.uniformBuffers = myManager.getResourceAddr<std::vector<VkBuffer>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER, i);
					descInitInfo.textureImageView = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureImageViews[itr];
					descInitInfo.textureSampler = myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->textureSamplers[itr];
					descInitInfo.descriptorSets = &myManager.getResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER, i)->descriptorSets[itr];
					descInitInfo.layout = myManager.getResourceAddr<VkDescriptorSetLayout>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET_LAYOUT, 0);
					descInitInfo.descriptorPool = myManager.getResource<VkDescriptorPool>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_POOL, itr);
					descInitInfo.bindIndexes = { 0, 1 };
					createDescriptorSets(descInitInfo);
				}
			}
		}

		// initialize command buffers
		void vulkanRenderer::initCommandBuffers() {
			DMKBindCommandBufferInfo commandInfo;
			commandInfo.objectBindDatas = myManager.getFullResourceAddr<ADGRObjectRenderData>(DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER);
			myCommandBufferManager.bindCommands(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), commandInfo);
		}

		// initialize semaphores and fences
		void vulkanRenderer::initSemaphoresAndFences() {
			initSyncObjects(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device,
				myManager.getFullResourceAddr<VkSemaphore>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_IMAGE_AVAILABLE),
				myManager.getFullResourceAddr<VkSemaphore>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_RENDER_FINISHED),
				myManager.getFullResourceAddr<VkFence>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_FENCE_IN_FLIGHT));
		}

		/* STAGE BASED SHUTDOWN */
		// wait idle
		void vulkanRenderer::rendererWait() {
			vkDeviceWaitIdle(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex)->device);
		}
	}
}