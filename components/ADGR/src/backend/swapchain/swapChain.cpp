/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "swapChain.h"
#include "backend/buffers/indexBuffer.h"
#include "backend/queues/queues.h"
#include "backend/texture/texture.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			swapChain::swapChain(VkDevice* device, VkSwapchainKHR* swapChain,
				VkPhysicalDevice* physicalDevice, VkSurfaceKHR* surface) :
				myDevice(device), mySwapChain(swapChain),
				myPhysicalDevice(physicalDevice), mySurface(surface) {
			}

			void swapChain::initSwapChain(GLFWwindow& window, std::vector<VkImage>* swapChainImages,
				VkFormat* swapChainImageFormat, VkExtent2D* swapChainExtent) {
				swapChainSupportDetails swapChainSupport = querySwapChainSupport(myPhysicalDevice, mySurface);

				VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
				VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
				VkExtent2D extent = chooseSwapExtent(window, swapChainSupport.capabilities);

				uint32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
				if (swapChainSupport.capabilities.maxImageCount > 0
					&& imageCount > swapChainSupport.capabilities.maxImageCount)
					imageCount = swapChainSupport.capabilities.maxImageCount;

				VkSwapchainCreateInfoKHR createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				createInfo.surface = *mySurface;
				createInfo.minImageCount = imageCount;
				createInfo.imageFormat = surfaceFormat.format;
				createInfo.imageColorSpace = surfaceFormat.colorSpace;
				createInfo.imageExtent = extent;
				createInfo.imageArrayLayers = 1;
				createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				//createInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

				queueFamilyindices indices = Dynamik::ADGR::core::findQueueFamilies(*myPhysicalDevice, *mySurface);
				uint32 queueFamilyindices[] = {
					indices.graphicsFamily.value(),
					indices.presentFamily.value()
				};

				if (indices.graphicsFamily != indices.presentFamily) {
					createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
					createInfo.queueFamilyIndexCount = 2;
					createInfo.pQueueFamilyIndices = queueFamilyindices;
				}
				else {
					createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
					createInfo.queueFamilyIndexCount = 0;
					createInfo.pQueueFamilyIndices = nullptr;
				}

				createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
				createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
				createInfo.presentMode = presentMode;
				createInfo.clipped = VK_TRUE;
				createInfo.oldSwapchain = VK_NULL_HANDLE;

				if (vkCreateSwapchainKHR(*myDevice, &createInfo, nullptr, mySwapChain))
					throw std::runtime_error("Failed to create Swap Chain!");

				vkGetSwapchainImagesKHR(*myDevice, *mySwapChain, &imageCount, nullptr);
				swapChainImages->resize(imageCount);
				vkGetSwapchainImagesKHR(*myDevice, *mySwapChain, &imageCount, swapChainImages->data());

				*swapChainImageFormat = surfaceFormat.format;
				*swapChainExtent = extent;
			}

			void swapChain::deleteSwapChain() {
				vkDestroySwapchainKHR(*myDevice, *mySwapChain, nullptr);
			}

			void swapChain::initImageViews(std::vector<VkImageView>* swapChainImageViews,
				std::vector<VkImage> swapChainImages, VkFormat swapChainImageFormat, uint32 mipLevels) {
				swapChainImageViews->resize(swapChainImages.size());

				for (uint32_t i = 0; i < swapChainImages.size(); i++) {
					swapChainImageViews->at(i) = createImageView(*myDevice, swapChainImages[i], swapChainImageFormat,
						VK_IMAGE_ASPECT_COLOR_BIT, 1);
				}
			}

			void swapChain::deleteImageViews(std::vector<VkImageView>* swapChainImageViews) {
				for (auto imageView : *swapChainImageViews)
					vkDestroyImageView(*myDevice, imageView, nullptr);
			}

			void swapChain::cleanUp(VkDevice device, std::vector<VkFramebuffer> swapChainFramebuffers,
				VkCommandPool commandPool, std::vector<VkCommandBuffer> commandBuffers, VkPipeline graphicsPipeline,
				VkPipelineLayout pipelineLayout, VkRenderPass renderPass, std::vector<VkImageView> swapChainImageViews,
				std::vector<VkImage> swapChainImages, std::vector<VkBuffer> uniformBuffers,
				std::vector<VkDeviceMemory> uniformBuffersMemory, VkDescriptorPool descriptorPool) {
				for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
					vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);

				vkFreeCommandBuffers(device, commandPool, static_cast<uint32>(commandBuffers.size()),
					commandBuffers.data());

				vkDestroyPipeline(device, graphicsPipeline, nullptr);
				vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
				vkDestroyRenderPass(device, renderPass, nullptr);

				for (size_t i = 0; i < swapChainImageViews.size(); i++)
					vkDestroyImageView(device, swapChainImageViews[i], nullptr);

				vkDestroySwapchainKHR(device, *mySwapChain, nullptr);

				for (size_t i = 0; i < swapChainImages.size(); i++) {
					vkDestroyBuffer(device, uniformBuffers[i], nullptr);
					vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
				}

				vkDestroyDescriptorPool(device, descriptorPool, nullptr);
			}

			swapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
				swapChainSupportDetails details;
				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

				uint32 formatCount;
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

				if (formatCount != 0) {
					details.formats.resize(formatCount);
					vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
				}

				uint32 presentModeCount;
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

				if (presentModeCount != 0) {
					details.presentModes.resize(presentModeCount);
					vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
						&presentModeCount, details.presentModes.data());
				}

				return details;
			}

			swapChainSupportDetails swapChain::querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface) {
				swapChainSupportDetails details;
				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*device, *surface, &details.capabilities);

				uint32 formatCount;
				vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, nullptr);

				if (formatCount != 0) {
					details.formats.resize(formatCount);
					vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, details.formats.data());
				}

				uint32 presentModeCount;
				vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &presentModeCount, nullptr);

				if (presentModeCount != 0) {
					details.presentModes.resize(presentModeCount);
					vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface,
						&presentModeCount, details.presentModes.data());
				}

				return details;
			}

			VkSurfaceFormatKHR swapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
				for (const auto& availableFormat : availableFormats)
					if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM
						&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
						return availableFormat;

				return availableFormats[0];
			}

			VkPresentModeKHR swapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
				VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

				for (const auto& availablePresentMode : availablePresentModes) {
					if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
						return availablePresentMode;
					}
					else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
						bestMode = availablePresentMode;
				}

				return bestMode;
			}

			VkExtent2D swapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
				if (capabilities.currentExtent.width != std::numeric_limits<uint32>::max())
					return capabilities.currentExtent;
				else {
					VkExtent2D actualExtent = {
						WIDTH,
						HEIGHT
					};

					actualExtent.width = std::max(capabilities.minImageExtent.width,
						std::min(capabilities.maxImageExtent.width, actualExtent.width));
					actualExtent.height = std::max(capabilities.minImageExtent.height,
						std::min(capabilities.maxImageExtent.height, actualExtent.height));

					return actualExtent;
				}
			}

			VkExtent2D swapChain::chooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities) {
				if (capabilities.currentExtent.width != std::numeric_limits<uint32>::max())
					return capabilities.currentExtent;
				else {
					int width, height;
					glfwGetFramebufferSize(&window, &width, &height);

					VkExtent2D actualExtent = {
						static_cast<uint32>(width),
						static_cast<uint32>(height)
					};

					actualExtent.width = std::max(capabilities.minImageExtent.width,
						std::min(capabilities.maxImageExtent.width, actualExtent.width));
					actualExtent.height = std::max(capabilities.minImageExtent.height,
						std::min(capabilities.maxImageExtent.height, actualExtent.height));

					return actualExtent;
				}
			}

			swapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface) {
				swapChainSupportDetails details;
				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*device, *surface, &details.capabilities);

				uint32 formatCount;
				vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, nullptr);

				if (formatCount != 0) {
					details.formats.resize(formatCount);
					vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, details.formats.data());
				}

				uint32 presentModeCount;
				vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &presentModeCount, nullptr);

				if (presentModeCount != 0) {
					details.presentModes.resize(presentModeCount);
					vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface,
						&presentModeCount, details.presentModes.data());
				}

				return details;
			}
		}
	}
}
