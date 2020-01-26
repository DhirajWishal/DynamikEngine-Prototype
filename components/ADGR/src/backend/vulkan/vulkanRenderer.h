#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 vulkanRenderer.h file
 */

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_H

#include "backend/base/RendererBackendBase.h"

#include "core/backend.h"
#include "core/data structures/DMK_ADGR_DataStructures.h"

#include "GameObject.h"
#include "debugger.h"

#include "core/data store/internalFormat.h"

namespace Dynamik {
	namespace ADGR {
		using namespace core;

		class vulkanRenderer : public RendererBackend {
		public:
			vulkanRenderer();
			vulkanRenderer(DMKRendererSettings settings) : RendererBackend(settings) {}
			~vulkanRenderer();

			void init() override;
			void initStageOne() override;
			void initStageTwo() override;
			void initStageThree() override;

			void drawFrame() override;
			void shutDown() override;


			std::deque<DMKEventContainer>* events() override;
			inline bool closeEvent() override { return myWindowManager.closeEvent(&myVulkanDataContainers[vulkanContainerIndex]); }

			void setFormats(std::vector<RendererFormat>& rendererFormats) override;
			void updateFormats(std::vector<RendererFormat>& rendererFormats) override;

			void recreateSwapChain();
			void setVertices(std::vector<Vertex>& vertices) { myVertices = vertices; }

			inline VkDevice getDevice() { return myVulkanDataContainers[vulkanContainerIndex].device; }

			void createVertexBuffer(DMKVulkanRendererCreateVertexBufferInfo info);
			void createIndexBuffer(DMKVulkanRendererCreateIndexBufferInfo info);
			void createUniformBuffer(DMKVulkanRendererCreateUniformBufferInfo info);
			void createDescriptorSets(DMKVulkanRendereCreateDescriptorSetsInfo info);

			/* STAGE BASED INITIALIZATION(STARTUP) */
			void initManagerFunctions();
			void initWindow();
			void initInstance();
			void initDebugger();
			void initwindowSurface();
			void initDevice();
			void initSwapChain();
			void initDescriptorSetLayout(vulkanFormat* myVulkanFormat);
			void initRenderPass();
			void initPipelines(vulkanFormat* myVulkanFormat);
			void initCommandPool();
			void initColorBuffer();
			void initDepthBuffer();
			void initFrameBuffers();
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

		private:
			void _addVulkanFormatsToManager(std::vector<RendererFormat>& rendererFormats);

			// renderer core classes
			core::windowManager myWindowManager{};
			core::instanceManager myInstanceManager{};
			core::deviceManager myDeviceManager{};
			core::swapChainManager mySwapChainManager{};
			core::pipelineManager myPipelineManager{};
			core::uniformBufferManager uniformBufferManager{};
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

			uint32_t imageIndex = 0;
			uint32_t currentFrame = 0;
			uint32_t vulkanContainerIndex = 0;

			bool enableVertexAndIndexClear = true;

			std::vector<Vertex> myVertices = {};
			std::vector<std::future<void>> myThreads = {};

			/* DATA STORE */
			std::vector<ADGRVulkanDataContainer> myVulkanDataContainers = {};
			std::vector<vulkanFormat> myVulkanFormats = {};

			std::vector<VkDescriptorSetLayout> myDescriptorSetLayouts = {};
			std::vector<VkSemaphore> myImageAvailableSemaphores = {};
			std::vector<VkSemaphore> myRenderFinishedSemaphores = {};
			std::vector<VkFence> myFencesInFlight = {};
		};
	}
}

#endif	//	_DMK_ADGR_RENDER_H
