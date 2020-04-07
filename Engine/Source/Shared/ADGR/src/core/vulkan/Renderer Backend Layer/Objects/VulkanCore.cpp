#include "adgrafx.h"
#include "VulkanCore.h"

#include "VulkanValidator.h"
#include "VulkanExtensionsManager.h"
#include "VulkanFunctions.h"
#include "defines.h"

#include "CentralDataHub.h"

#include "VulkanSwapChain.h"

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
				ADGRVulkanQueue indices = VulkanCore::findQueueFamilies(physicalDevice, surface);

				B1 extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

				B1 swapChainAdequate = false;
				if (extensionsSupported) {
					VulkanSwapChainSupportDetails swapChainSupport = VulkanSwapChain::querySwapChainSupport(&physicalDevice, &surface);
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

			// if drawing in vertex
			void drawVertex(VkCommandBuffer buffer, I32 index, ADGRVulkanRenderData* object, VkDeviceSize* offsets) {
				for (UI32 i = 0; i < object->vertexBuffers.size(); i++) {
					// bind pipeline
					vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipeline);

					// vertex buffer bind
					vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBuffers[i], offsets);

					// binding descriptor set(s)
					if (object->descriptors.descriptorSets.size())
						vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->swapChainPointer->getPipelineLayout(), 0, 1, &object->descriptors.descriptorSets[index], 0, nullptr);

					// draw command
					vkCmdDraw(buffer, object->vertexCount, 1, 0, 1);
				}
			}

			// if drawing in indexed
			void drawIndexed(VkCommandBuffer buffer, I32 index, ADGRVulkanRenderData* object, VkDeviceSize* offsets) {
				for (UI32 i = 0; i < object->vertexBuffers.size(); i++) {
					// bind pipeline
					vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipeline);

					// vertex buffer bind
					vkCmdBindVertexBuffers(buffer, 0, 1, &object->vertexBuffers[i], offsets);

					// binding descriptor set(s)
					if (object->descriptors.descriptorSets.size())
						vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineLayout, 0, 1, &object->descriptors.descriptorSets[index], 0, nullptr);

					// index buffer bind
					if (object->indexbufferObjectTypeSize == sizeof(UI8))
						vkCmdBindIndexBuffer(buffer, object->indexBuffers[i], 0, VK_INDEX_TYPE_UINT8_EXT);
					else if (object->indexbufferObjectTypeSize == sizeof(UI16))
						vkCmdBindIndexBuffer(buffer, object->indexBuffers[i], 0, VK_INDEX_TYPE_UINT16);
					else if (object->indexbufferObjectTypeSize == sizeof(UI32))
						vkCmdBindIndexBuffer(buffer, object->indexBuffers[i], 0, VK_INDEX_TYPE_UINT32);
					else if (object->indexbufferObjectTypeSize == sizeof(UI64))
						vkCmdBindIndexBuffer(buffer, object->indexBuffers[i], 0, VK_INDEX_TYPE_UINT32);

					// draw command
					vkCmdDrawIndexed(buffer, object->indexCount, 1, 0, 0, 0);
				}
			}


			void VulkanCore::initialize(ADGRVulkanCoreInitDescriptor initInfo)
			{
				initializeInstance(initInfo.instanceInitInfo);
				initializeDebugger();
				initializeSurface(initInfo.windowPointer);
				initializeDevice();
				initializeCommandPool();
				initializeSyncObjects();
			}

			void VulkanCore::terminate()
			{
				terminateSyncObjects();
			}

			void VulkanCore::initializeInstance(ADGRVulkanInstanceInitInfo info)
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

			void VulkanCore::initializeSurface(POINTER<GLFWwindow*> windowPtr)
			{
				if (glfwCreateWindowSurface(instance, windowPtr.dereference(), nullptr, &surface) != VK_SUCCESS) {
					DMK_CORE_FATAL("Failed to create window surface!");
				}
			}

			void VulkanCore::terminateInstance()
			{
				vkDestroySurfaceKHR(instance, surface, nullptr);
				vkDestroyInstance(instance, nullptr);
			}

			void VulkanCore::initializeDebugger()
			{
				if (!VulkanValidator::enableValidationLayers)
					return;

				VkDebugUtilsMessengerCreateInfoEXT createInfo;
				populateDebugMessegerCreateInfo(createInfo);

				if (createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to set up debug messenger!");
			}

			void VulkanCore::terminateDebugger()
			{
				if (VulkanValidator::enableValidationLayers)
					destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
			}

			void VulkanCore::syncFence(UI32 frame)
			{
				vkWaitForFences(logicalDevice, 1, &inFlightFences[frame], VK_TRUE, std::numeric_limits<uint64_t>::max());
			}

			VkResult VulkanCore::getNextImage(VkSwapchainKHR swapChain, POINTER<UI32> index, UI32 frame)
			{
				return vkAcquireNextImageKHR(logicalDevice, swapChain, std::numeric_limits<uint64_t>::max(),
					imageAvailables[frame], VK_NULL_HANDLE, index);
			}

			VkResult VulkanCore::submitQueues(VkSwapchainKHR swapChain, UI32 index, UI32 frame)
			{
				// submit info
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

				// wait for semaphores
				waitSemaphores[0] = { imageAvailables[frame] };
				waitStages[0] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
				submitInfo.waitSemaphoreCount = 1;
				submitInfo.pWaitSemaphores = waitSemaphores;
				submitInfo.pWaitDstStageMask = waitStages;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &commandBuffers[index];

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
				presentInfo.swapchainCount = 1;
				presentInfo.pSwapchains = &swapChain;
				presentInfo.pImageIndices = &index;

				// submit queue
				return vkQueuePresentKHR(presentQueue, &presentInfo);
			}

			void VulkanCore::populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
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

			void VulkanCore::initializeDevice()
			{
				initPhysicalDevice();
				initLogicalDevice();
			}

			void VulkanCore::terminateDevice()
			{
				vkDestroyDevice(logicalDevice, nullptr);
			}

			void VulkanCore::initPhysicalDevice()
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

						msaaSamples = VulkanFunctions::getMaxUsableSampleCount(physicalDevice);
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

			void VulkanCore::initLogicalDevice()
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

			void VulkanCore::initializeCommandPool()
			{
				ADGRVulkanQueue queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

				VkCommandPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
				poolInfo.flags = 0;

				if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create command pool!");
			}

			void VulkanCore::terminateCommandPool()
			{
				vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
			}

			ADGRVulkanQueue VulkanCore::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
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

			void VulkanCore::initializeCommandBuffers(ADGRVulkanCommandBufferInitInfo info)
			{
				commandBuffers.resize(info.count);

				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = commandPool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = static_cast<UI32>(info.count);

				if (vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate command buffers!");

				for (size_t i = 0; i < info.count; i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

					if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to begin recording command commandBuffers[i]!");

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass = info.swapChain.getRenderPass();
					renderPassInfo.framebuffer = info.swapChain.getFrameBuffer(i);
					renderPassInfo.renderArea.offset = { 0, 0 };
					renderPassInfo.renderArea.extent = info.swapChain.getSwapChainExtent();

					std::array<VkClearValue, 2> clearValues = {};
					//clearValues[0].color = {
					//	container->clearScreenValues[0],	// Red
					//	container->clearScreenValues[1],	// Green
					//	container->clearScreenValues[2],	// Blue
					//	container->clearScreenValues[3]		// Alpha
					//};

					clearValues[0].color = {
						info.clearValues[0],
						info.clearValues[1],
						info.clearValues[2],
						info.clearValues[3]
					};
					clearValues[1].depthStencil = { info.depthStencilDepth, info.stencilIndex };

					renderPassInfo.clearValueCount = static_cast<UI32>(clearValues.size());
					renderPassInfo.pClearValues = clearValues.data();

					/* BEGIN VULKAN COMMANDS */
					VkDeviceSize offsets[] = { 0 };
					// begin render pass
					vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					for (I32 _itr = 0; _itr < info.objects.size(); _itr++) {
						/* TODO: pushConstants */
						// pushConstants[0] = ...
						// vkCmdPushConstants(commandBuffers[i], &pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT,
						//		0, pushConstants.size(), pushConstants.data());
						//
						// Update light positions
						// w component = light radius scale

						/* DRAW COMMANDS */
						if (info.objects.at(_itr).swapChainPointer->getPushConstantCount())
							vkCmdPushConstants(commandBuffers[i], info.objects.at(_itr).pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, info.objects.at(_itr).swapChainPointer->pushConstants.size(), info.objects.at(_itr).swapChainPointer->pushConstants.data());

						// Render type selection
						if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_VERTEX) 		// Render as individual vertexes
							drawVertex(commandBuffers[i], i, &info.objects.at(_itr), offsets);

						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED) 		// Render as individual indexes
							drawIndexed(commandBuffers[i], i, &info.objects.at(_itr), offsets);

						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDIRECT) {
						}
						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED_INDIRECT) {
						}
						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_VERTEX) {		// Render as individual vertexes
							drawVertex(commandBuffers[i], i, &info.objects.at(_itr), offsets);
						}
						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDEXED) {		// Render as individual indexes
							drawIndexed(commandBuffers[i], i, &info.objects.at(_itr), offsets);
						}
						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDIRECT) {
						}
						else if (info.objects.at(_itr).renderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_SKYBOX_INDEXED_INDIRECT) {
						}
						else
							DMK_CORE_FATAL("Invalid rendering type!");
					}

					// end renderPass
					vkCmdEndRenderPass(commandBuffers[i]);

					/* END VULKAN COMMANDS */
					if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to record command commandBuffers[i]!");

					/*
					 Vulkan command (draw) order:
						begin render pass
						Per-object:
							init pipeline
							bind vertex buffers
							bind index buffers
							bind descriptor sets
							draw indexed data
						end render pass
						end command commandBuffers[i]
					*/
				}

			}

			void VulkanCore::terminateCommandBuffers()
			{
				vkFreeCommandBuffers(logicalDevice, commandPool, static_cast<UI32>(commandBuffers.size()), commandBuffers.data());

			}

			void VulkanCore::initializeSyncObjects()
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

			void VulkanCore::terminateSyncObjects()
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