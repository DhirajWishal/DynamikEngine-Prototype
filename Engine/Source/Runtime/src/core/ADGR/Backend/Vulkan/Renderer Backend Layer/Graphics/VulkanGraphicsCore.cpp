#include "adgrafx.h"
#include "VulkanGraphicsCore.h"

#include "VulkanValidator.h"
#include "VulkanExtensionsManager.h"
#include "VulkanGraphicsFunctions.h"
#include "defines.h"

#include "CentralDataHub.h"

#include "VulkanGraphicsSwapChain.h"

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

			B1 isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
				ADGRVulkanQueue indices = VulkanGraphicsCore::findQueueFamilies(physicalDevice, surface);

				B1 extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

				B1 swapChainAdequate = false;
				if (extensionsSupported) {
					VulkanGraphicsSwapChainSupportDetails swapChainSupport = VulkanGraphicsSwapChain::querySwapChainSupport(&physicalDevice, &surface);
					swapChainAdequate = (swapChainSupport.formats.empty()) && (swapChainSupport.presentModes.empty());
				}

				VkPhysicalDeviceFeatures supportedFeatures;
				vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

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
				VkInstance instance,
				const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
				const VkAllocationCallbacks* pAllocator,
				VkDebugUtilsMessengerEXT* pDebugMessenger
				) {
				auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
					"vkCreateDebugUtilsMessengerEXT");

				if (func != nullptr)
					return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
				else
					return VK_ERROR_EXTENSION_NOT_PRESENT;
			}

			void destroyDebugUtilsMessengerEXT(
				VkInstance instance,
				VkDebugUtilsMessengerEXT debugMessenger,
				const VkAllocationCallbacks* pAllocator
				) {
				auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
					"vkDestroyDebugUtilsMessengerEXT");

				if (func != nullptr)
					func(instance, debugMessenger, pAllocator);
			}

			void VulkanGraphicsCore::initialize(ADGRVulkanGraphicsCoreInitDescriptor initInfo)
			{
				initializeInstance(initInfo.instanceInitInfo);
				initializeDebugger();
				initializeSurface(initInfo.windowPointer);
				initializeDevice();
				initializeSyncObjects();
			}

			void VulkanGraphicsCore::terminate()
			{
				terminateSyncObjects();
			}

			void VulkanGraphicsCore::initializeInstance(ADGRVulkanInstanceInitInfo info)
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

					populateDebugMessegerCreateInfo(debugCreateInfo);
					createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
				}
				else {
					createInfo.enabledLayerCount = 0;

					createInfo.pNext = nullptr;
				}

				if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to create instance!");
			}

			void VulkanGraphicsCore::initializeSurface(POINTER<GLFWwindow*> windowPtr)
			{
				if (glfwCreateWindowSurface(instance, windowPtr.dereference(), nullptr, &surface) != VK_SUCCESS) {
					DMK_CORE_FATAL("Failed to create window surface!");
				}
			}

			void VulkanGraphicsCore::terminateInstance()
			{
				vkDestroySurfaceKHR(instance, surface, nullptr);
				vkDestroyInstance(instance, nullptr);
			}

			void VulkanGraphicsCore::initializeDebugger()
			{
				if (!VulkanValidator::enableValidationLayers)
					return;

				VkDebugUtilsMessengerCreateInfoEXT createInfo;
				populateDebugMessegerCreateInfo(createInfo);

				if (createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to set up debug messenger!");
			}

			void VulkanGraphicsCore::terminateDebugger()
			{
				if (VulkanValidator::enableValidationLayers)
					destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
			}

			void VulkanGraphicsCore::syncFence(UI32 frame)
			{
				vkWaitForFences(logicalDevice, 1, &inFlightFences[frame], VK_TRUE, std::numeric_limits<uint64_t>::max());
			}

			VkResult VulkanGraphicsCore::getNextImage(VkSwapchainKHR swapChain, POINTER<UI32> index, UI32 frame)
			{
				return vkAcquireNextImageKHR(logicalDevice, swapChain, std::numeric_limits<uint64_t>::max(),
					imageAvailables[frame], VK_NULL_HANDLE, index);
			}

			VkResult VulkanGraphicsCore::submitQueues(ARRAY<VkSwapchainKHR> swapChains, UI32 index, UI32 frame, ARRAY<VkCommandBuffer> buffers)
			{
				if (!buffers.size())
					DMK_CORE_FATAL("No command buffers were submitted!");

				// submit info
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

				// wait for semaphores
				waitSemaphores[0] = { imageAvailables[frame] };
				waitStages[0] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
				submitInfo.waitSemaphoreCount = 1;
				submitInfo.pWaitSemaphores = waitSemaphores;
				submitInfo.pWaitDstStageMask = waitStages;
				submitInfo.commandBufferCount = buffers.size();
				submitInfo.pCommandBuffers = buffers.data();

				// signal semaphores
				signalSemaphores[0] = { renderFinishes[frame] };
				submitInfo.signalSemaphoreCount = 1;
				submitInfo.pSignalSemaphores = signalSemaphores;

				// reset fences
				vkResetFences(logicalDevice, 1, &inFlightFences[frame]);

				// submit command queue
				if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[frame]) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to submit draw command buffer!");

				// present queue info
				presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
				presentInfo.waitSemaphoreCount = 1;
				presentInfo.pWaitSemaphores = signalSemaphores;

				// swapchain info
				presentInfo.swapchainCount = swapChains.size();
				presentInfo.pSwapchains = swapChains.data();
				presentInfo.pImageIndices = &index;

				// submit queue
				return vkQueuePresentKHR(presentQueue, &presentInfo);
			}

			void VulkanGraphicsCore::populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
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

			void VulkanGraphicsCore::initializeDevice()
			{
				initPhysicalDevice();
				initLogicalDevice();
			}

			void VulkanGraphicsCore::terminateDevice()
			{
				vkDestroyDevice(logicalDevice, nullptr);
			}

			void VulkanGraphicsCore::initPhysicalDevice()
			{
				UI32 deviceCount = 0;
				vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

				if (deviceCount == 0)
					DMK_CORE_FATAL("Failed to find GPUs with Vulkan support!");

				ARRAY<VkPhysicalDevice> devices(deviceCount);
				vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

				//std::multimap<I32, VkPhysicalDevice> candidates;

				auto props = VkPhysicalDeviceProperties{};
				for (const auto& device : devices) {
					if (isDeviceSuitable(device, surface)) {
						vkGetPhysicalDeviceProperties(device, &props);

						if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
							physicalDevice = device;
						else if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
							physicalDevice = device;
						else
							physicalDevice = device;

						msaaSamples = VulkanGraphicsFunctions::getMaxUsableSampleCount(physicalDevice);
						break;
					}

					physicalDevice = VK_NULL_HANDLE;
				}

				if (physicalDevice == VK_NULL_HANDLE)
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

			void VulkanGraphicsCore::initLogicalDevice()
			{
				ADGRVulkanQueue indices = findQueueFamilies(physicalDevice, surface);

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

				if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create logical device!");

				vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
				vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
			}

			ADGRVulkanQueue VulkanGraphicsCore::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
			{
				ADGRVulkanQueue indices;

				UI32 queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

				ARRAY<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

				I32 i = 0;
				for (const auto& queueFamily : queueFamilies) {
					if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						indices.graphicsFamily = i;
					}

					VkBool32 presentSupport = false;
					vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

					if (queueFamily.queueCount > 0 && presentSupport)
						indices.presentFamily = i;

					if (indices.isComplete())
						break;

					i++;
				}

				return indices;
			}

			void VulkanGraphicsCore::initializeSyncObjects()
			{
				imageAvailables.resize(MAX_FRAMES_IN_FLIGHT);
				renderFinishes.resize(MAX_FRAMES_IN_FLIGHT);
				inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

				VkSemaphoreCreateInfo semaphoreInfo = {};
				semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

				VkFenceCreateInfo fenceInfo = {};
				fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailables.at(i)) != VK_SUCCESS ||
						vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishes.at(i)) != VK_SUCCESS ||
						vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences.at(i)) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create synchronization objects for a frame!");
				}
			}

			void VulkanGraphicsCore::terminateSyncObjects()
			{
				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					vkDestroySemaphore(logicalDevice, imageAvailables[i], nullptr);
					vkDestroySemaphore(logicalDevice, renderFinishes[i], nullptr);
					vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
				}
			}

			B1 ADGRVulkanQueue::isComplete()
			{
				return graphicsFamily.has_value() && presentFamily.has_value();;
			}
		}
	}
}