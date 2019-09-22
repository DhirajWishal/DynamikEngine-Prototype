#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer (2D)

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		22/09/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_VULKAN_RENDERER_2D_
#define _DYNAMIK_VULKAN_RENDERER_2D_

#include "backend/Renderer/Renderer.h"

#include "core/backend.h"
#include "core/data structures/DMK_ADGR_DataStructures.h"

//#define INC_PROGRESS	(*myProgress += 1)
#define INC_PROGRESS	

namespace Dynamik {
	namespace ADGR {
		using namespace core;

		class vulkanRenderer2D : c_Renderer {
		public:
			vulkanRenderer2D();
			~vulkanRenderer2D();

			void init() override;
			void drawFrame() override;
			void shutDown() override;

			void loadObject(DMKVulkanRendererLoadObjectInfo info);

			void createVertexBuffer(DMKVulkanRendererCreateVertexBufferInfo info);
			void createIndexBuffer(DMKVulkanRendererCreateIndexBufferInfo info);
			void createUniformBuffer(DMKVulkanRendererCreateUniformBufferInfo info);
			void createDescriptorSets(DMKVulkanRendereCreateDescriptorSetsInfo info);

		private:
			core::window myWindow{};

			core::instanceManager myInstance{};
			core::device myDevice{};
			core::swapChain mySwapChain{};
			core::pipeline myPipeline{};
			core::uniformBufferManager uniformBuffer{};
			core::commandBufferManager myCommandBufferManager{};
			core::colorBufferManager myColorBufferManager{};
			//core::depthBufferManager myDepthBufferManager{};
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
			std::vector<std::string> modelPaths;
			std::vector<std::string> texturePaths;
			std::vector<std::string> vertexShaderPaths;
			std::vector<std::string> fragmentShaderPaths;

			//uint32_t* myProgress = 0;
		};
	}
}

#endif
