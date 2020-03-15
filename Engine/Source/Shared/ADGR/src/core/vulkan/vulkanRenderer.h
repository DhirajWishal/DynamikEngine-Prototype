#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 vulkanRenderer.h file
*/

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_H

#include "core/base/RendererBackendBase.h"

#include "backend/backend.h"

#include "GameObject.h"
#include "debugger.h"

#include "core/data store/internalFormat.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		using namespace core;

		/* RENDERER BACKEND LAYER
		 * BASE: RendererBackend
		 * Vulkan Backend API.
		 * Derived from the RendererBackend super class.
		 * This class contains all the necessary functions, data types and objects to correctly
		  handle all Vulkan-based rendering tasks.
		  *
		  * @warn: Only one instance should be created.
		 */
		class vulkanRenderer : public RendererBackend {
			/* DEFAULTS */
		public:
			/* CONSTRUCTOR
			 * Default constructor of the Vulkan Renderer.
			 */
			vulkanRenderer() {}

			/* CONSTRUCTOR
			 * Default constructor of the Vulkan Renderer.
			 *
			 * @param settings: Renderer Backend settings container.
			 */
			vulkanRenderer(DMKRendererSettings settings) : RendererBackend(settings) {}

			/* DESTRUCTOR
			 * Default destructor of the Vulkan Renderer.
			 */
			~vulkanRenderer() {}

			/* PUBLIC FUNCTIONS */
		public:
			/* FUNCTION
			 * Full initialization function.
			 */
			void init() override;

			/* FUNCTION
			 * Stage One initialization.
			 */
			void initStageOne() override;

			/* FUNCTION
			 * Stage Two initialization.
			 */
			void initStageTwo() override;

			/* FUNCTION
			 * Stage Three initialization.
			 */
			void initStageThree() override;

			/* FUNCTION
			 * Draw frame function.
			 */
			void drawFrame() override;

			/* FUNCTION
			 * Full Shut down function.
			 */
			void shutDown() override;

			/* FUNCTION
			 * Shut down stage one.
			 */
			void shutDownStageOne() override;

			/* FUNCTION
			 * Shut down stage two.
			 */
			void shutDownStageTwo() override;

			/* FUNCTION
			 * Shut down stage three.
			 */
			void shutDownStageThree() override;

			/* FUNCTION
			 * Get events.
			 */
			std::deque<DMKEventContainer>* events() override;

			/* FUNCTION
			 * Check for window close event.
			 */
			inline B1 closeEvent() override { return myWindowManager.closeEvent(&myVulkanDataContainers[vulkanContainerIndex]); }

			/* FUNCTION
			 * Set Renderer formats to the renderer.
			 *
			 * @param rendererFormats: An Array of renderer formats.
			 */
			void setFormats(std::vector<RendererFormat>& rendererFormats) override;

			/* FUNCTION
			 * Update the renderer formats.
			 *
			 * @param rendererFormats: An Array of renderer formats.
			 */
			void updateFormats(std::vector<RendererFormat>& rendererFormats) override;

			/* PRIVATE FUNCTIONS */
		private:
			void recreateSwapChain();
			void setVertices(std::vector<Vertex>& vertices) { myVertices = vertices; }

			inline VkDevice getDevice() { return myVulkanDataContainers[vulkanContainerIndex].device; }

			/* STAGE BASED INITIALIZATION(STARTUP) */
			void initManagerFunctions();
			void initWindow();
			void initInstance();
			void initDebugger();
			void initWindowSurface();
			void initDevice();
			void initSwapChain();
			void initCommandPool();
			void initColorBuffer();
			void initDepthBuffer();
			void initRenderPass();
			void initFrameBuffers();
			void otherInitFunctions();

			void initDescriptorSetLayout(vulkanFormat* myVulkanFormat);
			void initPipelines(vulkanFormat* myVulkanFormat);
			void initSkyboxsAndTextures(vulkanFormat* myVulkanFormat);
			void initVertexAndIndexBuffers(vulkanFormat* myVulkanFormat);
			void initUniformBuffers(vulkanFormat* myVulkanFormat);
			void initDescriptorPoolsAndSets(vulkanFormat* myVulkanFormat);
			void initCommandBuffers(std::vector<vulkanFormat>* myVulkanFormats);

			void initSemaphoresAndFences();

			void initObjectBasedFunctions(std::vector<vulkanFormat>* myVulkanFormats);

			/* STAGE BASED SHUTDOWN */
			void rendererWait();

			void initModels(std::vector<DMKObjectData> data);
			void initCubemap(DMKObjectData* data);

			void _addVulkanFormatsToManager(std::vector<RendererFormat>& rendererFormats);

			/* PRIVATE VARIABLES AND CONSTANTS */
		private:
			// renderer core classes
			core::windowManager myWindowManager{};
			core::instanceManager myInstanceManager{};
			core::deviceManager myDeviceManager{};
			core::swapChainManager mySwapChainManager{};
			core::pipelineManager myPipelineManager{};
			core::uniformBufferManager myUniformBufferManager{};
			core::commandBufferManager myCommandBufferManager{};
			core::colorBufferManager myColorBufferManager{};
			core::depthBufferManager myDepthBufferManager{};
			core::frameBufferManager myFrameBufferManager{};
			core::textureManager myTextureManager{};
			core::vertexBufferManager myVertexBufferManager{};
			core::indexBufferManager myIndexBufferManager{};
			core::skyboxManager mySkyboxManager{};
			core::shaderManager myShaderManager{};
			core::debugger myDebugger{};

			UI32 imageIndex = 0;
			UI32 currentFrame = 0;
			UI32 vulkanContainerIndex = 0;

			B1 enableVertexAndIndexClear = true;

			std::vector<Vertex> myVertices = {};
			std::vector<std::future<void>> myThreads = {};

			/* DATA STORE */
			std::vector<ADGRVulkanDataContainer> myVulkanDataContainers = {};
			std::vector<vulkanFormat> myVulkanFormats = {};

			std::vector<VkSemaphore> myImageAvailableSemaphores = {};
			std::vector<VkSemaphore> myRenderFinishedSemaphores = {};
			std::vector<VkFence> myFencesInFlight = {};

			VkDescriptorSetLayout myTerrainDescriptorSetLayout;
			std::vector<VkBuffer> myTerrainUniformBuffers;
			std::vector<VkDeviceMemory> myTerrainUniformBufferMamories;

			VkResult result;
			VkSubmitInfo submitInfo = {};
			VkPresentInfoKHR presentInfo = {};
			VkSemaphore waitSemaphores[1];
			VkPipelineStageFlags waitStages[1];
			VkSemaphore signalSemaphores[1];
			VkSwapchainKHR swapChains[1];
		};
	}
}

#endif
#endif	//	_DMK_ADGR_RENDER_H
