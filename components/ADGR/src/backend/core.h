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

#include "Platform.h"

#include "core/utils/DMK_DataTypes.h"
#include "backend/interface.h"

#include "keyEvent.h"
#include "mouseEvent.h"
#include "applicationEvent.h"

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
				//static core& getInstance() {
				//	static core curr;
				//	return curr;
				//}

				void initWindow();

				GLFWwindow* getWindow() { return window; }
				VkDevice getDevice() { return myDevice; }

				static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

				// helper functions;

				void setMipLevels(float);

				using eventCallbackFunction = std::function<void(Event&)>;

				struct Events {
					eventCallbackFunction callbackFunc;
				} eventData;

				void eventCallbackFunc(Event& event);

				int dumbshit = 10;
			private:
				static void onKeyEvent(GLFWwindow* window, int keycode, int scancode,
					int action, int mods);

				void onEvent(Event& event);

				GLFWwindow* window;

				VkInstance myInstance = VK_NULL_HANDLE;
				VkDevice myDevice = VK_NULL_HANDLE;
				VkPhysicalDevice myPhysicalDevice = VK_NULL_HANDLE;
				VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
				VkSurfaceKHR mySurface = VK_NULL_HANDLE;
				VkSwapchainKHR mySwapchain = VK_NULL_HANDLE;
				VkRenderPass myRenderPass = VK_NULL_HANDLE;
				VkPipelineLayout myPipelineLayout = VK_NULL_HANDLE;
				VkCommandPool myCommandPool = VK_NULL_HANDLE;

				VkBuffer VertexBuffer = VK_NULL_HANDLE;
				VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
				VkBuffer IndexBuffer = VK_NULL_HANDLE;
				VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
				VkBuffer StagingBuffer = VK_NULL_HANDLE;
				VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

				uint32 mipLevels = 0;
				VkImage textureImage = VK_NULL_HANDLE;
				VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
				VkImageView textureImageView = VK_NULL_HANDLE;
				VkSampler textureSampler = VK_NULL_HANDLE;
				VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
				VkImage colorImage = VK_NULL_HANDLE;
				VkDeviceMemory colorImageMemory = VK_NULL_HANDLE;
				VkImageView colorImageView = VK_NULL_HANDLE;

				VkImage depthImage = VK_NULL_HANDLE;
				VkDeviceMemory depthImageMemory = VK_NULL_HANDLE;
				VkImageView depthImageView = VK_NULL_HANDLE;

				std::vector<VkBuffer> UniformBuffers;
				std::vector<VkDeviceMemory> uniformBuffersMemory;

				VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
				VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
				std::vector<VkDescriptorSet> descriptorSets;

				VkSemaphore imageAvailableSemaphore = VK_NULL_HANDLE;
				VkSemaphore renderFinishedSemaphore = VK_NULL_HANDLE;
				std::vector<VkSemaphore> imageAvailableSemaphores;
				std::vector<VkSemaphore> renderFinishedSemaphores;
				std::vector<VkFence> inFlightFences;

				VkPipeline graphicsPipeline = VK_NULL_HANDLE;

				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;

				std::vector<VkImage> swapChainImages;
				VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
				VkExtent2D swapChainExtent = { 0, 0 };
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

				std::unique_ptr<windows::Window> myWindow;

				uint32 currentFrame = 0;
				bool frameBufferResized = false;

				std::vector<std::vector<std::string>> shaderPath;
				std::vector<std::vector<std::string>> assetsPath;

				const std::string vertexShaderPath = "E:/Projects/Dynamik Engine/Dynamik/components/Shaders/vert.spv";
				const std::string fragmentShaderPath = "E:/Projects/Dynamik Engine/Dynamik/components/Shaders/frag.spv";
				std::string MODEL_PATH = "E:/Projects/Dynamik Engine/Dynamik/core assets/models/chalet.obj";
				std::string TEXTURE_PATH = "E:/Projects/Dynamik Engine/Dynamik/core assets/textures/chalet.jpg";

				windows::Window myWindowHandler;

				struct coreProps {
					std::string title = "Dynamik Engine";
					unsigned int width = 1280;
					unsigned int height = 720;


				}myProps;
			};

			//class CoreProps {
			//	core* myRef;
			//	
			//public:
			//	//static void init(core* core) { current.myRef = core; }
			//
			//
			//	static CoreProps& getInstance() {
			//		static CoreProps current;
			//		//if (&current == nullptr) current = new CoreProps;
			//		return current;
			//	}
			//	void key(int keycode, int action, core* core) {
			//		printf("%d", keycode);
			//
			//		switch (action) {
			//		case GLFW_PRESS: {
			//			KeyPressedEvent event(keycode, 0);
			//			core->eventCallbackFunc(event);
			//
			//			break;
			//		}
			//		case GLFW_RELEASE: {
			//			KeyReleasedEvent event(keycode);
			//			core->eventCallbackFunc(event);
			//
			//			break;
			//		}
			//		case GLFW_REPEAT: {
			//			KeyPressedEvent event(keycode, 1);
			//			core->eventCallbackFunc(event);
			//
			//			break;
			//		}
			//		}
			//	}
			//};
		}
	}
}
