#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#if 0
#ifdef DMK_PLATFORM_WINDOWS
#ifdef DMK_ADGR_USE_VULKAN

#elif defined(DMK_ADGR_USE_DX12)

#elif defined(DMK_ADGR_USE_OPENGL)

#else
#error What are you?
#endif

#endif
#endif

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
#include "core/core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class core {
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
				GLFWwindow* window;

				VkInstance myInstance = VK_NULL_HANDLE;
				VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
				VkPhysicalDevice myPhysicalDevice = VK_NULL_HANDLE;
				VkDevice myDevice = VK_NULL_HANDLE;
				VkSurfaceKHR mySurface = VK_NULL_HANDLE;
				VkSwapchainKHR mySwapchain = VK_NULL_HANDLE;
				VkRenderPass myRenderPass = VK_NULL_HANDLE;
				VkPipelineLayout myPipelineLayout = VK_NULL_HANDLE;
				VkCommandPool myCommandPool = VK_NULL_HANDLE;

				VkBuffer VertexBuffer = VK_NULL_HANDLE;
				VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
				VkBuffer IndexBuffer = VK_NULL_HANDLE;
				VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;

				VkSemaphore imageAvailableSemaphore = VK_NULL_HANDLE;
				VkSemaphore renderFinishedSemaphore = VK_NULL_HANDLE;
				std::vector<VkSemaphore> imageAvailableSemaphores;
				std::vector<VkSemaphore> renderFinishedSemaphores;
				std::vector<VkFence> inFlightFences;

				VkPipeline graphicsPipeline = VK_NULL_HANDLE;

				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;

				std::vector<VkImage> swapChainImages;
				VkFormat swapChainImageFormat;
				VkExtent2D swapChainExtent;
				std::vector<VkImageView> swapChainImageViews;

				Buffer myMainBuffer;

				instance instance{ &myInstance };
				device device{ &myDevice, &myPhysicalDevice, &mySurface };
				debugger debugger{ &myInstance, &debugMessenger };
				swapChain swapchain{ &myDevice, &mySwapchain, &myPhysicalDevice, &mySurface };
				frameBuffer swapchainFrameBuffers;
				pipeline pipeline{ &myDevice, &myRenderPass, &graphicsPipeline };
				commandBuffer commandBuffer{ &myDevice, &myPhysicalDevice, &myCommandPool };
				vertexBuffer vertexBuffer{ &VertexBuffer, &vertexBufferMemory, NULL };
				indexBuffer indexBuffer{ &IndexBuffer, &indexBufferMemory };

				uint32 currentFrame = 0;
				bool frameBufferResized = false;

				std::string vertexShaderPath = "shaders/vert.spv";
				std::string fragmentShaderPath = "shaders/frag.spv";
			};
		}
	}
}
