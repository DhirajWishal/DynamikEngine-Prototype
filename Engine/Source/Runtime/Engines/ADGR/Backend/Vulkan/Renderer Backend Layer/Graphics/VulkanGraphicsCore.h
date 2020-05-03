#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_CORE_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_CORE_H

#include <GLFW/glfw3.h>
#include "VulkanGraphicsContainers.h"
#include "VulkanGraphicsSwapChain.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanRenderData;
			class VulkanGraphicsSwapChain;

			struct ADGRVulkanInstanceInitInfo {
				std::string applicationName = "Dynamik Engine";
				std::string engineName = "Dynamik";
			};

			struct ADGRVulkanGraphicsCoreInitDescriptor {
				ADGRVulkanInstanceInitInfo instanceInitInfo;

				UI32 width = 0;
				UI32 height = 0;

				POINTER<GLFWwindow*> windowPointer;
			};

			struct ADGRVulkanQueue {
				std::optional<UI32> graphicsFamily;
				std::optional<UI32> presentFamily;

				B1 isComplete();
			};

			class VulkanGraphicsCore {
			public:
				VulkanGraphicsCore() {}
				~VulkanGraphicsCore() {}

				void initialize(ADGRVulkanGraphicsCoreInitDescriptor initInfo);
				void terminate();

				void initializeInstance(ADGRVulkanInstanceInitInfo info);
				void initializeSurface(POINTER<GLFWwindow> windowPtr);
				void terminateInstance();

				void initializeDebugger();
				void terminateDebugger();

				void initializeDevice();
				void terminateDevice();

				void initializeSyncObjects();
				void terminateSyncObjects();

				static ADGRVulkanQueue findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

			public:
				/* DRAW CALLS */
				virtual void syncFence(UI32 frame);
				virtual VkResult getNextImage(VkSwapchainKHR swapChain, POINTER<UI32> index, UI32 frame);
				virtual VkResult submitQueues(ARRAY<VkSwapchainKHR> swapChains, UI32 index, UI32 frame, ARRAY<VkCommandBuffer> buffers);

				void populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

				void initPhysicalDevice();
				void initLogicalDevice();

				VkInstance instance = VK_NULL_HANDLE;
				VkSurfaceKHR surface = VK_NULL_HANDLE;
				VkSurfaceCapabilitiesKHR surfaceCapabilities = {};

				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

				VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;

				VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

				UI32 minMipLevel = 0;
				UI32 maxMipLevel = 0;

				ARRAY<VkSemaphore> imageAvailables;
				ARRAY<VkSemaphore> renderFinishes;
				ARRAY<VkFence> inFlightFences;

			private:
				VkResult result;
				VkSubmitInfo submitInfo = {};
				VkPresentInfoKHR presentInfo = {};
				VkSemaphore waitSemaphores[1];
				VkPipelineStageFlags waitStages[1];
				VkSemaphore signalSemaphores[1];
				ARRAY<VkCommandBuffer> additionalCommandBuffers;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_CORE_H
