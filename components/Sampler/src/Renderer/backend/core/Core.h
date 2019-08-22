#pragma once

#include "smpafx.h"

#include "Renderer/backend/controlHeader.h"
#include "Renderer/backend/data types/typenames.h"

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
				VkRenderPass renderPass;

				VkDescriptorSetLayout descriptorSetLayout;
				VkDescriptorPool descriptorPool;
				std::vector<VkDescriptorSet> descriptorSets;

				VkCommandPool commandPool;
				std::vector<VkCommandBuffer> commandBuffers;

				std::vector<VkSemaphore> imageAvailableSemaphore;
				std::vector<VkSemaphore> renderFinishedSemaphore;
				std::vector<VkFence> inFlightFence;

				VkExtent2D swapChainExtent;
				VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
				std::vector<VkImage> swapChainImages;
				std::vector<VkImageView> swapChainImageViews;

				std::vector<VkFramebuffer> frameBuffers;

				VkPipeline graphicsPipeline;
				VkPipelineLayout pipelineLayout;

				VkQueue graphicsQueue;

				VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

				VkImage colorImage = VK_NULL_HANDLE;
				VkDeviceMemory colorImageMemory = VK_NULL_HANDLE;
				VkImageView colorImageView = VK_NULL_HANDLE;

				VkImage depthImage = VK_NULL_HANDLE;
				VkDeviceMemory depthImageMemory = VK_NULL_HANDLE;
				VkImageView depthImageView = VK_NULL_HANDLE;

				VkImage textureImage = VK_NULL_HANDLE;
				VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
				VkImageView textureImageView = VK_NULL_HANDLE;
				VkSampler textureSampler = VK_NULL_HANDLE;

				uint32_t mipLevels = 0;

				bool frameBufferResizedEvent = false;
				unsigned int WIDTH = 1280;
				unsigned int HEIGHT = 720;

				float minMipLevel;
				float maxMipLevel;

				DMK_ShaderCode vertexShaderCode;
				VkShaderModule vertexShaderModule;
				VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};

				DMK_ShaderCode fragmentShaderCode;
				VkShaderModule fragmentShaderModule;
				VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};

#ifdef USE_SHADER_TESSELLATION
				DMK_ShaderCode tessellationShaderCode;
				VkShaderModule tessellationShaderModule;
				VkPipelineShaderStageCreateInfo tessellationShaderStageInfo = {};
#endif

#ifdef USE_SHADER_GEOMETRY
				DMK_ShaderCode geometryShaderCode;
				VkShaderModule geometryShaderModule;
				VkPipelineShaderStageCreateInfo geometryShaderStageInfo = {};
#endif
			};
		}
	}
}
