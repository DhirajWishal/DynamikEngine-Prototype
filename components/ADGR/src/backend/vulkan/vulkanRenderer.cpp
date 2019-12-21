/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 vulkanRenderer.cpp file

 Latest build/ version has a known bug that is yet to be fixed.
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
		void vulkanRenderer::init() {	// 19 inc
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

			// first layout
			initDescriptorSetLayout();
			INC_PROGRESS;

			// init pipelines
			initPipelines();
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
			INC_PROGRESS;

			// load models
			initVertexAndIndexBuffers();
			INC_PROGRESS;

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
			// idle vulkanFormat*
			rendererWait();

			// clear depth buffer
			myDepthBufferManager.clear(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			// clear color buffer
			myColorBufferManager.clear(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			for (int itr = 0; itr < vulkanFormatObjectsCount; itr++) {
				// clear swapChain, Pipeline, Uniform buffers and descriptorPool
				DMKSwapChainCleanUpInfo cleanInfo = {};
				cleanInfo.uniformBuffers = { myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myUniformBuffers };
				cleanInfo.uniformBufferMemories = { myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myUniformBufferMemories };
				cleanInfo.descriptorPools = myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myDescriptorPools;
				cleanInfo.pipelines = { myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myPipeline };
				cleanInfo.pipelineLayouts = { myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myPipelineLayout };
				mySwapChain.cleanUp(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), cleanInfo);

				// clear textures
				for (int i = 0; i < myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myTextureImages.size(); i++) {
					DMKTextureDeleteInfo deleteTexInfo = {};
					deleteTexInfo.texture = myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myTextureImages[i];
					deleteTexInfo.textureImageMemory = myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myTextureImageMemories[i];
					deleteTexInfo.sampler = myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myTextureImageSamplers[i];
					deleteTexInfo.imageView = myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myTextureImageViews[i];
					myTextureManager.deleteTexture(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), deleteTexInfo);
				}

				// clear index buffer
				for (int i = 0; i < myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myIndexBuffers.size(); i++) {
					DMKindexBufferDeleteInfo deleteIndInfo = {};
					deleteIndInfo.buffer = myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myIndexBuffers[i];
					deleteIndInfo.bufferMemory = myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myIndexBufferMemories[i];
					myIndexBufferManager.deleteBuffer(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), deleteIndInfo);
				}

				// clear vertex buffer
				for (int i = 0; i < myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myVertexBuffers.size(); i++) {
					DMKVertexBufferDeleteInfo deleteVertInfo = {};
					deleteVertInfo.buffer = myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myVertexBuffers[i];
					deleteVertInfo.bufferMemory = myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myVertexBufferMemories[i];
					myVertexBufferManager.deleteBuffer(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), deleteVertInfo);
				}

				// destroy descriptorSetLayout
				vkDestroyDescriptorSetLayout(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device,
					myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myDescriptorSetLayout, nullptr);
			}

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
			uint32_t imageIndex = NULL;
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
			for (int itr = 0; itr < vulkanFormatObjectsCount; itr++)
				// uniform buffer object update
				uniformBuffer.updateBuffer3D(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex),
					myManager.getEventContainer(), myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr), imageIndex);

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
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || myWindow.isFrameBufferResized()) {
				myWindow.frameBufferResizedUpdate(false);
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
			myWindow.onWindowResizeEvent();

			// idle
			vkDeviceWaitIdle(myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device);

			// clear swapChain, Pipeline, Uniform buffers and descriptorPool
			for (int itr = 0; itr < vulkanFormatObjectsCount; itr++) {
				DMKSwapChainCleanUpInfo cleanInfo = {};
				cleanInfo.uniformBuffers = { myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myUniformBuffers };
				cleanInfo.uniformBufferMemories = { myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myUniformBufferMemories };
				cleanInfo.descriptorPools = myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myDescriptorPools;
				cleanInfo.pipelines = { myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myPipeline };
				cleanInfo.pipelineLayouts = { myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myPipelineLayout };
				mySwapChain.cleanUp(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), cleanInfo);
			}

			// clean frame buffer
			myFrameBufferManager.clear(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex));

			// init swapchain
			initSwapChain();

			// init pipeline
			initPipelines();

			// TODO: manually initialization
			initColorBuffer();

			// TODO: manually initialization
			initDepthBuffer();

			// TODO: manually initialization
			initFrameBuffers();

			// uniform buffer creation
			initUniformBuffers();

			// init descriptor pool
			initDescriptorPoolsAndSets();

			// init command buffers
			initCommandBuffers();
		}

		// events
		std::deque<DMKEventContainer>* vulkanRenderer::events() {
			myWindow.pollEvents();
			myManager.setEventContainer(myWindow.getEventContainer());

			return myManager.getEventContainerAddr();
		}

		void vulkanRenderer::setRendererFormats(std::vector<RendererFormat>& rendererFormats) {
			vulkanFormatObjectsCount = rendererFormats.size();
			std::vector<vulkanFormat> _localFormats;

			for (int i = 0; i < vulkanFormatObjectsCount; i++)
				_localFormats.push_back(vulkanFormat(&rendererFormats[i]));

			myManager.setResource<std::vector<vulkanFormat>>(_localFormats, DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT);
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
			vertexBufferInfo.bufferMemory = info.bufferMemory;
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
				createVertexBuffer(vertBuffInfo);

				*data[i].vertexCount = (uint32_t)data[i].vertexBufferObject->size();

				// create indexBuffer
				DMKVulkanRendererCreateIndexBufferInfo idxBuffInfo;
				idxBuffInfo.indexBufferObject = data[i].indexBufferObject;
				idxBuffInfo.buffer = data[i].indexBuffer;
				idxBuffInfo.bufferMemory = data[i].indexBufferMemory;
				createIndexBuffer(idxBuffInfo);

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
				createVertexBuffer(vertBuffInfo);

				*data->vertexCount = (uint32_t)_localVertexBufferObject.size();
			}
		}

		/* STAGE BASED INITIALIZATION */
		// initialize the manager variables locally
		void vulkanRenderer::initManagerFunctions() {
			myManager.allocate<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, (vulkanContainerIndex + 1));
			myManager.allocate<VkDescriptorSetLayout>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET_LAYOUT, 1);
			myManager.allocate<VkSemaphore>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_IMAGE_AVAILABLE, MAX_FRAMES_IN_FLIGHT);
			myManager.allocate<VkSemaphore>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_RENDER_FINISHED, MAX_FRAMES_IN_FLIGHT);
			myManager.allocate<VkFence>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_FENCE_IN_FLIGHT, MAX_FRAMES_IN_FLIGHT);
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
			for (int i = 0; i < vulkanFormatObjectsCount; i++) {
				DMKUniformBufferCreateDescriptorSetLayoutInfo layoutInfo;
				layoutInfo.layout = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myDescriptorSetLayout;
				layoutInfo.bindIndex = { 0, 1 };
				uniformBuffer.createDescriptorSetLayout(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), layoutInfo);
			}
		}

		// initialize the pipelines
		// Customizable rendering support
		void vulkanRenderer::initPipelines() {
			for (int i = 0; i < vulkanFormatObjectsCount; i++) {
				// compile shaders
				if (compileShaders)
					for (int itr = 0; itr < myManager.getFullResourceAddr<std::vector<std::string>>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_PATH)->size(); itr++)
						myShaderManager.compileShaders(*myManager.getResourceAddr<std::string>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_PATH, itr), true);

				// load shaders
				ADGRVulkanShaderDataContainer shaderContainer = {};
				shaderContainer.shaderCodes.resize(4);

				// vertex shader
				if (myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.vertexShaderPath != "NONE")
					shaderContainer.shaderCodes[0] = utils::readFile(myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.vertexShaderPath);
				else
					shaderContainer.shaderCodes[0] = {};
				// tessellation shader
				if (myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.tessellationShaderPath != "NONE")
					shaderContainer.shaderCodes[1] = utils::readFile(myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.tessellationShaderPath);
				else
					shaderContainer.shaderCodes[1] = {};
				// geometry shader
				if (myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.geometryShaderPath != "NONE")
					shaderContainer.shaderCodes[2] = utils::readFile(myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.geometryShaderPath);
				else
					shaderContainer.shaderCodes[2] = {};
				// fragment shader
				if (myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.fragmentShaderPath != "NONE")
					shaderContainer.shaderCodes[3] = utils::readFile(myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.fragmentShaderPath);
				else
					shaderContainer.shaderCodes[3] = {};

				// init shaders
				myShaderManager.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), &shaderContainer);

				// init pipeline
				DMKPipelineInitInfo initInfo;
				initInfo.layouts = { myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myDescriptorSetLayout };
				initInfo.shaderDataContainer = shaderContainer;
				auto [_localPipeline, _localPipelineLayout] = myPipeline.init(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), initInfo);

				// add pipelines and pipelineLayouts
				myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myPipeline = _localPipeline;
				myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myPipelineLayout = _localPipelineLayout;

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
			for (int i = 0; i < vulkanFormatObjectsCount; i++) {
				int texturePathSize = myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myGameObject->myProperties.texturePaths.size();
				float mipLevels = myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myMipLevel;
				myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImages.resize(texturePathSize);
				myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImageMemories.resize(texturePathSize);
				myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImageSamplers.resize(texturePathSize);
				myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImageViews.resize(texturePathSize);

				if (myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_SKYBOX) {
					for (int itr = 0; itr < texturePathSize; itr++) {
						// skybox
						DMKInitCubemapInfo cubemapInfo = {};
						cubemapInfo.path = myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myGameObject->myProperties.texturePaths[itr];
						cubemapInfo.textureImage = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImages[itr];
						cubemapInfo.textureImageMemory = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImageMemories[itr];
						cubemapInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
						cubemapInfo.mipLevels = mipLevels;
						cubemapInfo.imageView = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImageViews[itr];
						cubemapInfo.imageSampler = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImageSamplers[itr];
						cubemapInfo.flipImage = invert[itr];
						mySkyboxManager.loadCubemap(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), cubemapInfo);
					}
				}
				else {
					// texture
					for (int itr = 0; itr < texturePathSize; itr++) {
						// texture creation - init
						DMKInitTextureInfo textureInfo;
						textureInfo.path = myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myGameObject->myProperties.texturePaths[itr];
						textureInfo.textureImage = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImages[itr];
						textureInfo.textureImageMemory = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImageMemories[itr];
						textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
						textureInfo.mipLevels = mipLevels;
						myTextureManager.initTexture(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), textureInfo);

						// texture - imageViews
						DMKInitTextureImageViewsInfo viewInfo;
						viewInfo.textureImage = myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImages[itr];
						viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
						viewInfo.mipLevels = mipLevels;

						DMKCreateImageViewInfo cImgVewinfo;
						cImgVewinfo.device = myManager.getResource<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex).device;
						cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
						cImgVewinfo.image = myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImages[itr];
						cImgVewinfo.mipLevels = mipLevels;
						cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
						cImgVewinfo.textureImageView = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImageViews[itr];
						myTextureManager.initTextureImageViews(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), viewInfo, cImgVewinfo);

						// texture - sampler
						DMKInitTextureSamplerInfo samplerInfo;
						samplerInfo.textureSampler = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImageSamplers[itr];
						samplerInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
						samplerInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
						samplerInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
						samplerInfo.magFilter = VK_FILTER_LINEAR;
						samplerInfo.minFilter = VK_FILTER_LINEAR;
						samplerInfo.mipLevel = mipLevels;
						myTextureManager.initTextureSampler(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex), samplerInfo);
					}
				}
			}
		}

		void vulkanRenderer::initVertexAndIndexBuffers() {
			for (int i = 0; i < vulkanFormatObjectsCount; i++) {
				myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myVertexBuffers.resize(1);
				myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myVertexBufferMemories.resize(1);
				myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myIndexBuffers.resize(1);
				myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myIndexBufferMemories.resize(1);

				// create vertexBuffer
				DMKVulkanRendererCreateVertexBufferInfo vertBuffInfo;
				vertBuffInfo.vertexBufferObject = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myVertexBufferObjects[0];
				vertBuffInfo.buffer = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myVertexBuffers[0];
				vertBuffInfo.bufferMemory = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myVertexBufferMemories[0];
				createVertexBuffer(vertBuffInfo);

				// create indexBuffer
				DMKVulkanRendererCreateIndexBufferInfo idxBuffInfo;
				idxBuffInfo.indexBufferObject = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myRendererFormat->myInternalFormat->myIndexBufferObjects[0];
				idxBuffInfo.buffer = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myIndexBuffers[0];
				idxBuffInfo.bufferMemory = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myIndexBufferMemories[0];
				createIndexBuffer(idxBuffInfo);
			}
		}

		// initialize uniform buffers
		void vulkanRenderer::initUniformBuffers() {
			for (int itr = 0; itr < vulkanFormatObjectsCount; itr++) {
				DMKVulkanRendererCreateUniformBufferInfo uBuffInfo;
				uBuffInfo.buffer = &myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myUniformBuffers;
				uBuffInfo.bufferMemory = &myManager.getResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT, itr)->myUniformBufferMemories;
				createUniformBuffer(uBuffInfo);
			}
		}

		// initialize descriptor pools and sets
		void vulkanRenderer::initDescriptorPoolsAndSets() {
			for (int i = 0; i < vulkanFormatObjectsCount; i++) {
				int textureImagesSize = myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImages.size();
				myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myDescriptorSets.resize(textureImagesSize); // wrong method
				myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myDescriptorPools.resize(textureImagesSize); // wrong method

				for (int itr = 0; itr < textureImagesSize; itr++) {
					// init descriptor pool
					uniformBuffer.initDescriptorPool(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex),
						&myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myDescriptorPools[itr]);

					// init descriptor set
					DMKVulkanRendereCreateDescriptorSetsInfo descInitInfo;
					descInitInfo.uniformBuffers = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myUniformBuffers;
					descInitInfo.textureImageView = myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImageViews[itr];
					descInitInfo.textureSampler = myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myTextureImageSamplers[itr];
					descInitInfo.descriptorSets = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myDescriptorSets[itr];
					descInitInfo.layout = &myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myDescriptorSetLayout;
					descInitInfo.descriptorPool = myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT)->at(i).myDescriptorPools[itr];
					descInitInfo.bindIndexes = { 0, 1 };
					createDescriptorSets(descInitInfo);
				}
			}
		}

		// initialize command buffers
		void vulkanRenderer::initCommandBuffers() {
			DMKBindCommandBufferInfo commandInfo;
			commandInfo.objectBindDatas = myManager.getFullResourceAddr<vulkanFormat>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT);
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