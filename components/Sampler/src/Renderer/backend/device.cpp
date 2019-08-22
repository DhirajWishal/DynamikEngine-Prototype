#include "smpafx.h"
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
					swapChainSupportDetails swapChainSupport = querySwapChainSupport(&physicalDevice, m_surface);
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
				queueFamilyindices indices = findQueueFamilies(*m_physicalDevice, surface);

				std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
				std::set<uint32_t> uniqueQueueFamilies = {
					indices.graphicsFamily.value(),
					indices.presentFamily.value()
				};

				float queuePriority = 1.0f;
				for (uint32_t queueFamily : uniqueQueueFamilies) {
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

				VkPhysicalDeviceFeatures2 features = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
				features.features.multiDrawIndirect = true;
				features.features.pipelineStatisticsQuery = true;

				VkPhysicalDevice16BitStorageFeatures features16 = 
				{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES };
				features16.storageBuffer16BitAccess = true;

				VkPhysicalDevice8BitStorageFeaturesKHR features8 = 
				{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR };
				features8.storageBuffer8BitAccess = true;

				// This will only be used if meshShadingSupported=true (see below)
				VkPhysicalDeviceMeshShaderFeaturesNV featuresMesh = 
				{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV };
				featuresMesh.taskShader = true;
				featuresMesh.meshShader = true;

				VkDeviceCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
				createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
				createInfo.pQueueCreateInfos = queueCreateInfos.data();
				createInfo.pEnabledFeatures = &deviceFeatures;
				createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
				createInfo.ppEnabledExtensionNames = deviceExtensions.data();

				createInfo.pNext = &features;
				features.pNext = &features16;
				features16.pNext = &features8;

				if (enableValidationLayers) {
					createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayer.size());
					createInfo.ppEnabledLayerNames = validationLayer.data();
				}
				else {
					createInfo.enabledLayerCount = 0;
				}

				if (meshShadingSupported)
					features8.pNext = &featuresMesh;

				if (vkCreateDevice(*m_physicalDevice, &createInfo, nullptr, m_device) != VK_SUCCESS)
					std::runtime_error("failed to create logical device!");

				vkGetDeviceQueue(*myDevice, indices.graphicsFamily.value(), 0, m_graphicsQueue);
				vkGetDeviceQueue(*myDevice, indices.presentFamily.value(), 0, m_graphicsQueue);
			}

			void device::initPhysicalDevice() {
				uint32 deviceCount = 0;
				vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

				if (deviceCount == 0)
					std::runtime_error("Failed to find GPUs with Vulkan support!");

				std::vector<VkPhysicalDevice> devices(deviceCount);
				vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

				//std::multimap<int, VkPhysicalDevice> candidates;

				for (const auto& device : devices) {
					if (isDeviceSuitable(device)) {
						*myPhysicalDevice = device;
						*m_msaaSamples = getMaxUsableSampleCount(*m_physicalDevice);
						break;
					}
				}

				if (*myPhysicalDevice == VK_NULL_HANDLE)
					std::runtime_error("Failed to find a suitable GPU!");
			}
		}
	}
}
