#pragma once

#ifndef _DYNAMIK_VULKAN_RENDERER_CORE_
#define _DYNAMIK_VULKAN_RENDERER_CORE_

#include "backend/vulkan/core/controlHeader.h"
#include "backend/vulkan/core/data types/typenames.h"
#include "data structures/vertex.h"

#include "backend/vulkan/core/functions/textureFunctions.h"

#include "Event.h"

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

				virtual VkInstance getInstance() { return instance; }
				virtual VkInstance* getInstanceAddr() { return &instance; }
				virtual VkDevice getDevice() { return device; }
				virtual VkDevice* getDeviceAddr() { return &device; }
				virtual VkDebugUtilsMessengerEXT getdebugMessenger() { return debugMessenger; }
				virtual VkDebugUtilsMessengerEXT* getdebugMessengerAddr() { return &debugMessenger; }

				virtual void init() {}
				virtual void clear() {}

			protected:
				static GLFWwindow* windowsWindow;
				
				static VkInstance instance;
				static VkDevice device;
				static VkPhysicalDevice physicalDevice;
				static VkSurfaceKHR surface;
				static VkDebugUtilsMessengerEXT debugMessenger;
				static VkSwapchainKHR swapchain;
				static VkSurfaceCapabilitiesKHR surfaceCapabilities;
				static VkRenderPass renderPass;
				
				static VkDescriptorSetLayout descriptorSetLayout;
				static VkDescriptorPool descriptorPool;
				static std::vector<VkDescriptorSet> descriptorSets;
				
				static VkCommandPool commandPool;
				static std::vector<VkCommandBuffer> commandBuffers;
				
				static std::vector<VkSemaphore> imageAvailableSemaphore;
				static std::vector<VkSemaphore> renderFinishedSemaphore;
				static std::vector<VkFence> inFlightFence;
				
				static VkExtent2D swapChainExtent;
				static VkFormat swapChainImageFormat;
				static std::vector<VkImage> swapChainImages;
				static std::vector<VkImageView> swapChainImageViews;
				
				static std::vector<VkFramebuffer> frameBuffers;

				static std::vector<float> clearScreenValues;
				
				static VkPipeline graphicsPipeline;
				static VkPipelineLayout pipelineLayout;
				
				static VkQueue graphicsQueue;
				static VkQueue presentQueue;
				
				static VkSampleCountFlagBits msaaSamples;
				
				static VkImage colorImage;
				static VkDeviceMemory colorImageMemory;
				static VkImageView colorImageView;
				
				static VkImage depthImage;
				static VkDeviceMemory depthImageMemory;
				static VkImageView depthImageView;
				
				static VkImage textureImage;
				static VkDeviceMemory textureImageMemory;
				static VkImageView textureImageView;
				static VkSampler textureSampler;
				
				static uint32_t mipLevels;
				
				static bool frameBufferResizedEvent;
				static unsigned int WIDTH;
				static unsigned int HEIGHT;
				
				static float minMipLevel;
				static float maxMipLevel;
				
				static DMK_ShaderCode vertexShaderCode;
				static VkShaderModule vertexShaderModule;
				static VkPipelineShaderStageCreateInfo vertexShaderStageInfo;
				
#ifdef USE_SHADER_TESSELLATION
				static DMK_ShaderCode tessellationShaderCode;
				static VkShaderModule tessellationShaderModule;
				static VkPipelineShaderStageCreateInfo tessellationShaderStageInfo;
#endif				
#ifdef USE_SHADER_GEOMETRY
				static DMK_ShaderCode geometryShaderCode;
				static VkShaderModule geometryShaderModule;
				static VkPipelineShaderStageCreateInfo geometryShaderStageInfo;
#endif

				static DMK_ShaderCode fragmentShaderCode;
				static VkShaderModule fragmentShaderModule;
				static VkPipelineShaderStageCreateInfo fragmentShaderStageInfo;
			};
		}
	}
}

#endif
