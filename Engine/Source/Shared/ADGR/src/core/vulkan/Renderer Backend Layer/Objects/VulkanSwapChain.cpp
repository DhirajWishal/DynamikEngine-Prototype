#include "adgrafx.h"
#include "VulkanSwapChain.h"
#include "VulkanQueue.h"
#include "VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface) {
				VulkanSwapChainSupportDetails details;
				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*device, *surface, &details.capabilities);

				UI32 formatCount;
				vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, nullptr);

				if (formatCount != 0) {
					details.formats.resize(formatCount);
					vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, details.formats.data());
				}

				UI32 presentModeCount;
				vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &presentModeCount, nullptr);

				if (presentModeCount != 0) {
					details.presentModes.resize(presentModeCount);
					vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface,
						&presentModeCount, details.presentModes.data());
				}

				return details;
			}

			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const ARRAY<VkSurfaceFormatKHR>& availableFormats) {
				for (const auto& availableFormat : availableFormats)
					if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM
						&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
						return availableFormat;

				return ((ARRAY<VkSurfaceFormatKHR>)availableFormats)[0];
			}

			VkPresentModeKHR chooseSwapPresentMode(const ARRAY<VkPresentModeKHR>& availablePresentModes) {
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

			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, UI32 width, UI32 height) {
				if (capabilities.currentExtent.width != std::numeric_limits<UI32>::max())
					return capabilities.currentExtent;
				else {
					VkExtent2D actualExtent = {
						width,
						height
					};

					actualExtent.width = std::max(capabilities.minImageExtent.width,
						std::min(capabilities.maxImageExtent.width, actualExtent.width));
					actualExtent.height = std::max(capabilities.minImageExtent.height,
						std::min(capabilities.maxImageExtent.height, actualExtent.height));

					return actualExtent;
				}
			}

			void VulkanSwapChain::initialize(VulkanInstance instance, VulkanDevice device, UI32 width, UI32 height)
			{
				VulkanSwapChainSupportDetails swapChainSupport = querySwapChainSupport(&device.physicalDevice, &instance.surface);

				VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
				VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
				VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

				VkCompositeAlphaFlagBitsKHR surfaceComposite =
					(instance.surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
					? VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
					: (instance.surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
					? VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR
					: (instance.surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
					? VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR
					: VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;

				UI32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
				if (swapChainSupport.capabilities.maxImageCount > 0
					&& imageCount > swapChainSupport.capabilities.maxImageCount)
					imageCount = swapChainSupport.capabilities.maxImageCount;

				VkSwapchainCreateInfoKHR createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				createInfo.surface = instance.surface;
				createInfo.minImageCount = imageCount;
				createInfo.imageFormat = surfaceFormat.format;
				createInfo.imageColorSpace = surfaceFormat.colorSpace;
				createInfo.imageExtent = extent;
				createInfo.imageArrayLayers = 1;
				createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				//createInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

				VulkanQueue indices = VulkanQueue::findQueueFamilies(device, instance);
				UI32 queueFamilyindices[] = {
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

				if (vkCreateSwapchainKHR(device.logicalDevice, &createInfo, nullptr, &mySwapChain))
					DMK_CORE_FATAL("Failed to create Swap Chain!");

				vkGetSwapchainImagesKHR(device.logicalDevice, mySwapChain, &imageCount, nullptr);
				mySwapChainImages.resize(imageCount);
				vkGetSwapchainImagesKHR(device.logicalDevice, mySwapChain, &imageCount, mySwapChainImages.data());

				mySwapChainImageFormat = surfaceFormat.format;
				mySwapChainExtent = extent;

				initializeImageViews(device);
			}
			
			void VulkanSwapChain::terminate(VulkanDevice device)
			{
				// destroy swapchain image views
				for (size_t i = 0; i < mySwapChainImageViews.size(); i++)
					vkDestroyImageView(device.logicalDevice, mySwapChainImageViews[i], nullptr);

				// destroy swapchain
				vkDestroySwapchainKHR(device.logicalDevice, mySwapChain, nullptr);
			}
			
			void VulkanSwapChain::initializeImageViews(VulkanDevice device)
			{
				mySwapChainImageViews.resize(mySwapChainImages.size());

				for (UI32 i = 0; i < mySwapChainImages.size(); i++) {
					ADGRCreateImageViewInfo info;
					info.device = device;
					info.image = mySwapChainImages[i];
					info.format = mySwapChainImageFormat;
					info.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					info.mipLevels = 1;

					mySwapChainImageViews.at(i) = VulkanFunctions::createImageView(info);
				}
			}
		}
	}
}