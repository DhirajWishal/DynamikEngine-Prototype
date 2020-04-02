#include "adgrafx.h"
#include "VulkanCoreFunctions.h"

#include "core/vulkan/Renderer Backend Layer/Managers/VulkanValidator.h"
#include "core/vulkan/Renderer Backend Layer/Managers/VulkanExtensionsManager.h"
#include "core/vulkan/Renderer Backend Layer/Managers/VulkanFunctions.h"
#include "core/vulkan/Renderer Backend Layer/Managers/VulkanQueueManager.h"
#include "defines.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			/* HELPER FUNCTIONS */
			/* DEVICE */
			const ARRAY<CCPTR> deviceExtensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

			B1 checkDeviceExtensionSupport(VkPhysicalDevice device) {
				UI32 extensionCount = 0;
				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

				ARRAY<VkExtensionProperties> availableExtensions(extensionCount);
				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

				std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

				for (const auto& extension : availableExtensions)
					requiredExtensions.erase(extension.extensionName);

				return requiredExtensions.empty();
			}

			B1 isDeviceSuitable(POINTER<VulkanCoreObject> core) {
				VulkanQueueManager indices = VulkanQueueManager::findQueueFamilies(core);

				B1 extensionsSupported = checkDeviceExtensionSupport(core->physicalDevice);

				B1 swapChainAdequate = false;
				if (extensionsSupported) {
					VulkanSwapChainSupportDetails swapChainSupport = VulkanCoreFunctions::querySwapChainSupport(&core->physicalDevice, &core->surface);
					swapChainAdequate = (swapChainSupport.formats.empty()) && (swapChainSupport.presentModes.empty());
				}

				VkPhysicalDeviceFeatures supportedFeatures;
				vkGetPhysicalDeviceFeatures(core->physicalDevice, &supportedFeatures);

				return indices.isComplete()
					&& extensionsSupported
					&& swapChainAdequate
					&& supportedFeatures.samplerAnisotropy;
			}

			/* DEBUGGER */
			static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData
				) {
				std::string myMessageStatement = "Vulkan Validation Layer ";
				std::string myMessagePreStatement = ": ";

				if (messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
					myMessagePreStatement = "(General): ";
				else if (messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
					myMessagePreStatement = "(Validation): ";
				else if (messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
					myMessagePreStatement = "(Performance): ";

				switch (messageSeverity) {
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
					std::cerr << myMessageStatement << myMessagePreStatement << pCallbackData->pMessage << std::endl;
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
					DMK_CORE_INFO("" + (myMessageStatement + myMessagePreStatement + std::string(pCallbackData->pMessage)));
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
					DMK_CORE_WARN("" + (myMessageStatement + myMessagePreStatement + std::string(pCallbackData->pMessage)));
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
					DMK_CORE_ERROR("" + (myMessageStatement + myMessagePreStatement + std::string(pCallbackData->pMessage)));
					break;
				default:
					std::cerr << myMessageStatement << myMessagePreStatement << pCallbackData->pMessage << std::endl;
					break;
				}

				return VK_FALSE;
			}

			VkResult createDebugUtilsMessengerEXT(
				POINTER<VulkanCoreObject> core,
				const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
				const VkAllocationCallbacks* pAllocator,
				VkDebugUtilsMessengerEXT* pDebugMessenger
				) {
				auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(core->instance,
					"vkCreateDebugUtilsMessengerEXT");

				if (func != nullptr)
					return func(core->instance, pCreateInfo, pAllocator, pDebugMessenger);
				else
					return VK_ERROR_EXTENSION_NOT_PRESENT;
			}

			void destroyDebugUtilsMessengerEXT(
				POINTER<VulkanCoreObject> core,
				const VkAllocationCallbacks* pAllocator
				) {
				auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(core->instance,
					"vkDestroyDebugUtilsMessengerEXT");

				if (func != nullptr)
					func(core->instance, core->debugMessenger, pAllocator);
			}

			/* SWAPCHAIN */
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

			/* CLASS DEFINITIONS */
			/* INSTANCE */
			void VulkanCoreFunctions::initializeInstance(POINTER<VulkanCoreObject> core, ADGRVulkanInstanceInitInfo info)
			{
				if (VulkanValidator::enableValidationLayers && !VulkanValidator::checkValidationLayerSupport())
					DMK_CORE_FATAL("validation layers requested, but not available!");

				// instance info
				VkApplicationInfo appInfo = {};
				appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				appInfo.pApplicationName = info.applicationName.c_str();
				appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.pEngineName = info.engineName.c_str();
				appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.apiVersion = VK_API_VERSION_1_1;

				VkInstanceCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				createInfo.pApplicationInfo = &appInfo;

				auto extentions = VulkanExtensionsManager::getRequiredExtentions(VulkanValidator::enableValidationLayers);
				createInfo.enabledExtensionCount = static_cast<UI32>(extentions.size());
				createInfo.ppEnabledExtensionNames = extentions.data();

				VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
				if (VulkanValidator::enableValidationLayers) {
					createInfo.enabledLayerCount = static_cast<UI32>(VulkanValidator::validationLayer.size());
					createInfo.ppEnabledLayerNames = VulkanValidator::validationLayer.data();

					VulkanCoreFunctions::populateDebugMessegerCreateInfo(debugCreateInfo);
					createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
				}
				else {
					createInfo.enabledLayerCount = 0;

					createInfo.pNext = nullptr;
				}

				if (vkCreateInstance(&createInfo, nullptr, &core->instance) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to create instance!");
			}
			
			void VulkanCoreFunctions::terminateInstance(POINTER<VulkanCoreObject> core)
			{
				vkDestroySurfaceKHR(core->instance, core->surface, nullptr);
				vkDestroyInstance(core->instance, nullptr);
			}

			/* DEBUGGER */
			void VulkanCoreFunctions::initializeDebugger(POINTER<VulkanCoreObject> core)
			{
				if (!VulkanValidator::enableValidationLayers)
					return;

				VkDebugUtilsMessengerCreateInfoEXT createInfo;
				populateDebugMessegerCreateInfo(createInfo);

				if (createDebugUtilsMessengerEXT(core, &createInfo, nullptr, &core->debugMessenger) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to set up debug messenger!");
			}

			void VulkanCoreFunctions::terminateDebugger(POINTER<VulkanCoreObject> core)
			{
				if (VulkanValidator::enableValidationLayers)
					destroyDebugUtilsMessengerEXT(core, nullptr);
			}

			void VulkanCoreFunctions::populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
			{
				createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
				createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
				createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
				createInfo.pfnUserCallback = debugCallback;
			}
			
			/* DEVICE */
			void VulkanCoreFunctions::initializeDevice(POINTER<VulkanCoreObject> core)
			{
				initPhysicalDevice(core);
				initLogicalDevice(core);
			}
			
			void VulkanCoreFunctions::terminateDevice(POINTER<VulkanCoreObject> core)
			{
				vkDestroyDevice(core->logicalDevice, nullptr);
			}
			
			void VulkanCoreFunctions::initPhysicalDevice(POINTER<VulkanCoreObject> core)
			{
				UI32 deviceCount = 0;
				vkEnumeratePhysicalDevices(core->instance, &deviceCount, nullptr);

				if (deviceCount == 0)
					DMK_CORE_FATAL("Failed to find GPUs with Vulkan support!");

				ARRAY<VkPhysicalDevice> devices(deviceCount);
				vkEnumeratePhysicalDevices(core->instance, &deviceCount, devices.data());

				//std::multimap<I32, VkPhysicalDevice> candidates;

				auto props = VkPhysicalDeviceProperties{};
				for (const auto& device : devices) {
					core->physicalDevice = device;
					if (isDeviceSuitable(core)) {
						vkGetPhysicalDeviceProperties(device, &props);

						if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
							core->physicalDevice = device;
						else if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
							core->physicalDevice = device;
						else
							core->physicalDevice = device;

						core->msaaSamples = VulkanFunctions::getMaxUsableSampleCount(core->physicalDevice);
						break;
					}

					core->physicalDevice = VK_NULL_HANDLE;
				}

				if (core->physicalDevice == VK_NULL_HANDLE)
					DMK_CORE_FATAL("Failed to find a suitable GPU!");

#if defined(DMK_DEBUG)
				printf("\n\t---------- VULKAN PHYSICAL DEVICE INFO ----------\n");
				printf("API Version: %I32d\n", props.apiVersion);
				printf("Driver Version: %I32d\n", props.driverVersion);
				printf("Vendor ID: %I32d\n", props.vendorID);
				printf("Device ID: %I32d\n", props.deviceID);

				switch (props.deviceType) {
				case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER:
					printf("Device Type: Other GPU\n");
					break;
				case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
					printf("Device Type: Integrated GPU (onboard)\n");
					break;
				case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
					printf("Device Type: Virtual GPU\n");
					break;
				case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU:
					printf("Device Type: CPU\n");
					break;
				default:
					printf("Device Type: *UNIDENTIFIED\n");
					break;
				}

				printf("Device Name: %s\n", props.deviceName);
				printf("\t-------------------------------------------------\n\n");

				// ----------
#endif
			}
			
			void VulkanCoreFunctions::initLogicalDevice(POINTER<VulkanCoreObject> core)
			{
				VulkanQueueManager indices = VulkanQueueManager::findQueueFamilies(core);

				ARRAY<VkDeviceQueueCreateInfo> queueCreateInfos;
				std::set<UI32> uniqueQueueFamilies = {
					indices.graphicsFamily.value(),
					indices.presentFamily.value()
				};

				F32 queuePriority = 1.0f;
				for (UI32 queueFamily : uniqueQueueFamilies) {
					VkDeviceQueueCreateInfo queueCreateInfo = {};
					queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					queueCreateInfo.queueFamilyIndex = queueFamily;
					queueCreateInfo.queueCount = 1;
					queueCreateInfo.pQueuePriorities = &queuePriority;
					queueCreateInfos.push_back(queueCreateInfo);
				}

				VkPhysicalDeviceFeatures deviceFeatures = {};
				deviceFeatures.samplerAnisotropy = VK_TRUE;
				deviceFeatures.sampleRateShading = VK_TRUE; // enable sample shading feature for the device

				VkDeviceCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
				createInfo.queueCreateInfoCount = static_cast<UI32>(queueCreateInfos.size());
				createInfo.pQueueCreateInfos = queueCreateInfos.data();
				createInfo.pEnabledFeatures = &deviceFeatures;
				createInfo.enabledExtensionCount = static_cast<UI32>(deviceExtensions.size());
				createInfo.ppEnabledExtensionNames = deviceExtensions.data();

				if (VulkanValidator::enableValidationLayers) {
					createInfo.enabledLayerCount = static_cast<UI32>(VulkanValidator::validationLayer.size());
					createInfo.ppEnabledLayerNames = VulkanValidator::validationLayer.data();
				}
				else {
					createInfo.enabledLayerCount = 0;
				}

				if (vkCreateDevice(core->physicalDevice, &createInfo, nullptr, &core->logicalDevice) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create logical device!");

				vkGetDeviceQueue(core->logicalDevice, indices.graphicsFamily.value(), 0, &core->graphicsQueue);
				vkGetDeviceQueue(core->logicalDevice, indices.presentFamily.value(), 0, &core->presentQueue);
			}
			
			/* SWAPCHAIN */
			void VulkanCoreFunctions::initializeSwapChain(POINTER<VulkanCoreObject> core, UI32 width, UI32 height)
			{
				VulkanSwapChainSupportDetails swapChainSupport = querySwapChainSupport(&core->physicalDevice, &core->surface);

				VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
				VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
				VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

				VkCompositeAlphaFlagBitsKHR surfaceComposite =
					(core->surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
					? VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
					: (core->surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
					? VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR
					: (core->surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
					? VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR
					: VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;

				UI32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
				if (swapChainSupport.capabilities.maxImageCount > 0
					&& imageCount > swapChainSupport.capabilities.maxImageCount)
					imageCount = swapChainSupport.capabilities.maxImageCount;

				VkSwapchainCreateInfoKHR createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				createInfo.surface = core->surface;
				createInfo.minImageCount = imageCount;
				createInfo.imageFormat = surfaceFormat.format;
				createInfo.imageColorSpace = surfaceFormat.colorSpace;
				createInfo.imageExtent = extent;
				createInfo.imageArrayLayers = 1;
				createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				//createInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

				VulkanQueueManager indices = VulkanQueueManager::findQueueFamilies(core);
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

				if (vkCreateSwapchainKHR(core->logicalDevice, &createInfo, nullptr, &core->swapChain))
					DMK_CORE_FATAL("Failed to create Swap Chain!");

				vkGetSwapchainImagesKHR(core->logicalDevice, core->swapChain, &imageCount, nullptr);
				core->swapChainImages.resize(imageCount);
				vkGetSwapchainImagesKHR(core->logicalDevice, core->swapChain, &imageCount, core->swapChainImages.data());

				core->swapChainImageFormat = surfaceFormat.format;
				core->swapChainExtent = extent;

				initializeSwapChainImageViews(core);
			}
			
			void VulkanCoreFunctions::terminateSwapChain(POINTER<VulkanCoreObject> core)
			{
				// destroy swapchain image views
				for (size_t i = 0; i < core->swapChainImageViews.size(); i++)
					vkDestroyImageView(core->logicalDevice, core->swapChainImageViews[i], nullptr);

				// destroy swapchain
				vkDestroySwapchainKHR(core->logicalDevice, core->swapChain, nullptr);
			}
			
			void VulkanCoreFunctions::initializeSwapChainImageViews(POINTER<VulkanCoreObject> core)
			{
				core->swapChainImageViews.resize(core->swapChainImages.size());

				for (UI32 i = 0; i < core->swapChainImages.size(); i++) {
					ADGRCreateImageViewInfo info;
					info.image = core->swapChainImages[i];
					info.format = core->swapChainImageFormat;
					info.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					info.mipLevels = 1;

					core->swapChainImageViews.at(i) = VulkanFunctions::createImageView(core, info);
				}
			}
			
			VulkanSwapChainSupportDetails VulkanCoreFunctions::querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface)
			{
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
			
			/* RENDER PASS */
			void VulkanCoreFunctions::initializeRenderPass(POINTER<VulkanCoreObject> core, ADGRVulkanRenderPassInitInfo info)
			{
				ARRAY<VkAttachmentDescription> attachments;

				// attachment descriptions
				VkAttachmentDescription colorAttachment = {};
				colorAttachment.format = core->swapChainImageFormat;
				colorAttachment.samples = core->msaaSamples;
				colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				attachments.push_back(colorAttachment);

				VkAttachmentReference colorAttachmentRef = {};
				colorAttachmentRef.attachment = 0;
				colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				VkAttachmentReference colorAttachmentResolveRef = {};
				colorAttachmentResolveRef.attachment = 2;
				colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				// sub passes
				VkSubpassDescription subpass = {};
				subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpass.colorAttachmentCount = 1;
				subpass.pColorAttachments = &colorAttachmentRef;
				subpass.pResolveAttachments = &colorAttachmentResolveRef;

				if (info.enableDepthAttachment)
				{
					VkAttachmentDescription depthAttachment = {};
					depthAttachment.format = VulkanFunctions::findDepthFormat(core->physicalDevice);
					depthAttachment.samples = core->msaaSamples;
					depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
					depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					attachments.push_back(depthAttachment);

					VkAttachmentReference depthAttachmentRef = {};
					depthAttachmentRef.attachment = 1;
					depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

					subpass.pDepthStencilAttachment = &depthAttachmentRef;
				}

				VkAttachmentDescription colorAttachmentResolve = {};
				colorAttachmentResolve.format = core->swapChainImageFormat;
				colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
				colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachments.push_back(colorAttachmentResolve);

				ARRAY<VkSubpassDescription> subPasses;
				subPasses.push_back(subpass);

				for (VkSubpassDescription _description : info.additionalSubPasses)
					subPasses.push_back(_description);

				VkSubpassDependency dependency = {};
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = info.destinationSubpass;
				dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.srcAccessMask = info.accessFlags;
				dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

				ARRAY<VkSubpassDependency> dependencies;
				dependencies.push_back(dependency);

				for (VkSubpassDependency _dependency : info.additionalSubPassDependencies)
					dependencies.push_back(_dependency);

				// render pass info
				VkRenderPassCreateInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				renderPassInfo.attachmentCount = static_cast<UI32>(attachments.size());
				renderPassInfo.pAttachments = attachments.data();
				renderPassInfo.subpassCount = subPasses.size();
				renderPassInfo.pSubpasses = subPasses.data();
				renderPassInfo.dependencyCount = dependencies.size();
				renderPassInfo.pDependencies = dependencies.data();

				// create the render pass
				if (vkCreateRenderPass(core->logicalDevice, &renderPassInfo, nullptr, &core->renderPass) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create render pass!");
			}
			
			void VulkanCoreFunctions::terminateRenderPass(POINTER<VulkanCoreObject> core)
			{
				if (core->renderPass != VK_NULL_HANDLE)
					vkDestroyRenderPass(core->logicalDevice, core->renderPass, nullptr);
			}
			
			/* PIPELINE */
			void VulkanCoreFunctions::initializePipeline(POINTER<VulkanCoreObject> core, ADGRVulkanPipelineInitInfo info, POINTER<VulkanRenderObject> object)
			{
				VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
				pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipelineLayoutInfo.setLayoutCount = 1;
				pipelineLayoutInfo.pSetLayouts = &object->descriptorLayout;

				if (info.pushConstantsEnable) {
					ARRAY<VkPushConstantRange> pushConstantInfos;

					core->pushConstants.resize(info.pushConstantCount);
					/*
#define r 7.5f
#define sin_t sin(glm::radians(1.5f * 360))
#define cos_t cos(glm::radians(1.5f * 360))
#define y -4.0f

					info.global->pushConstants[0] = glm::vec4(r * 1.1 * sin_t, y, r * 1.1 * cos_t, 1.0f);
					info.global->pushConstants[1] = glm::vec4(-r * sin_t, y, -r * cos_t, 1.0f);
					info.global->pushConstants[2] = glm::vec4(r * 0.85f * sin_t, y, -sin_t * 2.5f, 1.5f);
					info.global->pushConstants[3] = glm::vec4(0.0f, y, r * 1.25f * cos_t, 1.5f);
					info.global->pushConstants[4] = glm::vec4(r * 2.25f * cos_t, y, 0.0f, 1.25f);
					info.global->pushConstants[5] = glm::vec4(r * 2.5f * cos_t, y, r * 2.5f * sin_t, 1.25f);

#undef r
#undef y
#undef sin_t
#undef cos_t
					*/
					// initialize push constants
					for (I32 i = 0; i < info.pushConstantCount; i++) {
						VkPushConstantRange pushConsInfo = {};
						pushConsInfo.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;// TODO::
						pushConsInfo.size = sizeof(core->pushConstants);
						pushConsInfo.offset = info.pushConstantOffset;

						pushConstantInfos.push_back(pushConsInfo);
					}
					pipelineLayoutInfo.pushConstantRangeCount = info.pushConstantCount;
					pipelineLayoutInfo.pPushConstantRanges = pushConstantInfos.data();
				}
				else
				{
					pipelineLayoutInfo.pushConstantRangeCount = 0;
					pipelineLayoutInfo.pPushConstantRanges = nullptr;
				}

				// create the pipeline layout
				if (vkCreatePipelineLayout(core->logicalDevice, &pipelineLayoutInfo, nullptr, &object->pipelineLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create pipeline layout!");

				auto bindingDescription = Vertex::getBindingDescription(1);
				auto attributeDescriptions = Vertex::getAttributeDescriptions();

				VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
				vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
				vertexInputInfo.vertexBindingDescriptionCount = static_cast<UI32>(bindingDescription.size());
				vertexInputInfo.vertexAttributeDescriptionCount = static_cast<UI32>(attributeDescriptions.size());
				vertexInputInfo.pVertexBindingDescriptions = bindingDescription.data();
				vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

				// initialize the input assembler
				VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
				inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
				inputAssembly.topology = info.inputAssemblyTopology;
				inputAssembly.primitiveRestartEnable = info.inputAssemblyPrimitiveRestartEnable;

				// initialize the viewport(s)
				std::array<VkViewport, 1> viewports;
				for (I32 i = 0; i < info.viweportCount; i++) {
					VkViewport viewport = {};
					viewport.x = 0.0f;
					viewport.y = 0.0f;
					viewport.width = core->swapChainExtent.width;
					viewport.height = core->swapChainExtent.height;
					viewport.minDepth = 0.0f;
					viewport.maxDepth = 1.0f;

					viewports[0] = (viewport);
				}

				// initialize the scissor(s)
				std::array<VkRect2D, 1> scissors;
				for (I32 i = 0; i < info.scissorCount; i++) {
					VkRect2D scissor = {};
					scissor.offset = info.offsets[0];
					scissor.extent = core->swapChainExtent;

					scissors[0] = (scissor);
				}

				// initialize the viewport state
				VkPipelineViewportStateCreateInfo viewportState = {};
				viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
				viewportState.viewportCount = info.viweportCount;
				viewportState.pViewports = viewports.data();
				viewportState.scissorCount = info.scissorCount;
				viewportState.pScissors = scissors.data();

				// initialize the rasterizer
				VkPipelineRasterizationStateCreateInfo rasterizer = {};
				rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
				rasterizer.depthClampEnable = info.rasterizerDepthClampEnable;
				rasterizer.rasterizerDiscardEnable = info.rasterizerDiscardEnable;
				rasterizer.polygonMode = info.rasterizerPolygonMode;
				rasterizer.lineWidth = info.rasterizerLineWidth;
				rasterizer.cullMode = info.rasterizerCullMode;
				rasterizer.frontFace = info.rasterizerFrontFace;
				rasterizer.depthBiasEnable = info.rasterizerDepthBiasEnable;

				// initialize multisampling
				VkPipelineMultisampleStateCreateInfo multisampling = {};
				multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
				multisampling.rasterizationSamples = core->msaaSamples;
				multisampling.sampleShadingEnable = info.multisamplerSampleShadingEnable; // enable sample shading in the pipeline
				multisampling.minSampleShading = info.multisamplerMinSampleShading; // min fraction for sample shading; closer to one is smoother

				VkPipelineDepthStencilStateCreateInfo depthStencil = {};
				depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
				depthStencil.depthTestEnable = info.depthStencilEnable;
				depthStencil.depthWriteEnable = info.depthStencilWriteEnable;
				depthStencil.depthCompareOp = info.depthStencilCompareOp;
				depthStencil.depthBoundsTestEnable = info.depthStencilBoundsTestEnable;
				depthStencil.stencilTestEnable = info.depthStencilTestEnable;

				std::array<VkPipelineColorBlendAttachmentState, 1> colorBlendAttachments;

				// initialize the color blender(s)
				for (I32 i = 0; i < info.colorBlendingColorBlendCount; i++) {
					VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
					colorBlendAttachment.colorWriteMask = info.colorBlenderColorWriteMasks[i];
					colorBlendAttachment.blendEnable = info.colorBlenderBlendEnables[i];

					colorBlendAttachments[0] = (colorBlendAttachment);
				}

				// initialize the color blender state
				VkPipelineColorBlendStateCreateInfo colorBlending = {};
				colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
				colorBlending.logicOpEnable = info.colorBlendingLogicOpEnable;
				colorBlending.logicOp = info.colorBlendingLogicOp;
				colorBlending.attachmentCount = info.colorBlendingColorBlendCount;
				colorBlending.pAttachments = colorBlendAttachments.data();
				colorBlending.blendConstants[0] = info.colorBlendingBlendConstants[0];
				colorBlending.blendConstants[1] = info.colorBlendingBlendConstants[1];
				colorBlending.blendConstants[2] = info.colorBlendingBlendConstants[2];
				colorBlending.blendConstants[3] = info.colorBlendingBlendConstants[3];

				//ARRAY<VkPipelineShaderStageCreateInfo> shaderStages;
				//for (VulkanShaderManager _shader : info.shaders)
				//	shaderStages.push_back(_shader.stageCreateInfo);

				VkPipelineShaderStageCreateInfo _infoV;
				_infoV.stage = VK_SHADER_STAGE_VERTEX_BIT;
				_infoV.module = VulkanCoreFunctions::createShaderModule(core->logicalDevice, VulkanCoreFunctions::getCode(info.vertex));
				_infoV.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				_infoV.pName = "main";
				_infoV.pNext = VK_NULL_HANDLE;
				_infoV.pSpecializationInfo = VK_NULL_HANDLE;
				_infoV.flags = VK_NULL_HANDLE;

				VkPipelineShaderStageCreateInfo _infoF;
				_infoF.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				_infoF.module = VulkanCoreFunctions::createShaderModule(core->logicalDevice, VulkanCoreFunctions::getCode(info.fragment));
				_infoF.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				_infoF.pName = "main";
				_infoF.pNext = VK_NULL_HANDLE;
				_infoF.pSpecializationInfo = VK_NULL_HANDLE;
				_infoF.flags = VK_NULL_HANDLE;

				std::array<VkPipelineShaderStageCreateInfo, 2> stages = { _infoV , _infoF };

				// initialize the pipeline
				VkGraphicsPipelineCreateInfo pipelineInfo = {};
				pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
				//pipelineInfo.stageCount = shaderStages.size();
				//pipelineInfo.pStages = shaderStages.data();
				pipelineInfo.stageCount = 2;
				pipelineInfo.pStages = stages.data();
				pipelineInfo.pVertexInputState = &vertexInputInfo;
				pipelineInfo.pInputAssemblyState = &inputAssembly;
				pipelineInfo.pViewportState = &viewportState;
				pipelineInfo.pRasterizationState = &rasterizer;
				pipelineInfo.pMultisampleState = &multisampling;
				pipelineInfo.pDepthStencilState = &depthStencil;
				pipelineInfo.pColorBlendState = &colorBlending;
				pipelineInfo.layout = object->pipelineLayout;
				pipelineInfo.renderPass = *object->renderPass;
				pipelineInfo.subpass = info.pipelineSubPass;
				pipelineInfo.basePipelineHandle = info.pipelineBasePipelineHandle;
				pipelineInfo.basePipelineIndex = info.pipelineBasePipelineIndex;

				// initialize dynamic state
				if (info.dynamicStateEnable) {
					VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
					ARRAY<VkDynamicState> dynamicState = {
						VK_DYNAMIC_STATE_VIEWPORT,
						VK_DYNAMIC_STATE_SCISSOR
					};

					dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
					dynamicStateInfo.dynamicStateCount = static_cast<UI32>(dynamicState.size());
					dynamicStateInfo.pDynamicStates = dynamicState.data();
					dynamicStateInfo.flags = info.dynamicStateFlags;
					pipelineInfo.pDynamicState = &dynamicStateInfo;
				}

				// create the pipeline
				if (vkCreateGraphicsPipelines(core->logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &object->pipeline) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create graphics pipeline!");
			}
			
			void VulkanCoreFunctions::terminatePipeline(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object)
			{
				vkDestroyPipeline(core->logicalDevice, object->pipeline, nullptr);
				vkDestroyPipelineLayout(core->logicalDevice, object->pipelineLayout, nullptr);
			}
			
			/* DESCRIPTOR LAYOUT */
			void VulkanCoreFunctions::initializeDescriptorLayout(POINTER<VulkanCoreObject> core, ADGRVulkanDescriptorSetLayoutInitInfo info, POINTER<VulkanRenderObject> object)
			{
				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

				VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
				samplerLayoutBinding.binding = 1; // info.bindIndex;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

				ARRAY<VkDescriptorSetLayoutBinding> bindings;
				bindings.push_back(uboLayoutBinding);
				bindings.push_back(samplerLayoutBinding);

				for (VkDescriptorSetLayoutBinding _binding : info.additionalBindings)
					bindings.push_back(_binding);

				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = static_cast<UI32>(bindings.size());
				layoutInfo.pBindings = bindings.data();

				if (vkCreateDescriptorSetLayout(core->logicalDevice, &layoutInfo, nullptr, &object->descriptorLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor set layout!");
			}
			
			void VulkanCoreFunctions::terminateDescriptorLayout(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object)
			{
				vkDestroyDescriptorSetLayout(core->logicalDevice, object->descriptorLayout, nullptr);
			}
			
			/* DESCRIPTOR POOL */
			void VulkanCoreFunctions::initializeDescriptorPool(POINTER<VulkanCoreObject> core, ADGRVulkanDescriptorPoolInitInfo info, POINTER<VulkanRenderObject> object)
			{
				for (UI32 itr = 0; itr < info.poolCount; itr++) {
					ARRAY<VkDescriptorPoolSize> poolSizes = {};
					VkDescriptorPoolSize _poolSize1;
					_poolSize1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					_poolSize1.descriptorCount = static_cast<UI32>(core->swapChainImages.size());

					VkDescriptorPoolSize _poolSize2;
					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = static_cast<UI32>(core->swapChainImages.size());

					poolSizes.push_back(_poolSize1);
					poolSizes.push_back(_poolSize2);

					for (VkDescriptorPoolSize _size : info.additionalSizes)
						poolSizes.push_back(_size);

					VkDescriptorPoolCreateInfo poolInfo = {};
					poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
					poolInfo.poolSizeCount = static_cast<UI32>(poolSizes.size());
					poolInfo.pPoolSizes = poolSizes.data();
					poolInfo.maxSets = static_cast<UI32>(core->swapChainImages.size());

					VkDescriptorPool _localDescriptorPool = VK_NULL_HANDLE;

					if (vkCreateDescriptorPool(core->logicalDevice, &poolInfo, nullptr, &_localDescriptorPool) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create descriptor pool!");

					object->descriptorPools.push_back(_localDescriptorPool);
				}
			}
			
			void VulkanCoreFunctions::terminateDescriptorPool(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object)
			{
				for (auto descriptorPool : object->descriptorPools)
					vkDestroyDescriptorPool(core->logicalDevice, descriptorPool, nullptr);
			}
			
			/* DESCRIPTOR SETS */
			void VulkanCoreFunctions::initializeDescriptorSets(POINTER<VulkanCoreObject> core, ADGRVulkanDescriptorSetsInitInfo info, POINTER<VulkanRenderObject> object)
			{
				ARRAY<VkDescriptorSetLayout> layouts(core->swapChainImages.size(), object->descriptorLayout);
				object->descriptorSets.resize(object->textures.size());

				for (UI32 itr = 0; itr < object->textures.size(); itr++) {
					VkDescriptorSetAllocateInfo allocInfo = {};
					allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
					allocInfo.descriptorPool = object->descriptorPools[itr];
					allocInfo.descriptorSetCount = static_cast<UI32>(core->swapChainImages.size());
					allocInfo.pSetLayouts = layouts.data();

					object->descriptorSets.resize(core->swapChainImages.size());
					if (vkAllocateDescriptorSets(core->logicalDevice, &allocInfo, object->descriptorSets[itr].data()) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to allocate descriptor sets!");

					for (size_t i = 0; i < (core->swapChainImages.size()); i++) {
						VkDescriptorBufferInfo bufferInfo = {};
						bufferInfo.buffer = object->uniformBuffers[i];
						bufferInfo.offset = 0;
						bufferInfo.range = sizeof(UniformBufferObject);

						VkDescriptorImageInfo imageInfo = {};
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						imageInfo.imageView = object->textures[itr].imageView;
						imageInfo.sampler = object->textures[itr].imageSampler;

						ARRAY<VkWriteDescriptorSet> descriptorWrites = {};

						VkWriteDescriptorSet _writes1;
						_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes1.dstSet = object->descriptorSets[itr][i];
						_writes1.dstBinding = 0;
						_writes1.dstArrayElement = 0;
						_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
						_writes1.descriptorCount = 1;
						_writes1.pBufferInfo = &bufferInfo;

						VkWriteDescriptorSet _writes2;
						_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes2.dstSet = object->descriptorSets[itr][i];
						_writes2.dstBinding = 1;
						_writes2.dstArrayElement = 0;
						_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						_writes2.descriptorCount = 1;
						_writes2.pImageInfo = &imageInfo;

						descriptorWrites.push_back(_writes1);
						descriptorWrites.push_back(_writes2);

						for (VkWriteDescriptorSet _write : info.additionalWrites)
							descriptorWrites.push_back(_write);

						vkUpdateDescriptorSets(core->logicalDevice, static_cast<UI32>(descriptorWrites.size()),
							descriptorWrites.data(), 0, nullptr);
					} // make two descriptor layouts for each descriptor set
				}
			}
			
			void VulkanCoreFunctions::terminateDescriptorSets(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object)
			{
			}
			
			/* SHADERS */
			ADGRVulkanShaderContainer VulkanCoreFunctions::initializeShader(POINTER<VulkanCoreObject> core, ADGRVulkanShaderInitInfo info)
			{
				ADGRVulkanShaderContainer _container;

				if (_container.type == ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX)
					_container.stageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
				else if (_container.type == ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION)
					_container.stageCreateInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				else if (_container.type == ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY)
					_container.stageCreateInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
				else if (_container.type == ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT)
					_container.stageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

				_container.stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				_container.stageCreateInfo.module = _container.shaderModule;
				_container.stageCreateInfo.pName = "main";
				_container.stageCreateInfo.pNext = VK_NULL_HANDLE;
				_container.stageCreateInfo.pSpecializationInfo = VK_NULL_HANDLE;
				_container.stageCreateInfo.flags = VK_NULL_HANDLE;

				return _container;
			}
			
			void VulkanCoreFunctions::terminateShader(POINTER<VulkanCoreObject> core, ADGRVulkanShaderContainer container)
			{
				vkDestroyShaderModule(core->logicalDevice, container.shaderModule, nullptr);
			}
			
			VkShaderModule VulkanCoreFunctions::createShaderModule(VkDevice device, ARRAY<CHR> code)
			{
				VkShaderModuleCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				createInfo.codeSize = code.size();
				createInfo.pCode = reinterpret_cast<const UI32*>(code.data());

				VkShaderModule shaderModule;
				if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to create Shader module!");

				return shaderModule;
			}
			
			ARRAY<CHR> VulkanCoreFunctions::getCode(std::string path)
			{
				std::ifstream file(path, std::ios::ate | std::ios::binary);

				if (!file.is_open())
					throw std::runtime_error("Failed to Open file!");

				size_t fileSize = (size_t)file.tellg();
				ARRAY<char> buffer(fileSize);
				file.seekg(0);
				file.read(buffer.data(), fileSize);

				file.close();
				return buffer;
			}
			
			/* SYNC OBJECTS */
			void VulkanCoreFunctions::initializeSyncObjects(VkDevice device, POINTER<ADGRVulkanSyncObjectsContainer> container)
			{
				container->imageAvailables.resize(MAX_FRAMES_IN_FLIGHT);
				container->renderFinishes.resize(MAX_FRAMES_IN_FLIGHT);
				container->inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

				VkSemaphoreCreateInfo semaphoreInfo = {};
				semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

				VkFenceCreateInfo fenceInfo = {};
				fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &container->imageAvailables.at(i)) != VK_SUCCESS ||
						vkCreateSemaphore(device, &semaphoreInfo, nullptr, &container->renderFinishes.at(i)) != VK_SUCCESS ||
						vkCreateFence(device, &fenceInfo, nullptr, &container->inFlightFences.at(i)) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create synchronization objects for a frame!");
				}
			}
			
			void VulkanCoreFunctions::terminateSyncObjects(VkDevice device, POINTER<ADGRVulkanSyncObjectsContainer> container)
			{
				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					vkDestroySemaphore(device, container->imageAvailables[i], nullptr);
					vkDestroySemaphore(device, container->renderFinishes[i], nullptr);
					vkDestroyFence(device, container->inFlightFences[i], nullptr);
				}
			}
		}
	}
}