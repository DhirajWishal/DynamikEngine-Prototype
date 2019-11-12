#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "backend/backend.h"

namespace Dynamik {
	namespace ADGR {

		class Renderer {
		public:
			Renderer();
			~Renderer();

			void init();
			void drawFrame();
			void shutdown();

			void recreateSwapChain();

			void loadObjectData();
			void events() {
				myWindow.pollEvents();
			}
			bool closeEvent() { return myWindow.closeEvent(); }

		private:
			core::window myWindow;

			VkInstance instance;
			VkDevice device;

			core::instanceManager myInstance;
			core::device myDevice;
			core::swapChain mySwapChain;
			core::pipeline myPipeline;
			core::uniformBufferManager uniformBuffer;
			core::commandBufferManager myCommandBufferManager;
			core::colorBufferManager myColorBufferManager;
			core::depthBufferManager myDepthBufferManager;
			core::frameBufferManager myFrameBufferManager;
			core::textureManager myTextureManager;
			core::modelManager myModelManager;
			core::vertexBufferManager myVertexBufferManager;
			core::indexBufferManager myIndexBufferManager;

			core::shaderManager myShaderManager;

			core::debugger myDebugger{ myInstance.getInstanceAddr(), myInstance.getdebugMessengerAddr() };

			std::vector<core::Vertex> vbo;
			std::vector<uint32_t> ibo;
			std::vector<VkDescriptorSet> descriptorSets;

			std::vector<VkSemaphore> imageAvailableSemaphore;
			std::vector<VkSemaphore> renderFinishedSemaphore;
			std::vector<VkFence> inFlightFence;

			VkImage texImage;
			VkDeviceMemory texImageMemory;

			VkBuffer vertexBuffer;
			VkDeviceMemory vertexBufferMemory;
			VkBuffer indexBuffer;
			VkDeviceMemory indexBufferMemory;
			std::vector<VkBuffer> uniformBuffers;
			std::vector<VkDeviceMemory> uniformBufferMemories;

			VkSampler textureSampler;
			VkImageView textureImageView;

			std::vector<core::Vertex> vertices;

			uint32_t currentFrame = 0;

			core::keyEvent myEvent;

		};
	}
}
