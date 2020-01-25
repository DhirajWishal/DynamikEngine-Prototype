#include "adgrafx.h"
#include "device.h"

#include "extensions/extensions.h"
#include "validators/validators.h"
#include "queues/queues.h"
#include "controlHeader.h"
#include "swapChain.h"

#include "functions/bufferFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			bool deviceManager::checkDeviceExtensionSupport(VkPhysicalDevice device) {
				uint32_t extensionCount = 0;
				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

				std::vector<VkExtensionProperties> availableExtensions(extensionCount);
				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

				std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

				for (const auto& extension : availableExtensions)
					requiredExtensions.erase(extension.extensionName);

				return requiredExtensions.empty();
			}

			bool deviceManager::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR* surface) {
				queueFamilyindices indices = findQueueFamilies(device, *surface);

				bool extensionsSupported = checkDeviceExtensionSupport(device);

				bool swapChainAdequate = false;
				if (extensionsSupported) {
					swapChainSupportDetails swapChainSupport = querySwapChainSupport(&device, surface);
					swapChainAdequate = !swapChainSupport.formats.empty()
						&& !swapChainSupport.presentModes.empty();
				}

				VkPhysicalDeviceFeatures supportedFeatures;
				vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

				return indices.isComplete()
					&& extensionsSupported
					&& swapChainAdequate
					&& supportedFeatures.samplerAnisotropy;
			}

			void deviceManager::init(ADGRVulkanDataContainer* container) {
				initPhysicalDevice(container);
				initLogicalDevice(container);
			}

			void deviceManager::initLogicalDevice(ADGRVulkanDataContainer* container) {
				queueFamilyindices indices = findQueueFamilies(container->physicalDevice, container->surface);

				std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
				std::set<uint32> uniqueQueueFamilies = {
					indices.graphicsFamily.value(),
					indices.presentFamily.value()
				};

				float queuePriority = 1.0f;
				for (uint32 queueFamily : uniqueQueueFamilies) {
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
				createInfo.queueCreateInfoCount = static_cast<uint32>(queueCreateInfos.size());
				createInfo.pQueueCreateInfos = queueCreateInfos.data();
				createInfo.pEnabledFeatures = &deviceFeatures;
				createInfo.enabledExtensionCount = static_cast<uint32>(deviceExtensions.size());
				createInfo.ppEnabledExtensionNames = deviceExtensions.data();

				if (enableValidationLayers) {
					createInfo.enabledLayerCount = static_cast<uint32>(validationLayer.size());
					createInfo.ppEnabledLayerNames = validationLayer.data();
				}
				else {
					createInfo.enabledLayerCount = 0;
				}

				if (vkCreateDevice(container->physicalDevice, &createInfo, nullptr, &container->device) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create logical device!");

				vkGetDeviceQueue(container->device, indices.graphicsFamily.value(), 0, &container->graphicsQueue);
				vkGetDeviceQueue(container->device, indices.presentFamily.value(), 0, &container->presentQueue);
			}

			void deviceManager::initPhysicalDevice(ADGRVulkanDataContainer* container) {
				uint32 deviceCount = 0;
				vkEnumeratePhysicalDevices(container->instance, &deviceCount, nullptr);

				if (deviceCount == 0)
					DMK_CORE_FATAL("Failed to find GPUs with Vulkan support!");

				std::vector<VkPhysicalDevice> devices(deviceCount);
				vkEnumeratePhysicalDevices(container->instance, &deviceCount, devices.data());

				//std::multimap<int, VkPhysicalDevice> candidates;

				for (const auto& device : devices) {
					if (isDeviceSuitable(device, &container->surface)) {
						auto props = VkPhysicalDeviceProperties{};
						vkGetPhysicalDeviceProperties(device, &props);

						if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
							container->physicalDevice = device;
						else if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
							container->physicalDevice = device;
						else
							container->physicalDevice = device;

						container->msaaSamples = getMaxUsableSampleCount(container->physicalDevice);
						break;
					}
				}

				if (container->physicalDevice == VK_NULL_HANDLE)
					DMK_CORE_FATAL("Failed to find a suitable GPU!");

				auto props = VkPhysicalDeviceProperties{};
				vkGetPhysicalDeviceProperties(container->physicalDevice, &props);

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
		}
	}
}