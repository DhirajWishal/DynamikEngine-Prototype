#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_VULKAN_RENDERER_H
#define _DYNAMIK_VULKAN_RENDERER_H

#include "core/backend.h"
#include "core/data structures/DMK_ADGR_DataStructures.h"

#define INC_PROGRESS	(*myProgress += 1)

namespace Dynamik {
	namespace ADGR {
		using namespace core;

		struct DMK_ADGR_CreatePipelineInfo {
			DMKPipelineInitInfo pipelineInitInfo;
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
			void events() {
				myWindow.pollEvents();
			}
			bool closeEvent() { return myWindow.closeEvent(); }

			void setModelPaths(std::vector<std::string>& object, std::vector<std::string>& texture);
			void setShaderPaths(std::vector<std::string>& vertex, std::vector<std::string>& fragment);

			std::tuple<int, mouseEventData*> getEvent() { return myWindow.getEvent(); }

			VkDevice getDevice() { return myDevice.getDeviceCpy(); }

			void initPipeline(DMK_ADGR_CreatePipelineInfo info);

			void createVertexBuffer(DMKVulkanRendererCreateVertexBufferInfo info);
			void createIndexBuffer(DMKVulkanRendererCreateIndexBufferInfo info);
			void createUniformBuffer(DMKVulkanRendererCreateUniformBufferInfo info);
			void createDescriptorSets(DMKVulkanRendereCreateDescriptorSetsInfo info);

			void includeShader();

		private:
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

			core::debugger myDebugger{ myInstance.getInstanceAddr(), myInstance.getdebugMessengerAddr() };

			// terrain data
			std::vector<core::Vertex> terrainVBO = {};
			VkBuffer terrainVertexBuffer = VK_NULL_HANDLE;
			VkDeviceMemory terrainVertexBufferMemory = VK_NULL_HANDLE;

			// cube map data
			VkImage cubeMap;
			VkImageView cubeMapImageView;
			VkDeviceMemory cubeMapMemory;

			std::vector<core::Vertex> vbo2 = {};
			std::vector<uint32_t> ibo = {};
			std::vector<VkDescriptorSet> descriptorSets = {};
			std::vector<VkDescriptorSet> descriptorSets2 = {};

			std::vector<VkSemaphore> imageAvailableSemaphore = {};
			std::vector<VkSemaphore> renderFinishedSemaphore = {};
			std::vector<VkFence> inFlightFence = {};

			VkImage texImage = VK_NULL_HANDLE;
			VkDeviceMemory texImageMemory = VK_NULL_HANDLE;

			VkBuffer vertexBuffer2 = VK_NULL_HANDLE;
			VkDeviceMemory vertexBufferMemory2 = VK_NULL_HANDLE;
			VkBuffer indexBuffer = VK_NULL_HANDLE;
			VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
			std::vector<VkBuffer> uniformBuffers;
			std::vector<VkBuffer> uniformBuffers2;
			std::vector<VkDeviceMemory> uniformBufferMemories;
			std::vector<VkDeviceMemory> uniformBufferMemories2;

			VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
			VkDescriptorSetLayout layout = VK_NULL_HANDLE;
			VkDescriptorPool descriptorPool2 = VK_NULL_HANDLE;
			VkDescriptorSetLayout layout2 = VK_NULL_HANDLE;

			VkSampler textureSampler = VK_NULL_HANDLE;
			VkImageView textureImageView = VK_NULL_HANDLE;

			//VkImage skyBox;

			DMKVulkanSkyboxContainer mySkybox;

			std::vector<core::Vertex> vertices = {};

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

			int shaderCodeIndex = 0;
		};
	}
}

#endif	//
