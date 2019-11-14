#include "adgrafx.h"
#include "swapChain.h"

#include "controlHeader.h"
#include "queues/queues.h"

#include "functions/textureFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void swapChain::init(ADGRVulkanDataContainer* container) {
				swapChainSupportDetails swapChainSupport = querySwapChainSupport(&container->physicalDevice, &container->surface);

				VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
				VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
				VkExtent2D extent = chooseSwapExtent(*container->window, swapChainSupport.capabilities);

				VkCompositeAlphaFlagBitsKHR surfaceComposite =
					(container->surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
					? VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
					: (container->surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
					? VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR
					: (container->surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
					? VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR
					: VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;

				uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
				if (swapChainSupport.capabilities.maxImageCount > 0
					&& imageCount > swapChainSupport.capabilities.maxImageCount)
					imageCount = swapChainSupport.capabilities.maxImageCount;

				VkSwapchainCreateInfoKHR createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				createInfo.surface = container->surface;
				createInfo.minImageCount = imageCount;
				createInfo.imageFormat = surfaceFormat.format;
				createInfo.imageColorSpace = surfaceFormat.colorSpace;
				createInfo.imageExtent = extent;
				createInfo.imageArrayLayers = 1;
				createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				//createInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

				queueFamilyindices indices = Dynamik::ADGR::core::findQueueFamilies(container->physicalDevice, container->surface);
				uint32_t queueFamilyindices[] = {
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

				if (vkCreateSwapchainKHR(container->device, &createInfo, nullptr, &container->swapchainContainer.swapchain))
					DMK_CORE_FATAL("Failed to create Swap Chain!");

				vkGetSwapchainImagesKHR(container->device, container->swapchainContainer.swapchain, &imageCount, nullptr);
				container->swapchainContainer.swapChainImages.resize(imageCount);
				vkGetSwapchainImagesKHR(container->device, container->swapchainContainer.swapchain, &imageCount, container->swapchainContainer.swapChainImages.data());

				container->swapchainContainer.swapchainImageFormat = surfaceFormat.format;
				container->swapchainContainer.swapchainExtent = extent;
			}

			swapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
				swapChainSupportDetails details;
				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

				uint32_t formatCount;
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

				if (formatCount != 0) {
					details.formats.resize(formatCount);
					vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
				}

				uint32_t presentModeCount;
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

				uint32_t formatCount;
				vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, nullptr);

				if (formatCount != 0) {
					details.formats.resize(formatCount);
					vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, details.formats.data());
				}

				uint32_t presentModeCount;
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

			VkExtent2D swapChain::chooseSwapExtent(ADGRVulkanDataContainer* container, const VkSurfaceCapabilitiesKHR& capabilities) {
				if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
					return capabilities.currentExtent;
				else {
					VkExtent2D actualExtent = {
						container->WIDTH,
						container->HEIGHT
					};

					actualExtent.width = std::max(capabilities.minImageExtent.width,
						std::min(capabilities.maxImageExtent.width, actualExtent.width));
					actualExtent.height = std::max(capabilities.minImageExtent.height,
						std::min(capabilities.maxImageExtent.height, actualExtent.height));

					return actualExtent;
				}
			}

			VkExtent2D swapChain::chooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities) {
				if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
					return capabilities.currentExtent;
				else {
					int width, height;
					glfwGetFramebufferSize(&window, &width, &height);

					VkExtent2D actualExtent = {
						static_cast<uint32_t>(width),
						static_cast<uint32_t>(height)
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

				uint32_t formatCount;
				vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, nullptr);

				if (formatCount != 0) {
					details.formats.resize(formatCount);
					vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, details.formats.data());
				}

				uint32_t presentModeCount;
				vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &presentModeCount, nullptr);

				if (presentModeCount != 0) {
					details.presentModes.resize(presentModeCount);
					vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface,
						&presentModeCount, details.presentModes.data());
				}

				return details;
			}

			void swapChain::clear(ADGRVulkanDataContainer* container) {
				vkDestroySwapchainKHR(container->device, container->swapchainContainer.swapchain, nullptr);
			}

			void swapChain::cleanUp(ADGRVulkanDataContainer* container, DMKSwapChainCleanUpInfo& info) {
				for (size_t i = 0; i < container->frameBufferContainer.buffers.size(); i++)
					vkDestroyFramebuffer(container->device, container->frameBufferContainer.buffers[i], nullptr);

				vkFreeCommandBuffers(container->device, container->commandBufferContainer.commandPool,
					static_cast<uint32>(container->commandBufferContainer.buffers.size()),
					container->commandBufferContainer.buffers.data());

				vkDestroyPipeline(container->device, container->pipelineContainers[0].pipeline, nullptr);
				vkDestroyPipelineLayout(container->device, container->pipelineContainers[0].pipelineLayout, nullptr);
				vkDestroyRenderPass(container->device, container->pipelineContainers[0].renderPass, nullptr);

				for (size_t i = 0; i < container->swapchainContainer.swapchainImageViews.size(); i++)
					vkDestroyImageView(container->device, container->swapchainContainer.swapchainImageViews[i], nullptr);

				vkDestroySwapchainKHR(container->device, container->swapchainContainer.swapchain, nullptr);

				for (int x = 0; x < info.uniformBuffers.size(); x++) {
					for (size_t i = 0; i < container->swapchainContainer.swapChainImages.size(); i++) {
						vkDestroyBuffer(container->device, info.uniformBuffers[x][i], nullptr);
						vkFreeMemory(container->device, info.uniformBufferMemories[x][i], nullptr);
					}
				}

				for(int i = 0; i < info.descriptorPool.size(); i++)
				vkDestroyDescriptorPool(container->device, info.descriptorPool[i], nullptr);
			}

			void swapChain::initImageViews(ADGRVulkanDataContainer* container) {
				container->swapchainContainer.swapchainImageViews.resize(container->swapchainContainer.swapChainImages.size());

				for (uint32_t i = 0; i < container->swapchainContainer.swapChainImages.size(); i++) {
					DMKCreateImageViewInfo info;
					info.device = container->device;
					info.image = container->swapchainContainer.swapChainImages[i];
					info.format = container->swapchainContainer.swapchainImageFormat;
					info.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					info.mipLevels = 1;

					container->swapchainContainer.swapchainImageViews.at(i) = createImageView(info);
				}
			}
		}
	}
}