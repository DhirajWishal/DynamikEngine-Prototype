#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 vulkanRenderer.h

 * Commit convention: (Finalising Dynamik Engine Architecture: stage #)
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

		struct DMKObjectDataCreateInfo {
			std::string modelPath = "";
			std::vector<std::string> texturePaths = {};

			std::vector<float> offsets = { 0.0f, 0.0f, 0.0f };

			DMKPipelineInitInfo pipelineInfo = {};
			bool compileShaders = false;
			std::vector<std::string> shaderPaths;
			std::vector<std::string> rawShaderPaths = {};

			bool requireUniformBuffer = false;
			std::vector<VkBuffer>* uniformBuffers = {};
			std::vector<VkDeviceMemory>* uniformBufferMemory = {};

			uint32_t mipLevel = 1;
			DMK_ADGR_RENDERING_TECHNOLOGY renderingType = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;
		};

		class vulkanRenderer {
		public:
			vulkanRenderer();
			~vulkanRenderer();

			//inline void setProgress(uint32_t* progress) {
			//	myProgress = progress;
			//}

			void init();
			void drawFrame();
			void shutdown();

			void recreateSwapChain();

			std::deque<DMKEventContainer>* events();
			inline bool closeEvent() { return myWindow.closeEvent(myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex)); }

			void setProgress(uint32_t* progress) {
				myProgress = progress;
			}
			void setRendererFormats(std::vector<RendererFormat>& rendererFormats);
			void setModelPaths(std::vector<std::string>& object, std::vector<std::vector<std::string>>& texture);
			void setShaderPaths(std::string& vertex, std::string& fragment);

			inline VkDevice getDevice() { return myManager.getResourceAddr<ADGRVulkanDataContainer>(DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER, vulkanContainerIndex)->device; }

			void initPipeline(DMK_ADGR_CreatePipelineInfo info);

			void createVertexBuffer(DMKVulkanRendererCreateVertexBufferInfo info);
			void createIndexBuffer(DMKVulkanRendererCreateIndexBufferInfo info);
			void createUniformBuffer(DMKVulkanRendererCreateUniformBufferInfo info);
			void createDescriptorSets(DMKVulkanRendereCreateDescriptorSetsInfo info);

			void includeShader();

			/* STAGE BASED INITIALIZATION(STARTUP) */
			void initManagerFunctions();
			void initWindow();
			void initInstance();
			void initDebugger();
			void initWindowSurface();
			void initDevice();
			void initSwapChain();
			void initDescriptorSetLayout();
			void initPipelines();
			void initCommandPool();
			void initColorBuffer();
			void initDepthBuffer();
			void initFrameBuffers();
			void initSkyboxsAndTextures();
			//void initCubemapAndModels();
			void initVertexAndIndexBuffers();
			void initUniformBuffers();
			void initDescriptorPoolsAndSets();
			void initCommandBuffers();
			void initSemaphoresAndFences();

			/* STAGE BASED SHUTDOWN */
			void rendererWait();

			void initModels(std::vector<DMKObjectData> data);
			void initCubemap(DMKObjectData* data);

		private:
			// renderer core classes
			core::window myWindow{};
			core::instanceManager myInstance{};
			core::device myDevice{};
			core::swapChain mySwapChain{};
			core::pipeline myPipeline{};
			core::uniformBufferManager uniformBuffer{};
			core::commandBufferManager myCommandBufferManager{};
			core::colorBufferManager myColorBufferManager{};
			core::depthBufferManager myDepthBufferManager{};
			core::frameBufferManager myFrameBufferManager{};
			core::textureManager myTextureManager{};
			core::vertexBufferManager myVertexBufferManager{};
			core::indexBufferManager myIndexBufferManager{};
			core::skybox mySkyboxManager{};
			core::shaderManager myShaderManager{};
			core::debugger myDebugger{};

			// main data manager
			core::manager myManager{};

			uint32_t currentFrame = 0;
			uint32_t vulkanFormatObjectsCount = 0;

			uint32_t* myProgress = 0;

			bool compileShaders = false;
			bool enableVertexAndIndexClear = true;

			int vulkanContainerIndex = 0;
		};
	}
}

#endif	//	_DMK_ADGR_RENDER_H
