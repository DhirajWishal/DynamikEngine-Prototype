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

#include "Platform/Windows.h"
#include "Platform/Windows/utils/loadIcon.h"

#include "core/utils/DMK_DataTypes.h"
#include "backend/interface.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class ADGR_API core {
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

				VkInstance myInstance = nullptr;
				VkDebugUtilsMessengerEXT debugMessenger = nullptr;
				VkPhysicalDevice myPhysicalDevice = nullptr;
				VkDevice myDevice = nullptr;
				VkSurfaceKHR mySurface = nullptr;
				VkSwapchainKHR mySwapchain = nullptr;
				VkRenderPass myRenderPass = nullptr;
				VkPipelineLayout myPipelineLayout = nullptr;
				VkCommandPool myCommandPool = nullptr;

				VkBuffer VertexBuffer = nullptr;
				VkDeviceMemory vertexBufferMemory = nullptr;
				VkBuffer IndexBuffer = nullptr;
				VkDeviceMemory indexBufferMemory = nullptr;
				VkBuffer StagingBuffer = nullptr;
				VkDeviceMemory stagingBufferMemory = nullptr;

				uint32 mipLevels = 0;
				VkImage textureImage = nullptr;
				VkDeviceMemory textureImageMemory = nullptr;
				VkImageView textureImageView = nullptr;
				VkSampler textureSampler = nullptr;
				VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
				VkImage colorImage = nullptr;
				VkDeviceMemory colorImageMemory = nullptr;
				VkImageView colorImageView = nullptr;

				VkImage depthImage = nullptr;
				VkDeviceMemory depthImageMemory = nullptr;
				VkImageView depthImageView = nullptr;

				std::vector<VkBuffer> UniformBuffers;
				std::vector<VkDeviceMemory> uniformBuffersMemory;

				VkDescriptorPool descriptorPool = nullptr;
				VkDescriptorSetLayout descriptorSetLayout = nullptr;
				std::vector<VkDescriptorSet> descriptorSets;

				VkSemaphore imageAvailableSemaphore = nullptr;
				VkSemaphore renderFinishedSemaphore = nullptr;
				std::vector<VkSemaphore> imageAvailableSemaphores;
				std::vector<VkSemaphore> renderFinishedSemaphores;
				std::vector<VkFence> inFlightFences;

				VkPipeline graphicsPipeline = nullptr;

				VkQueue graphicsQueue = nullptr;
				VkQueue presentQueue = nullptr;

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
				const std::string MODEL_PATH = "E:/Projects/Dynamik Engine/Dynamik/core assets/models/chalet.obj";
				const std::string TEXTURE_PATH = "E:/Projects/Dynamik Engine/Dynamik/core assets/textures/chalet.jpg";
			};
		}
	}
}
