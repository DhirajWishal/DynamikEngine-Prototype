#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_CORE_H
#define _DYNAMIK_ADGR_VULKAN_CORE_H

#include <GLFW/glfw3.h>
#include "VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanInstanceInitInfo {
				std::string applicationName = "Dynamik Engine";
				std::string engineName = "Dynamik";
			};

			struct ADGRVulkanCoreInitDescriptor {
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

			struct ADGRVulkanCommandBufferInitInfo {
				ARRAY<VulkanRenderableObject> objects;
				ARRAY<F32> clearValues = { 
					(2.0f / 256.0f),
					(8.0f / 256.0f),
					(32.0f / 256.0f),
					(1.00000000f) };
				F32 depthStencilDepth = 1.0f;
				UI32 stencilIndex = 0;

				UI32 count = 3;
			};

			class VulkanCore {
			public:
				VulkanCore() {}
				~VulkanCore() {}

				void initialize(ADGRVulkanCoreInitDescriptor initInfo);
				void terminate();

				void initializeInstance(ADGRVulkanInstanceInitInfo info);
				void initializeSurface(POINTER<GLFWwindow*> windowPtr);
				void terminateInstance();

				void initializeDebugger();
				void terminateDebugger();

				void initializeDevice();
				void terminateDevice();

				void initializeCommandPool();
				void terminateCommandPool();

				void initializeCommandBuffers(ADGRVulkanCommandBufferInitInfo info);
				void terminateCommandBuffers();

				void initializeSyncObjects();
				void terminateSyncObjects();

				static ADGRVulkanQueue findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

				const VkSurfaceKHR getSurface() const { return surface; }
				const VkSurfaceCapabilitiesKHR getSurfaceCapabilities() const { return surfaceCapabilities; }

				const VkDevice getLogicalDevice() const { return logicalDevice; }
				const POINTER<VkDevice> getLogicalDeviceAddr() const { return &logicalDevice; }

				const VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
				const POINTER<VkPhysicalDevice> getPlysicalDeviceAddr() const { return &physicalDevice; }

				const VkCommandPool getCommandPool() const { return commandPool; }
				const POINTER<VkCommandPool> getCommandPoolAddr() const { return &commandPool; }

				const VkQueue getGraphicsQueue() const { return graphicsQueue; }
				const POINTER<VkQueue> getGraphicsQueueAddr() const { return &graphicsQueue; }

				const VkQueue getPresentQueue() const { return presentQueue; }
				const POINTER<VkQueue> getPresentQueueAddr() const { return &presentQueue; }

				const VkSampleCountFlagBits getMsaaSamples() const { return msaaSamples; }

			public:
				/* DRAW CALLS */
				virtual void syncFence(UI32 frame);
				virtual VkResult getNextImage(VkSwapchainKHR swapChain, POINTER<UI32> index, UI32 frame);
				virtual VkResult submitQueues(VkSwapchainKHR swapChain, UI32 index, UI32 frame);

			private:
				void populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

				void initPhysicalDevice();
				void initLogicalDevice();

				VkResult result;
				VkSubmitInfo submitInfo = {};
				VkPresentInfoKHR presentInfo = {};
				VkSemaphore waitSemaphores[1];
				VkPipelineStageFlags waitStages[1];
				VkSemaphore signalSemaphores[1];

				VkInstance instance = VK_NULL_HANDLE;
				VkSurfaceKHR surface = VK_NULL_HANDLE;
				VkSurfaceCapabilitiesKHR surfaceCapabilities = {};

				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

				VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

				VkCommandPool commandPool = VK_NULL_HANDLE;
				ARRAY<VkCommandBuffer> commandBuffers;

				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;

				VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

				UI32 minMipLevel = 0;
				UI32 maxMipLevel = 0;

				ARRAY<VkSemaphore> imageAvailables;
				ARRAY<VkSemaphore> renderFinishes;
				ARRAY<VkFence> inFlightFences;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_CORE_H
