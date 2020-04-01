#include "adgrafx.h"
#include "VulkanDeviceManager.h"

#include "VulkanSwapChainManager.h"
#include "VulkanValidator.h"
#include "VulkanFunctions.h"
#include "VulkanQueueManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
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
					VulkanSwapChainSupportDetails swapChainSupport = querySwapChainSupport(&core->physicalDevice, &core->surface);
					swapChainAdequate = (swapChainSupport.formats.empty()) && (swapChainSupport.presentModes.empty());
				}

				VkPhysicalDeviceFeatures supportedFeatures;
				vkGetPhysicalDeviceFeatures(core->physicalDevice, &supportedFeatures);

				return indices.isComplete()
					&& extensionsSupported
					&& swapChainAdequate
					&& supportedFeatures.samplerAnisotropy;
			}

			void VulkanDeviceManager::initialize(POINTER<VulkanCoreObject> core)
			{
				initPhysicalDevice(core);
				initLogicalDevice(core);
			}

			void VulkanDeviceManager::terminate(POINTER<VulkanCoreObject> core)
			{
				vkDestroyDevice(core->logicalDevice, nullptr);
			}

			void VulkanDeviceManager::initPhysicalDevice(POINTER<VulkanCoreObject> core)
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

			void VulkanDeviceManager::initLogicalDevice(POINTER<VulkanCoreObject> core)
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
		}
	}
}