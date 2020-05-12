#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_GRAPHICS_CORE_H
#define _DYNAMIK_RENDERER_VULKAN_GRAPHICS_CORE_H

#include <GLFW/glfw3.h>
#include "VulkanGraphicsContainers.h"
#include "VulkanGraphicsSwapChain.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanRenderData;
			class VulkanGraphicsSwapChain;
			struct VulkanSurfaceContainer;

			struct VulkanInstanceInitInfo {
				std::string applicationName = "Dynamik Engine";
				std::string engineName = "Dynamik";
			};

			struct VulkanQueue {
				std::optional<UI32> graphicsFamily;
				std::optional<UI32> presentFamily;

				B1 isComplete();
			};

			class VulkanGraphicsCore {
			public:
				VulkanGraphicsCore() {}
				~VulkanGraphicsCore() {}

				void terminate();

				void initializeInstance(VulkanInstanceInitInfo info);
				VulkanSurfaceContainer createSurface(POINTER<GLFWwindow> windowHandle);
				void validateSurface(VkSurfaceKHR surface);
				void getSurfaceCapabilities(POINTER<VulkanSurfaceContainer> container);
				void terminateSurface(VulkanSurfaceContainer container);
				void terminateInstance();

				void initializeDebugger();
				void terminateDebugger();

				void initializeDevice(VkSurfaceKHR baseSurface);
				void terminateDevice();

				void initializeSyncObjects();
				void terminateSyncObjects();

				static VulkanQueue findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

				B1 checkSurfaceCompatibility(VkSurfaceKHR surface);

				VkPhysicalDeviceProperties deviceProperties;

			public:
				/* DRAW CALLS */
				virtual void syncFence(UI32 frame);
				virtual VkResult getNextImage(VkSwapchainKHR swapChain, POINTER<UI32> index, UI32 frame);
				virtual VkResult submitQueues(std::vector<VkSwapchainKHR> swapChains, UI32 index, UI32 frame, std::vector<VkCommandBuffer> buffers);

				void populateDebugMessegerCreateInfo(POINTER<VkDebugUtilsMessengerCreateInfoEXT> createInfo);

				void initPhysicalDevice(VkSurfaceKHR baseSurface);
				void initLogicalDevice(VkSurfaceKHR baseSurface);

				VkInstance instance = VK_NULL_HANDLE;
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

				VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;

				VkSampleCountFlagBits maxMSAASamples = VK_SAMPLE_COUNT_1_BIT;

				UI32 minMipLevel = 0;
				UI32 maxMipLevel = 0;

				std::vector<VkSemaphore> imageAvailables;
				std::vector<VkSemaphore> renderFinishes;
				std::vector<VkFence> inFlightFences;

			private:
				VkResult result;
				VkSubmitInfo submitInfo = {};
				VkPresentInfoKHR presentInfo = {};
				VkSemaphore waitSemaphores[1];
				VkPipelineStageFlags waitStages[1];
				VkSemaphore signalSemaphores[1];
				std::vector<VkCommandBuffer> additionalCommandBuffers;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_CORE_H
