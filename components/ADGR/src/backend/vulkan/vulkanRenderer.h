#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_H

#include "core/backend.h"
#include "core/data structures/DMK_ADGR_DataStructures.h"

#define INC_PROGRESS	(*myProgress += 1)

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

			inline void setProgress(uint32_t* progress) {
				myProgress = progress;
			}

			inline void setCommandOutput(std::string* commandString) {
				commandOutput = commandString;
			}

			void init();
			void drawFrame();
			void shutdown();

			void recreateSwapChain();

			void loadObjectData();
			void loadObject(DMKVulkanRendererLoadObjectInfo info);
			inline void events() {
				myWindow.pollEvents();
			}
			inline bool closeEvent() { return myWindow.closeEvent(&myVulkanDataContainer); }

			void setModelPaths(std::vector<std::string>& object, std::vector<std::string>& texture);
			void setShaderPaths(std::vector<std::string>& vertex, std::vector<std::string>& fragment);

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

			void setDataToSimulation();

			ADGRVulkanDataContainer myVulkanDataContainer;

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

			// terrain data
			std::vector<core::Vertex> terrainVBO = {};
			VkBuffer terrainVertexBuffer = VK_NULL_HANDLE;
			VkDeviceMemory terrainVertexBufferMemory = VK_NULL_HANDLE;

			std::vector<uint32_t> terrainIBO = {};
			VkBuffer terrainIndexBuffer = VK_NULL_HANDLE;
			VkDeviceMemory terrainIndexMemory = VK_NULL_HANDLE;

			std::vector<core::Vertex> terrainVBO2 = {};
			VkBuffer terrainVertexBuffer2 = VK_NULL_HANDLE;
			VkDeviceMemory terrainVertexBufferMemory2 = VK_NULL_HANDLE;

			std::vector<uint32_t> terrainIBO2 = {};
			VkBuffer terrainIndexBuffer2 = VK_NULL_HANDLE;
			VkDeviceMemory terrainIndexMemory2 = VK_NULL_HANDLE;

			//std::vector<std::vector<core::Vertex>> vertexBuffers = {};
			//std::vector<std::vector<uint32_t>> indexBuffers = {};

			std::vector<core::PointVertex> simulationVBO = {};
			VkBuffer simulationVertexBuffer = VK_NULL_HANDLE;
			VkDeviceMemory simulationVertexBufferMemory = VK_NULL_HANDLE;

			// cube map data
			VkImage cubeMap;
			VkImageView cubeMapImageView;
			VkDeviceMemory cubeMapMemory;

			std::vector<VkDescriptorSet> descriptorSets = {};
			std::vector<VkDescriptorSet> descriptorSets2 = {};

			std::vector<VkSemaphore> imageAvailableSemaphore = {};
			std::vector<VkSemaphore> renderFinishedSemaphore = {};
			std::vector<VkFence> inFlightFence = {};

			std::vector<core::Vertex> vbo = {};
			VkBuffer vertexBuffer = VK_NULL_HANDLE;
			VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;

			std::vector<core::Vertex> vbo2 = {};
			VkBuffer vertexBuffer2 = VK_NULL_HANDLE;
			VkDeviceMemory vertexBufferMemory2 = VK_NULL_HANDLE;
			std::vector<uint32_t> ibo = {};
			VkBuffer indexBuffer = VK_NULL_HANDLE;
			VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
			std::vector<uint32_t> ibo2 = {};
			VkBuffer indexBuffer2 = VK_NULL_HANDLE;
			VkDeviceMemory indexBufferMemory2 = VK_NULL_HANDLE;

			std::vector<VkBuffer> uniformBuffers;
			std::vector<VkBuffer> uniformBuffers2;
			std::vector<VkDeviceMemory> uniformBufferMemories;
			std::vector<VkDeviceMemory> uniformBufferMemories2;


			std::vector<std::vector<VkBuffer>> uniformBuffersContainer;
			std::vector<std::vector<VkDeviceMemory>> uniformBufferMemoriesContainer;
			std::vector<VkDescriptorPool> descriptorPools;
			std::vector<std::vector<VkDescriptorSet>> descriptorSetsContainer;


			VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
			VkDescriptorSetLayout layout = VK_NULL_HANDLE;
			VkDescriptorPool descriptorPool2 = VK_NULL_HANDLE;
			VkDescriptorSetLayout layout2 = VK_NULL_HANDLE;

			VkImage texImage = VK_NULL_HANDLE;
			VkDeviceMemory texImageMemory = VK_NULL_HANDLE;
			VkSampler textureSampler = VK_NULL_HANDLE;
			VkImageView textureImageView = VK_NULL_HANDLE;

			VkImage texImage2 = VK_NULL_HANDLE;
			VkDeviceMemory texImageMemory2 = VK_NULL_HANDLE;
			VkSampler textureSampler2 = VK_NULL_HANDLE;
			VkImageView textureImageView2 = VK_NULL_HANDLE;

			DMKVulkanSkyboxContainer mySkybox;

			std::vector<core::Vertex> vertices = {};


			std::vector<std::vector<Vertex>> vertexBufferObjects;
			std::vector<VkBuffer> vertexBuffers;
			std::vector<VkDeviceMemory> vertexBufferMemories;

			std::vector<std::vector<uint32_t>> indexBufferObjects;
			std::vector<VkBuffer> indexBuffers;
			std::vector<VkDeviceMemory> indexBufferMemories;

			std::vector<VkImage> textureImages;
			std::vector<VkDeviceMemory> textureImageMemories;
			std::vector<VkSampler> textureSamplers;
			std::vector<VkImageView> textureImageViews;


			uint32_t currentFrame = 0;
			uint32_t myMipLevel = 1;

			core::keyEvent myEvent{};
			core::cursorEvent myCEvent{};

			uint32_t* myProgress = 0;

			// dynamic paths
			std::vector<std::string> modelPaths = {};
			std::vector<std::string> texturePaths = {};
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

			bool compileShaders = true;

			std::string* commandOutput = nullptr;

			std::vector<std::string> textureSetPaths = {
				//"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/Wolf_Eyes_1.jpg",
				//"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/Wolf_Eyes_2.jpg",
				//"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/Material__47_-_Default_Texture.png",
				//"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/Wolf_Body.jpg",
				"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/tree/bark_tree.jpg",
				//"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/tree/bark_tree_nor.jpg",
				//"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/tree/blade_sample.jpg",
				"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/tree/human.jpg",
				"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/tree/leaves_01.jpg",
				"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/tree/leaves_02.jpg",
				"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/tree/leaves_03.jpg",
				//"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/tree/leaves_alpha.jpg",
				//"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/tree/leaves_alpha_inverted.jpg",
				//"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/tree/ground.jpg",
				//"E:/Projects/Dynamik Engine/Dynamik/core assets/textures/tree/concrete.png"
			};

			int shaderCodeIndex = 0;

			uint32_t vertexCount = 0;
			uint32_t indexCount = 0;

			uint32_t vertexCount2 = 0;
			uint32_t indexCount2 = 0;

			std::vector<uint32_t> vertexCounts = {};
			std::vector<uint32_t> indexCounts = {};

			std::vector<std::thread> threads;

			int count = 1;

			std::vector<DMKObjectData> objDataVector = {};
		};
	}
}

#endif	//	_DMK_ADGR_VULKAN_RENDERER_H
