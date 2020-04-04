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

			struct ADGRVulkanRenderPassInitInfo {
				B1 enableDepthAttachment = true;

				UI32 destinationSubpass = 0;
				VkAccessFlags accessFlags = 0;

				ARRAY<VkSubpassDescription> additionalSubPasses;
				ARRAY<VkSubpassDependency> additionalSubPassDependencies;
			};

			struct ADGRVulkanFrameBufferInitInfo {
				ARRAY<VkImageView> preAttachments;
				ARRAY<VkImageView> additionalAttachments;
			};

			struct ADGRVulkanCoreInitDescriptor {
				ADGRVulkanInstanceInitInfo instanceInitInfo;
				ADGRVulkanRenderPassInitInfo renderPassInitInfo;
				ADGRVulkanFrameBufferInitInfo frameBufferInitInfo;

				UI32 width = 0;
				UI32 height = 0;

				POINTER<GLFWwindow*> windowPointer;
			};

			struct VulkanSwapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities = {};
				ARRAY<VkSurfaceFormatKHR> formats = {};
				ARRAY<VkPresentModeKHR> presentModes = {};
			};

			struct ADGRVulkanQueue {
				std::optional<UI32> graphicsFamily;
				std::optional<UI32> presentFamily;

				B1 isComplete();
			};

			class VulkanCore {
			public:
				VulkanCore() {}
				virtual ~VulkanCore() {}

				virtual void initialize(ADGRVulkanCoreInitDescriptor initInfo);
				virtual void terminate();

				virtual void initializeInstance(ADGRVulkanInstanceInitInfo info);
				virtual void initializeSurface(POINTER<GLFWwindow*> windowPtr);
				virtual void terminateInstance();

				virtual void initializeDebugger();
				virtual void terminateDebugger();

				virtual void initializeDevice();
				virtual void terminateDevice();

				virtual void initializeSwapChain(UI32 width, UI32 height);
				virtual void terminateSwapChain();

				virtual void initializeCommandPool();
				virtual void terminateCommandPool();

				virtual void initializeRenderPass(ADGRVulkanRenderPassInitInfo info);
				virtual void terminateRenderPass();

				virtual void initializeFrameBuffer(ADGRVulkanFrameBufferInitInfo info);
				virtual void terminateFrameBuffer();

				virtual void initializeCommandBuffers(ARRAY<VulkanRenderableObject> objects);
				virtual void terminateCommandBuffers();

				virtual void initializeSyncObjects();
				virtual void terminateSyncObjects();

				static VulkanSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);
				static ADGRVulkanQueue findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

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

				const ARRAY<VkImage> getSwapChainImages() const { return swapChainImages; }
				const ARRAY<VkImageView> getSwapChainImageViews() const { return swapChainImageViews; }
				const VkExtent2D getSwapChainExtent() const { return swapChainExtent; }
				const VkFormat getSwapChainImageFormat() const { return swapChainImageFormat; }

				const VkSampleCountFlagBits getMsaaSamples() const { return msaaSamples; }

				const VkRenderPass getRenderPass() const { return renderPass; }

			public:
				/* DRAW CALLS */
				virtual void syncFence(UI32 frame);
				virtual VkResult getNextImage(POINTER<UI32> index);
				virtual VkResult submitQueues(UI32 index, UI32 frame);

			private:
				void populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

				void initPhysicalDevice();
				void initLogicalDevice();

				void initializeSwapChainImageViews();

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

				VkSwapchainKHR swapChain = VK_NULL_HANDLE;
				ARRAY<VkImage> swapChainImages;
				ARRAY<VkImageView> swapChainImageViews;
				VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
				VkExtent2D swapChainExtent = { 0, 0 };

				VkCommandPool commandPool = VK_NULL_HANDLE;
				ARRAY<VkCommandBuffer> commandBuffers;

				VkRenderPass renderPass = VK_NULL_HANDLE;

				ARRAY<VkFramebuffer> frameBuffers;

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
