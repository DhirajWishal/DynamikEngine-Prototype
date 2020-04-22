#include "dmkafx.h"
#include "VulkanComputeCore.h"

#include "../Graphics/VulkanUtilities.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			B1 isCandidate(ADGRVulkanComputeShaderPhysicalDeviceLimits oldLimits, VkPhysicalDeviceProperties properties)
			{
				ARRAY<B1> _candidateCheck(4);
				_candidateCheck[0] = oldLimits.maxSharedMemorySize < properties.limits.maxComputeSharedMemorySize;
				_candidateCheck[1] =
					oldLimits.maxWorkGroupCount[0] < properties.limits.maxComputeWorkGroupCount[0] ||
					oldLimits.maxWorkGroupCount[1] < properties.limits.maxComputeWorkGroupCount[1] ||
					oldLimits.maxWorkGroupCount[2] < properties.limits.maxComputeWorkGroupCount[2];
				_candidateCheck[2] = oldLimits.maxWorkGroupInvocations < properties.limits.maxComputeWorkGroupInvocations;
				_candidateCheck[3] =
					oldLimits.maxWorkGroupSize[0] < properties.limits.maxComputeWorkGroupSize[0] ||
					oldLimits.maxWorkGroupSize[1] < properties.limits.maxComputeWorkGroupSize[1] ||
					oldLimits.maxWorkGroupSize[2] < properties.limits.maxComputeWorkGroupSize[2];

				for (B1 _candidateResult : _candidateCheck)
					if (!_candidateResult)
						return false;

				return true;
			}

			void VulkanComputeCore::initializeInstance()
			{
				VkApplicationInfo appInfo = {};
				appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				appInfo.pApplicationName = "Dynamik Engine";
				appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.pEngineName = "Dynamik";
				appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.apiVersion = VK_API_VERSION_1_1;

				VkInstanceCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				createInfo.pApplicationInfo = &appInfo;
				createInfo.enabledExtensionCount = 0;
				createInfo.ppEnabledExtensionNames = VK_NULL_HANDLE;
				createInfo.enabledLayerCount = 0;
				createInfo.pNext = nullptr;

				if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to create instance!");
			}

			void VulkanComputeCore::initializeDevice()
			{
				initializePhysicalDevice();
				initializeLogicalDevice();
			}

			void VulkanComputeCore::submitQueue(ARRAY<VkCommandBuffer> commandBuffers)
			{
				VkSubmitInfo submitInfo;
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = commandBuffers.size();
				submitInfo.pCommandBuffers = commandBuffers.data();
				submitInfo.pNext = VK_NULL_HANDLE;
				submitInfo.pSignalSemaphores = VK_NULL_HANDLE;
				submitInfo.pWaitDstStageMask = VK_NULL_HANDLE;
				submitInfo.pWaitSemaphores = VK_NULL_HANDLE;
				submitInfo.signalSemaphoreCount = VK_NULL_HANDLE;
				submitInfo.waitSemaphoreCount = VK_NULL_HANDLE;

				if (vkQueueSubmit(computeQueue, 1, &submitInfo, 0) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to submit compute command buffer!");
			}

			void VulkanComputeCore::finishCompute()
			{
				vkQueueWaitIdle(computeQueue);
			}

			UI32 getBestTransferQueue(VkPhysicalDevice physicalDevice)
			{
				UI32 queueFamilyPropertiesCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, 0);

				VkQueueFamilyProperties queueFamilyProperties;

				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, &queueFamilyProperties);

				// first try and find a queue that has just the transfer bit set
				for (UI32 i = 0; i < queueFamilyPropertiesCount; i++) {
					// mask out the sparse binding bit that we aren't caring about (yet!)
					const VkQueueFlags maskedFlags = (~VK_QUEUE_SPARSE_BINDING_BIT & ((VkQueueFamilyProperties*)&queueFamilyProperties)[i].queueFlags);

					if (!((VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT) & maskedFlags) &&
						(VK_QUEUE_TRANSFER_BIT & maskedFlags)) {
						return i;
					}
				}

				// otherwise we'll prefer using a compute-only queue,
				// remember that having compute on the queue implicitly enables transfer!
				for (UI32 i = 0; i < queueFamilyPropertiesCount; i++) {
					// mask out the sparse binding bit that we aren't caring about (yet!)
					const VkQueueFlags maskedFlags = (~VK_QUEUE_SPARSE_BINDING_BIT & ((VkQueueFamilyProperties*)&queueFamilyProperties)[i].queueFlags);

					if (!(VK_QUEUE_GRAPHICS_BIT & maskedFlags) && (VK_QUEUE_COMPUTE_BIT & maskedFlags)) {
						return i;
					}
				}

				// lastly get any queue that'll work for us (graphics, compute or transfer bit set)
				for (UI32 i = 0; i < queueFamilyPropertiesCount; i++) {
					// mask out the sparse binding bit that we aren't caring about (yet!)
					const VkQueueFlags maskedFlags = (~VK_QUEUE_SPARSE_BINDING_BIT & ((VkQueueFamilyProperties*)&queueFamilyProperties)[i].queueFlags);

					if ((VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT) & maskedFlags) {
						return i;
					}
				}

				return VK_ERROR_INITIALIZATION_FAILED;
			}

			UI32 getBestComputeQueue(VkPhysicalDevice physicalDevice)
			{
				UI32 queueFamilyPropertiesCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, 0);

				VkQueueFamilyProperties queueFamilyProperties;

				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, &queueFamilyProperties);

				// first try and find a queue that has just the compute bit set
				for (UI32 i = 0; i < queueFamilyPropertiesCount; i++) {
					// mask out the sparse binding bit that we aren't caring about (yet!) and the transfer bit
					const VkQueueFlags maskedFlags = (~(VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT) &
						((VkQueueFamilyProperties*)&queueFamilyProperties)[i].queueFlags);

					if (!(VK_QUEUE_GRAPHICS_BIT & maskedFlags) && (VK_QUEUE_COMPUTE_BIT & maskedFlags)) {
						return i;
					}
				}

				// lastly get any queue that'll work for us
				for (UI32 i = 0; i < queueFamilyPropertiesCount; i++) {
					// mask out the sparse binding bit that we aren't caring about (yet!) and the transfer bit
					const VkQueueFlags maskedFlags = (~(VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT) &
						((VkQueueFamilyProperties*)&queueFamilyProperties)[i].queueFlags);

					if (VK_QUEUE_COMPUTE_BIT & maskedFlags) {
						return i;
					}
				}

				return 0;
			}

			void VulkanComputeCore::initializePhysicalDevice()
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
					vkGetPhysicalDeviceProperties(device, &props);

					if (isCandidate(physicalDeviceLimits, props))
					{
						vkGetPhysicalDeviceMemoryProperties(device, &physicalDeviceMemoryProperties);
						physicalDeviceLimits = ADGRVulkanComputeShaderPhysicalDeviceLimits(props);
						physicalDevice = device;
					}
				}

				if (physicalDevice == VK_NULL_HANDLE)
					DMK_CORE_FATAL("Failed to find a suitable GPU!");

#if defined(DMK_DEBUG)
				printf("\n\t---------- VULKAN PHYSICAL DEVICE INFO: COMPUTE SHADER ----------\n");
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
				printf("\t-----------------------------------------------------------------\n\n");

				// ----------
#endif
			}

			void VulkanComputeCore::initializeLogicalDevice()
			{
				queueFamilyIndices = ADGRVulkanComputeQueue::getQueues(physicalDevice);

				VkPhysicalDeviceFeatures deviceFeatures = {};
				deviceFeatures.samplerAnisotropy = VK_TRUE;
				deviceFeatures.sampleRateShading = VK_TRUE; // enable sample shading feature for the device

				VkDeviceCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
				createInfo.queueCreateInfoCount = 0;
				createInfo.pQueueCreateInfos = VK_NULL_HANDLE;
				createInfo.pEnabledFeatures = &deviceFeatures;
				createInfo.enabledExtensionCount = 0;
				createInfo.ppEnabledExtensionNames = VK_NULL_HANDLE;

				if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create logical device!");

				computeQueue = VK_NULL_HANDLE;
				transferQueue = VK_NULL_HANDLE;
				vkGetDeviceQueue(logicalDevice, queueFamilyIndices.computeFamily.value(), 0, &computeQueue);
				vkGetDeviceQueue(logicalDevice, queueFamilyIndices.transferFamily.value(), 0, &transferQueue);
			}

			ADGRVulkanComputeQueue ADGRVulkanComputeQueue::getQueues(VkPhysicalDevice physicalDevice)
			{
				ADGRVulkanComputeQueue _queue;

				_queue.computeFamily = getBestComputeQueue(physicalDevice);
				_queue.transferFamily = getBestTransferQueue(physicalDevice);

				return _queue;
			}

			B1 ADGRVulkanComputeQueue::isComplete()
			{
				return transferFamily.has_value() && computeFamily.has_value();
			}
		}
	}
}