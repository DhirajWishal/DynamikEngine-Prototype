#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "core/backend.h"
#include "core/data structures/DMK_ADGR_DataStructures.h"

namespace Dynamik {
	namespace ADGR {
		using namespace core;

		class vulkanRenderer {
		public:
			vulkanRenderer();
			~vulkanRenderer();

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

			std::tuple<keyEventData*, mouseEventData*> getEvent() { return myWindow.getEvent(); }

			VkDevice getDevice() { return myDevice.getDeviceCpy(); }


			void createVertexBuffer(DMKVulkanRendererCreateVertexBufferInfo info);
			void createIndexBuffer(DMKVulkanRendererCreateIndexBufferInfo info);
			void createUniformBuffer(DMKVulkanRendererCreateUniformBufferInfo info);
			void createDescriptorSets(DMKVulkanRendereCreateDescriptorSetsInfo info);

		private:
			core::window myWindow;

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

			std::vector<core::Vertex> terrainVBO;
			std::vector<core::Vertex> vbo2;
			std::vector<uint32_t> ibo;
			std::vector<VkDescriptorSet> descriptorSets;
			std::vector<VkDescriptorSet> descriptorSets2;

			std::vector<VkSemaphore> imageAvailableSemaphore;
			std::vector<VkSemaphore> renderFinishedSemaphore;
			std::vector<VkFence> inFlightFence;

			VkImage texImage;
			VkDeviceMemory texImageMemory;

			VkBuffer terrainVertexBuffer;
			VkBuffer vertexBuffer2;
			VkDeviceMemory terrainVertexBufferMemory;
			VkDeviceMemory vertexBufferMemory2;
			VkBuffer indexBuffer;
			VkDeviceMemory indexBufferMemory;
			std::vector<VkBuffer> uniformBuffers;
			std::vector<VkBuffer> uniformBuffers2;
			std::vector<VkDeviceMemory> uniformBufferMemories;
			std::vector<VkDeviceMemory> uniformBufferMemories2;

			VkDescriptorPool descriptorPool;
			VkDescriptorSetLayout layout;
			VkDescriptorPool descriptorPool2;
			VkDescriptorSetLayout layout2;

			VkSampler textureSampler;
			VkImageView textureImageView;

			std::vector<core::Vertex> vertices;

			uint32_t currentFrame = 0;
			uint32_t myMipLevel = 1;

			core::keyEvent myEvent;

		};
	}
}


class Benchmark {
public:
	Benchmark() {
		startTimePoint = std::chrono::high_resolution_clock::now();
	}

	~Benchmark() {
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>
			(startTimePoint).time_since_epoch().count();
		auto stop = std::chrono::time_point_cast<std::chrono::microseconds>
			(endTimePoint).time_since_epoch().count();

		auto duration = stop - start;
		double microSeconds = duration * .001;

		printf("Timer ended. Results: %lld us ( %f ms)\n", duration, microSeconds);
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;

	std::chrono::time_point<std::chrono::high_resolution_clock> time;
	std::chrono::time_point<std::chrono::high_resolution_clock> oldTime;
};

class FPS {
public:
	FPS() {
		printf("\t-----Benchmark tool: FPS Calculator initiated!-----\n");
	}

	void getFPS() {
		if (!refresh) {
			//system("CLS");
			//std::cout << std::flush;
			refresh = true;
		}

		time = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>
			(time).time_since_epoch().count();
		auto old = std::chrono::time_point_cast<std::chrono::microseconds>
			(oldTime).time_since_epoch().count();

		current = (uint64_t)(1000 / ((start - old) * .001));

		if (current < minimum) minimum = current;
		if (current > maximum) maximum = current;

		printf("FPS: %I64d\n", current);

		oldTime = time;
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> time;
	std::chrono::time_point<std::chrono::high_resolution_clock> oldTime;

	uint64_t current;
	uint64_t minimum = 10e100;
	uint64_t maximum = 0;

	bool refresh = false;
};

