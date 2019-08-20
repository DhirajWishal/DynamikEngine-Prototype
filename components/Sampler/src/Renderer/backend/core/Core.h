#pragma once

#include "smpafx.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class Core {
			public:
				Core() {}
				virtual ~Core() {}

				virtual void initCore() {}
				virtual void drawFrame() {}
				virtual void shutdownCore() {}
				virtual void clear() {}

				VkInstance getInstance() { return instance; }
				VkInstance* getInstanceAddr() { return &instance; }
				VkDevice getDevice() { return device; }
				VkDevice* getDeviceAddr() { return &device; }
				VkDebugUtilsMessengerEXT getdebugMessenger() { return debugMessenger; }
				VkDebugUtilsMessengerEXT* getdebugMessengerAddr() { return &debugMessenger; }

			protected:
				virtual void init() {}
				virtual void clear() {}

				GLFWwindow* windowsWindow = nullptr;

				VkInstance instance = VK_NULL_HANDLE;
				VkDevice device = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkSurfaceKHR surface = VK_NULL_HANDLE;
				VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
				VkSwapchainKHR swapchain = VK_NULL_HANDLE;
				VkSurfaceCapabilitiesKHR surfaceCapabilities;

				VkExtent2D swapChainExtent;
				VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
				std::vector<VkImage> swapChainImages;
				std::vector<VkImageView> swapChainImageViews;

				VkPipeline graphicsPipeline;
				VkPipelineLayout pipelineLayout;

				VkQueue graphicsQueue;

				VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

				bool frameBufferResizedEvent = false;
				unsigned int WIDTH = 1280;
				unsigned int HEIGHT = 720;
			};
		}
	}
}
