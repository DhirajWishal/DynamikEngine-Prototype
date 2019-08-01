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
#include "buffers/commandBuffer.h"
#include "buffers/frameBuffer.h"
#include "buffers/indexBuffer.h"
#include "buffers/vertexBuffer.h"
#include "device/device.h"
#include "extensions/extensions.h"
#include "instance/instance.h"
#include "pipeline/pipeline.h"
#include "queues/queues.h"
#include "swapchain/swapChain.h"
#include "validators/debugger/debugger.h"
#include "validators/validators.h"

#include "others/semaphores.h"
#include "others/syncObjects.h"
#include "defines.h"

#include "Platform/Windows.h"

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
				void recreateSwapChain();
				void drawFrame();

				void initWindow();

				GLFWwindow* getWindow() { return window; }
				VkDevice getDevice() { return myDevice; }

				static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

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
				indexBuffer indexBuffer{ &myDevice, &IndexBuffer, &indexBufferMemory };

				uint32 currentFrame = 0;
				bool frameBufferResized = false;

				std::string vertexShaderPath = "E:/Projects/Dynamik Engine/Dynamik/components/Shaders/vert.spv";
				std::string fragmentShaderPath = "E:/Projects/Dynamik Engine/Dynamik/components/Shaders/frag.spv";
			};
		}
	}
}
