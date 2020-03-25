#include "adgrafx.h"
#include "VulkanDevice.h"

#include "VulkanSwapChain.h"
#include "VulkanValidator.h"
#include "VulkanFunctions.h"

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

			B1 isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR* surface) {
				VulkanDevice _device;
				_device.physicalDevice = device;
				VulkanInstance _instance;
				_instance.surface = *surface;
				VulkanQueue indices = VulkanQueue::findQueueFamilies(_device, _instance);

				B1 extensionsSupported = checkDeviceExtensionSupport(device);

				B1 swapChainAdequate = false;
				if (extensionsSupported) {
					VulkanSwapChainSupportDetails swapChainSupport = querySwapChainSupport(&device, surface);
					swapChainAdequate = (swapChainSupport.formats.empty()) && (swapChainSupport.presentModes.empty());
				}

				VkPhysicalDeviceFeatures supportedFeatures;
				vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

				return indices.isComplete()
					&& extensionsSupported
					&& swapChainAdequate
					&& supportedFeatures.samplerAnisotropy;
			}

			void VulkanDevice::initialize(VulkanInstance instance, POINTER<VulkanQueue> queue, POINTER<VulkanGlobalVariables> global)
			{
				initPhysicalDevice(instance, global);
				initLogicalDevice(instance, queue);
			}

			void VulkanDevice::terminate()
			{
				vkDestroyDevice(logicalDevice, nullptr);
			}

			void VulkanDevice::initPhysicalDevice(VulkanInstance instance, POINTER<VulkanGlobalVariables> global)
			{
				UI32 deviceCount = 0;
				vkEnumeratePhysicalDevices(instance.instance, &deviceCount, nullptr);

				if (deviceCount == 0)
					DMK_CORE_FATAL("Failed to find GPUs with Vulkan support!");

				ARRAY<VkPhysicalDevice> devices(deviceCount);
				vkEnumeratePhysicalDevices(instance.instance, &deviceCount, devices.data());

				//std::multimap<I32, VkPhysicalDevice> candidates;

				auto props = VkPhysicalDeviceProperties{};
				for (const auto& device : devices) {
					if (isDeviceSuitable(device, &instance.surface)) {
						vkGetPhysicalDeviceProperties(device, &props);

						if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
							physicalDevice = device;
						else if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
							physicalDevice = device;
						else
							physicalDevice = device;

						global->msaaSamples = VulkanFunctions::getMaxUsableSampleCount(physicalDevice);
						break;
					}
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

			void VulkanDevice::initLogicalDevice(VulkanInstance instance, POINTER<VulkanQueue> queue)
			{
				VulkanQueue indices = VulkanQueue::findQueueFamilies(*this, instance);

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

				vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &queue->graphicsQueue);
				vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &queue->presentQueue);
			}
		}
	}
}