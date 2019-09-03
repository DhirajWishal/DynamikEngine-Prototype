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

			bool device::checkDeviceExtensionSupport(VkPhysicalDevice device) {
				uint32_t extensionCount = 0;
				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

				std::vector<VkExtensionProperties> availableExtensions(extensionCount);
				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

				std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

				for (const auto& extension : availableExtensions)
					requiredExtensions.erase(extension.extensionName);

				return requiredExtensions.empty();
			}

			bool device::isDeviceSuitable(VkPhysicalDevice device) {
				queueFamilyindices indices = findQueueFamilies(device, *m_surface);

				bool extensionsSupported = checkDeviceExtensionSupport(device);

				bool swapChainAdequate = false;
				if (extensionsSupported) {
					swapChainSupportDetails swapChainSupport = querySwapChainSupport(&device, m_surface);
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

			void device::init() {
				initPhysicalDevice();
				initLogicalDevice();
			}

			void device::initLogicalDevice() {
				queueFamilyindices indices = findQueueFamilies(physicalDevice, surface);

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

				if (vkCreateDevice(physicalDevice, &createInfo, nullptr, m_device) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create logical device!");

				vkGetDeviceQueue(*m_device, indices.graphicsFamily.value(), 0, &graphicsQueue);
				vkGetDeviceQueue(*m_device, indices.presentFamily.value(), 0, &presentQueue);
			}

			void device::initPhysicalDevice() {
				uint32 deviceCount = 0;
				vkEnumeratePhysicalDevices(*m_instance, &deviceCount, nullptr);

				if (deviceCount == 0)
					DMK_CORE_FATAL("Failed to find GPUs with Vulkan support!");

				std::vector<VkPhysicalDevice> devices(deviceCount);
				vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

				//std::multimap<int, VkPhysicalDevice> candidates;

				for (const auto& device : devices) {
					if (isDeviceSuitable(device)) {

						auto props = VkPhysicalDeviceProperties{};
						vkGetPhysicalDeviceProperties(device, &props);

						if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
							* myPhysicalDevice = device;
						else if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
							* myPhysicalDevice = device;
						else
							*myPhysicalDevice = device;

						*m_msaaSamples = getMaxUsableSampleCount(*m_physicalDevice);
						break;
					}
				}

				if (*m_physicalDevice == VK_NULL_HANDLE)
					DMK_CORE_FATAL("Failed to find a suitable GPU!");

				auto props = VkPhysicalDeviceProperties{};
				vkGetPhysicalDeviceProperties(*m_physicalDevice, &props);

#if defined(DMK_DEBUG) || defined(DMK_RELEASE)
				printf("\n\t---------- VULKAN PHYSICAL DEVICE INFO ----------\n");
				printf("API Version: %I32d\n", props.apiVersion);
				printf("Driver Version: %I32d\n", props.driverVersion);
				printf("Vendor ID: %I32d\n", props.vendorID);
				printf("Device ID: %I32d\n", props.deviceID);

				if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
					printf("Device Type: Discrete GPU (external)\n");
				else if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
					printf("Device Type: Integrated GPU (onboard)\n");
				else
					printf("Device Type: *UNIDENTIFIED\n");

				printf("Device Name: %s\n", props.deviceName);
				printf("\t-------------------------------------------------\n\n");

 // ----------
#endif
			}
		}
	}
}
