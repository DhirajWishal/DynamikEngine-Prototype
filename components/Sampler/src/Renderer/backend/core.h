#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

/* ---------- ########## ////////// CORE INCLUDES \\\\\\\\\\ ########## ---------- */
#include "buffers/buffer.h"
#include "buffers/colorBuffer.h"
#include "buffers/commandBuffer.h"
#include "buffers/depthBuffer.h"
#include "buffers/frameBuffer.h"
#include "buffers/indexBuffer.h"
#include "buffers/uniformBuffer.h"
#include "buffers/vertexBuffer.h"
#include "device/device.h"
#include "extensions/extensions.h"
#include "instance/instance.h"
#include "loader/model.h"
#include "pipeline/pipeline.h"
#include "queues/queues.h"
#include "swapchain/swapChain.h"
#include "texture/texture.h"
#include "validators/debugger/debugger.h"
#include "validators/validators.h"

#include "others/semaphores.h"
#include "others/syncObjects.h"
#include "defines.h"

#include "Renderer/Platform/Windows.h"
#include "Renderer/Platform/Windows/utils/loadIcon.h"

#include "core/utils/DMK_DataTypes.h"
#include "Renderer/backend/interface.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class SMP_API core {
			public:
				core();
				~core();

				void startup();
				void shutdown();
				void drawFrame();
				void recreateSwapChain();

				void initWindow();

				GLFWwindow* getWindow() { return window; }
				VkDevice getDevice() { return myDevice; }

				static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

				// helper functions;

				void setMipLevels(float);

			private:
				GLFWwindow* window = nullptr;

				VkInstance myInstance;
				VkDebugUtilsMessengerEXT debugMessenger;
				VkPhysicalDevice myPhysicalDevice;
				VkDevice myDevice;
				VkSurfaceKHR mySurface;
				VkSwapchainKHR mySwapchain;
				VkRenderPass myRenderPass;
				VkPipelineLayout myPipelineLayout;
				VkCommandPool myCommandPool;

				VkBuffer VertexBuffer;
				VkDeviceMemory vertexBufferMemory;
				VkBuffer IndexBuffer;
				VkDeviceMemory indexBufferMemory;
				VkBuffer StagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				uint32 mipLevels;
				VkImage textureImage;
				VkDeviceMemory textureImageMemory;
				VkImageView textureImageView;
				VkSampler textureSampler;
				VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
				VkImage colorImage;
				VkDeviceMemory colorImageMemory;
				VkImageView colorImageView;

				VkImage depthImage;
				VkDeviceMemory depthImageMemory;
				VkImageView depthImageView;

				std::vector<VkBuffer> UniformBuffers;
				std::vector<VkDeviceMemory> uniformBuffersMemory;

				VkDescriptorPool descriptorPool;
				VkDescriptorSetLayout descriptorSetLayout;
				std::vector<VkDescriptorSet> descriptorSets;

				VkSemaphore imageAvailableSemaphore;
				VkSemaphore renderFinishedSemaphore;
				std::vector<VkSemaphore> imageAvailableSemaphores;
				std::vector<VkSemaphore> renderFinishedSemaphores;
				std::vector<VkFence> inFlightFences;

				VkPipeline graphicsPipeline;

				VkQueue graphicsQueue;
				VkQueue presentQueue;

				std::vector<VkImage> swapChainImages;
				VkFormat swapChainImageFormat;
				VkExtent2D swapChainExtent;
				std::vector<VkImageView> swapChainImageViews;

				instance instance{ &myInstance };
				device device{ &myDevice, &myPhysicalDevice, &mySurface };
				debugger debugger{ &myInstance, &debugMessenger };
				swapChain swapchain{ &myDevice, &mySwapchain, &myPhysicalDevice, &mySurface };
				frameBuffer swapchainFrameBuffers{ &myDevice };
				commandBuffer commandBuffer{ &myDevice, &myCommandPool };
				pipeline pipeline{ &myDevice, &myRenderPass, &graphicsPipeline };
				vertexBuffer vertexBuffer{ &myDevice, &myPhysicalDevice, &VertexBuffer, &vertexBufferMemory };
				indexBuffer indexBuffer{ &myDevice, &myPhysicalDevice, &IndexBuffer, &indexBufferMemory };
				uniformBuffer uniformBuffer{ &myDevice, &myPhysicalDevice, &UniformBuffers,
					&uniformBuffersMemory, &descriptorPool, &descriptorSets, &descriptorSetLayout };
				texture texture{ &myDevice, &myPhysicalDevice, &StagingBuffer, &stagingBufferMemory,
					&textureImage, &textureImageMemory, &textureImageView, &textureSampler, &mipLevels };
				depthBuffer depthBuffer{ &myDevice, &myPhysicalDevice, &depthImage, &depthImageMemory,
					&depthImageView };
				colorBuffer colorBuffer{ &myDevice, &myPhysicalDevice, &colorImage, &colorImageMemory,
					&colorImageView };

				model myModel;

				uint32 currentFrame = 0;
				bool frameBufferResized = false;

				std::vector<std::vector<std::string>> shaderPath;
				std::vector<std::vector<std::string>> assetsPath;

				const std::string vertexShaderPath = "E:/Projects/Dynamik Engine/Dynamik/components/Shaders/vert.spv";
				const std::string fragmentShaderPath = "E:/Projects/Dynamik Engine/Dynamik/components/Shaders/frag.spv";
				const std::string MODEL_PATH = "E:/Projects/Dynamik Engine/Dynamik/core assets/models/chestnut.obj";
				const std::string TEXTURE_PATH = "E:/Projects/Dynamik Engine/Dynamik/core assets/textures/Kashtan.jpg";
			
				std::chrono::time_point<std::chrono::high_resolution_clock> time;
				std::chrono::time_point<std::chrono::high_resolution_clock> oldTime;
				};
		}
	}
}

class timer {
public:
	timer(const char* file, int line) {
		std::cout << "Timer initiated at " << file << ":" << line << " ...\n";

		startTimePoint = std::chrono::high_resolution_clock::now();
	}
	~timer() {
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>
			(startTimePoint).time_since_epoch().count();
		auto stop = std::chrono::time_point_cast<std::chrono::microseconds>
			(endTimePoint).time_since_epoch().count();

		auto duration = stop - start;
		double microSeconds = duration * .001;

		std::cout << "Timer ended. Results: " << duration << "us (" << microSeconds << "ms)\n";
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;

};
