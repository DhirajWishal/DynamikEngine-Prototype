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

#include "GameObject/GameObject.h"
#include "debugger.h"

#define INC_PROGRESS	(*myProgress += 1)

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
			DMK_ADGR_RENDERING_TECHNOLOGY renderingType = DMK_ADGR_VULKAN_RENDERER_INDEXED;
		};

		class vulkanRenderer {
		public:
			vulkanRenderer();
			~vulkanRenderer();

			inline void setProgress(uint32_t* progress) {
				myProgress = progress;
			}

			void init();
			void drawFrame();
			void shutdown();

			void recreateSwapChain();

			static void loadObjectData(ADGRVulkanDataContainer* vulkanDataContainer,
				DMKObjectDataCreateInfo& info, ADGRObjectRenderData* renderData);

			void loadObject(DMKVulkanRendererLoadObjectInfo info);
			inline void events() {
				myWindow.pollEvents();
			}
			inline bool closeEvent() { return myWindow.closeEvent(&myVulkanDataContainer); }

			void setGameObjects(std::vector<GameObject>& gameObjects) {
				gameObjectStore = gameObjects;
			}
			void setModelPaths(std::vector<std::string>& object, std::vector<std::vector<std::string>>& texture);
			void setShaderPaths(std::vector<std::string>& vertex, std::vector<std::string>& fragment);

			void initGameObjects();

			inline std::tuple<int, mouseEventData*> getEvent() { return myWindow.getEvent(); }

			inline VkDevice getDevice() { return myVulkanDataContainer.device; }

			void initPipeline(DMK_ADGR_CreatePipelineInfo info);

			void createVertexBuffer(DMKVulkanRendererCreateVertexBufferInfo info);
			void createIndexBuffer(DMKVulkanRendererCreateIndexBufferInfo info);
			void createUniformBuffer(DMKVulkanRendererCreateUniformBufferInfo info);
			void createDescriptorSets(DMKVulkanRendereCreateDescriptorSetsInfo info);

			void includeShader();

			void initModels(std::vector<DMKObjectData> data);

		private:
			// threads
			void thread_second();	// textures
			static void thread_third(DMKVulkanRendererLoadObjectInfo createInfo);	// models

			ADGRVulkanDataContainer myVulkanDataContainer = {};

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
			core::modelManager myModelManager{};
			core::vertexBufferManager myVertexBufferManager{};
			core::indexBufferManager myIndexBufferManager{};
			core::skybox mySkyboxManager{};
			core::shaderManager myShaderManager{};
			core::debugger myDebugger{ &myVulkanDataContainer.instance, &myVulkanDataContainer.debugMessenger };

			std::vector<VkSemaphore> imageAvailableSemaphore = {};
			std::vector<VkSemaphore> renderFinishedSemaphore = {};
			std::vector<VkFence> inFlightFence = {};

			std::vector<std::vector<VkBuffer>> uniformBuffersContainer = {};
			std::vector<std::vector<VkDeviceMemory>> uniformBufferMemoriesContainer = {};
			std::vector<VkDescriptorPool> descriptorPools = {};
			std::vector<std::vector<VkDescriptorSet>> descriptorSetsContainer = {};

			std::vector<ADGRObjectRenderData> objectDataStore = {};

			VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
			VkDescriptorSetLayout layout = VK_NULL_HANDLE;

			uint32_t currentFrame = 0;
			uint32_t myMipLevel = 1;

			core::keyEvent myEvent{};
			core::cursorEvent myCEvent{};

			uint32_t* myProgress = 0;

			// dynamic paths
			std::vector<std::string> modelPaths = {};
			std::vector<std::vector<std::string>> texturePaths = {};
			std::vector<std::string> vertexShaderPaths = {
				"E:/Projects/Dynamik Engine/Dynamik/Application/vert.spv",
				"E:/Projects/Dynamik Engine/Dynamik/components/Shaders/vert.spv"
			};
			std::vector<std::string> fragmentShaderPaths = {
				"E:/Projects/Dynamik Engine/Dynamik/Application/frag.spv",
				"E:/Projects/Dynamik Engine/Dynamik/components/Shaders/frag.spv"
			};
			std::vector<std::string> vertexShaderSourcePaths = {
				"E:/Projects/Dynamik Engine/Dynamik/components/Shaders/shader.vert",
				"E:/Projects/Dynamik Engine/Dynamik/components/ADGR/src/backend/shaders/shader.2D.vert"
			};
			std::vector<std::string> fragmentShaderSourcePaths = {
				"E:/Projects/Dynamik Engine/Dynamik/components/Shaders/shader.frag",
				"E:/Projects/Dynamik Engine/Dynamik/components/ADGR/src/backend/shaders/shader.2D.frag"
			};

			bool compileShaders = false;

			int shaderCodeIndex = 0;

			int objCount = 25;
			int uniformCount = 1;

			std::vector<DMKObjectData> objDataVector = {};
			std::vector<GameObject> gameObjectStore = {};

			Debugger::benchmark::Profiler localProfiler;
		};
	}
}

#endif	//	_DMK_ADGR_VULKAN_RENDERER_H
