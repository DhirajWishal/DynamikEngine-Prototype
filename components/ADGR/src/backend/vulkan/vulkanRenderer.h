#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 vulkanRenderer.h
 */

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_H

#include "core/backend.h"
#include "core/data structures/DMK_ADGR_DataStructures.h"

#include "GameObject.h"
#include "debugger.h"

#include "core/data store/internalFormat.h"

#define INC_PROGRESS					(*myProgress += 1)
#define MULTIPLE_INC_PROGRESS(count)	for(int i_itr__ = 0; i_itr__ < count; i_itr__++) INC_PROGRESS

namespace Dynamik {
	namespace ADGR {
		using namespace core;

		struct DMK_ADGR_CreatePipelineInfo {
			DMKPipelineInitInfo pipelineInitInfo = {};
		};

		class vulkanRenderer {
		public:
			vulkanRenderer();
			~vulkanRenderer();

			void init();

			void initStageOne();
			void initStageTwo();
			void initStageThree();

			void drawFrame();
			void shutdown();

			void recreateSwapChain();

			std::deque<DMKEventContainer>* events();
			inline bool closeEvent() { return myWindowManager.closeEvent(&myVulkanDataContainers[vulkanContainerIndex]); }

			void setProgress(uint32_t* progress) {
				myProgress = progress;
			}
			void setVulkanFormats(std::vector<RendererFormat>& rendererFormats);
			void updateVulkanFormats(std::vector<RendererFormat>& rendererFormats);
			void setModelPaths(std::vector<std::string>& object, std::vector<std::vector<std::string>>& texture);
			void setShaderPaths(std::string& vertex, std::string& fragment);
			void setVertices(std::vector<Vertex>& vertices) {
				myVertices = vertices;
			}

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
			//void initCubemapAndModels(vulkanFormat* myVulkanFormat);
			void initVertexAndIndexBuffers(vulkanFormat* myVulkanFormat);
			void initUniformBuffers(vulkanFormat* myVulkanFormat);
			void initDescriptorPoolsAndSets(vulkanFormat* myVulkanFormat);
			void initCommandBuffers(std::vector<vulkanFormat>* myVulkanFormats);
			void initSemaphoresAndFences();

			//void initModelsAndTextures(vulkanFormat* format);

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
			uint32_t vulkanFormatObjectsCount = 0;
			uint32_t vulkanContainerIndex = 0;

			uint32_t* myProgress = 0;

			bool compileShaders = false;
			bool enableVertexAndIndexClear = true;

			std::vector<Vertex> myVertices = {};
			std::vector<std::future<void>> myThreads = {};

			/* DATA STORE */
			std::vector<ADGRVulkanDataContainer> myVulkanDataContainers = {};
			std::vector<vulkanFormat> myVulkanFormats = {};
			std::deque<DMKEventContainer> myEventContainers = {};

			std::vector<VkDescriptorSetLayout> myDescriptorSetLayouts = {};
			std::vector<VkSemaphore> myImageAvailableSemaphores = {};
			std::vector<VkSemaphore> myRenderFinishedSemaphores = {};
			std::vector<VkFence> myFencesInFlight = {};

			std::vector<std::string> myModelPaths = {};
			std::vector<std::vector<std::string>> myTexturePaths = {};
			std::vector<std::string> myShaderPaths = {};
		};
	}
}

#endif	//	_DMK_ADGR_RENDER_H
