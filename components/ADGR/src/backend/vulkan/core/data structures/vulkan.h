#pragma once

#ifndef DMK_ADGR_VULKAN_H
#define DMK_ADGR_VULKAN_H

#include <vulkan/vulkan.h>
#include <vector>

#include "backend/vulkan/core/data structures/DMK_ADGR_DataStructures.h"
#include "backend/vulkan/core/data types/typenames.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			/* SWAPCHAIN DATA CONTAINER */
			struct ADGRVulkanSwapchainDataContainer {
				VkFormat swapchainImageFormat = VK_FORMAT_UNDEFINED;				// Vulkan swapchain image format
				VkExtent2D swapChainExtent = {										// Vulkan swapchain extent 2D
					VK_NULL_HANDLE,	// width
					VK_NULL_HANDLE	// height
				};
				std::vector<VkImageView> swapchainImageViews = {};					// Vulkan swapchain image views
				std::vector<VkImage> swapChainImages = {};							// Vulkan swapchain images
			};

			/* PIPELINE DATA CONTAINER */
			struct ADGRVulkanPipelineDataContainer {
				VkRenderPass renderPass = VK_NULL_HANDLE;							// Vulkan render pass
				std::vector<DMK_ShaderCode> shaderCodes = {							// Vulkan shader codes
					{},	// vertex shader
					{},	// tessellation shader
					{},	// geometry shader
					{}	// fragment shader
				};
				std::vector<VkShaderModule> shaderModules = {						// Vulkan shader modules
					VK_NULL_HANDLE,	// vertex shader
					VK_NULL_HANDLE,	// tessellation shader
					VK_NULL_HANDLE,	// geometry shader
					VK_NULL_HANDLE	// fragment shader
				};
				std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfo = {	// Vulkan shader stage create info
					{},	// vertex shader
					{},	// tessellation shader
					{},	// geometry shader
					{}	// fragment shader
				};
				VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;					// Vulkan pipeline layout
				VkPipeline pipeline = VK_NULL_HANDLE;								// Vulkan pipeline
			};

			/* COMMAND BUFFER CONTAINER */
			struct ADGRVulkanCommandBufferDataContainer {
				VkCommandPool commandPool = VK_NULL_HANDLE;							// Vulkan command pool
				std::vector<VkCommandBuffer> buffers = {};							// Vulkan command buffers
			};

			/* COLOR BUFFER CONTAINER */
			struct ADGRVulkanColorBufferDataContainer {
				VkImageView imageView = VK_NULL_HANDLE;								// Vulkan color image view
				VkImage image = VK_NULL_HANDLE;										// Vulkan color image
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;						// Vulkan color image memory
			};

			/* DEPTH BUFFER CONTAINER */
			struct ADGRVulkanDepthBufferDataContainer {
				VkImageView imageView = VK_NULL_HANDLE;								// Vulkan depth image view
				VkImage image = VK_NULL_HANDLE;										// Vulkan depth image
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;						// Vulkan depth image memory
			};

			/* FRAME BUFFER CONTAINER */
			struct ADGRVulkanFrameBufferDataContainer {
				std::vector<VkFramebuffer> buffers = {};							// Vulkan frame buffers
			};

			/* DESCRIPTOR SETS CONATAINER */
			struct ADGRVulkanDescriptorSetsDataContainer {
				VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;			// Vulkan descriptor set layout
				VkDescriptorPool descriptorPool = VK_NULL_HANDLE;					// Vulkan descriptor pool
				std::vector<VkDescriptorSet> descriptorSets = {};					// Vulkan descriptor sets
			};

			/* SEMAPHORES AND FENCES CONTAINER */
			struct ADGRVulkanSemaphoresAndFencesDataContainer {
				std::vector<VkSemaphore> imageAvailableSemaphore = {};				// Vulkan image available semaphores
				std::vector<VkSemaphore> renderFinishedSemaphore = {};				// Vulkan render finished semaphores
				std::vector<VkFence> inFlightFence = {};							// Vulkan in-flight fences
			};

			/* DATA CONTAINER */
			struct ADGRVulkanDataContainer {
				/* WINDOW */
				GLFWwindow* window = nullptr;										// GLFW Window

				/* VULKAN CORE */
				VkInstance instance = VK_NULL_HANDLE;								// Vulkan instance
				VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;			// Vulkan debug messenger
				VkSurfaceKHR surface = VK_NULL_HANDLE;								// Vulkan GLFW window surface
				VkSurfaceCapabilitiesKHR surfaceCapabilities = {};					// Vulkan GLFW window surfce capabilities
				VkDevice device = VK_NULL_HANDLE;									// Vulkan device
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;					// Vulkan physical device

				/* SWAPCHAIN */
				ADGRVulkanSwapchainDataContainer* swapchainContainer = nullptr;			// ADGR swapchain container

				/* PIPELINE */
				std::vector<ADGRVulkanPipelineDataContainer*> pipelineContainers = {};	// ADGR pipeline containers

				/* COMMAND BUFFER */
				ADGRVulkanCommandBufferDataContainer* commandBufferContainer = nullptr;	// ADGR command buffer container		

				/* COLOR BUFFER */
				ADGRVulkanColorBufferDataContainer* colorBufferContainer = nullptr;		// ADGR color buffer container

				/* DEPTH BUFFER */
				ADGRVulkanDepthBufferDataContainer* depthBufferContainer = nullptr;		// ADGR depth buffer container

				/* FRAME BUFFER */
				ADGRVulkanFrameBufferDataContainer* frameBufferContainer = nullptr;		// ADGR frame buffer container

				/* DESCRIPTOR SETS */
				std::vector<ADGRVulkanDescriptorSetsDataContainer*> descriptorSetsContainers = {};	// ADGR descriptor set container

				/* SEMAPHORES AND FENCES */
				ADGRVulkanSemaphoresAndFencesDataContainer* semaphoresAndFencesContainer = {};	// ADGR semaphores and fences container

				/* BLANK SCREEN BACKGROUND COLOR */
				std::vector<float> clearScreenValues = {								// Vulkan clear screen values
				(2.0f / 256.0f),		// Red		
				(8.0f / 256.0f),		// Green	
				(32.0f / 256.0f),		// Blue		
				(1.00000000f)			// Alpha	
				};	// Dynwamik color code: rgba(2, 8, 32, 1)

				/* QUEUES */
				VkQueue graphicsQueue = VK_NULL_HANDLE;									// Vulkan graphics queue
				VkQueue presentQueue = VK_NULL_HANDLE;									// Vulkan present queue

				/* MSAA BITS */
				VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;				// Vulkan msaa sample bits

				/* CORE EVENTS */
				bool frameBufferResizedEvent = false;									// Vulkan GLFW frame buffer resized event
				unsigned int WIDTH = 0;													// Vulkan GLFW window width
				unsigned int HEIGHT = 0;												// Vulkan GLFW window height

				/* CORE MIP LEVELS */
				float minMipLevel = 0.0f;												// Vulkan minimum mipmap level
				float maxMipLevel = 0.0f;												// Vulkan maximum mipmap level
			};
		}
	}
}

#endif
