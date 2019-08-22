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
		using namespace core;
		
		class Renderer {
		public:
			Renderer();
			~Renderer();

			void loadObjectData();

		private:
			window myWindow;

			instance myInstance;
			device myDevice;
			swapChain mySwapChain;
			pipeline myPipeline;
			uniformBufferManager uniformBuffer;
			commandBufferManager myCommandBufferManager;
			colorBufferManager myColorBufferManager;
			depthBufferManager myDepthBufferManager;
			frameBufferManager myFrameBufferManager;
			textureManager myTextureManager;
			modelManager myModelManager;
			vertexBufferManager myVertexBufferManager;
			indexBufferManager myIndexBufferManager;

			shaderManager myShaderManager;

			debugger myDebugger{myInstance.getInstanceAddr(), myInstance.getdebugMessengerAddr()};

			std::vector<Vertex> vbo;
			std::vector<uint32_t> ibo;

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

			std::vector<Vertex> vertices;
		};
	}
}
