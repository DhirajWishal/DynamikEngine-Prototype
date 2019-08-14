/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "device.h"

#include "backend/queues/queues.h"
#include "backend/extensions/extensions.h"
#include "backend/validators/validators.h"
#include "backend/swapchain/swapChain.h"
#include "backend/buffers/depthBuffer.h"


#include "core/utils/DMK_DataTypes.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
				uint32 extensionCount = 0;
				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

				std::vector<VkExtensionProperties> availableExtensions(extensionCount);
				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

				std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

				for (const auto& extension : availableExtensions)
					requiredExtensions.erase(extension.extensionName);

				return requiredExtensions.empty();
			}

			device::device(VkDevice* device, VkPhysicalDevice* physicalDevice, VkSurfaceKHR* surface) :
				myDevice(device), myPhysicalDevice(physicalDevice), mySurface(surface) {
			}

			void device::initLogicalDevice(VkSurfaceKHR surface,
				VkQueue* graphicsQueue, VkQueue* presentQueue) {
				queueFamilyindices indices = findQueueFamilies(*myPhysicalDevice, surface);

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

				if (vkCreateDevice(*myPhysicalDevice, &createInfo, nullptr, myDevice) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create logical device!");

				vkGetDeviceQueue(*myDevice, indices.graphicsFamily.value(), 0, graphicsQueue);
				vkGetDeviceQueue(*myDevice, indices.presentFamily.value(), 0, presentQueue);
			}

			void device::pickPhysicalDevice(VkInstance instance, VkSampleCountFlagBits* msaaSamples) {
				uint32 deviceCount = 0;
				vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

				if (deviceCount == 0)
					DMK_CORE_FATAL("Failed to find GPUs with Vulkan support!");

				std::vector<VkPhysicalDevice> devices(deviceCount);
				vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

				//std::multimap<int, VkPhysicalDevice> candidates;

				for (const auto& device : devices) {
					if (isDeviceSuitable(device)) {
						*myPhysicalDevice = device;
						*msaaSamples = getMaxUsableSampleCount(*myPhysicalDevice);
						break;
					}
				}

				if (*myPhysicalDevice == VK_NULL_HANDLE)
					DMK_CORE_FATAL("Failed to find a suitable GPU!");
			}

			bool device::isDeviceSuitable(VkPhysicalDevice device) {
				queueFamilyindices indices = findQueueFamilies(device, *mySurface);

				bool extensionsSupported = checkDeviceExtensionSupport(device);

				bool swapChainAdequate = false;
				if (extensionsSupported) {
					swapChainSupportDetails swapChainSupport = querySwapChainSupport(device, *mySurface);
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

			int device::rateDeviceSuitability() {
				/*
				int score = 0;

				// Discrete GPUs have a significant performance advantage
				if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
					score += 1000;
				}

				// Maximum possible size of textures affects graphics quality
				score += deviceProperties.limits.maxImageDimension2D;

				// Application can't function without geometry shaders
				if (!deviceFeatures.geometryShader) {
					return 0;
				}

				return score;
				*/
				return 0;
			}
		}
	}
}
