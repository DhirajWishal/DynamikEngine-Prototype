/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			core::core() {

			}

			core::~core() {

			}

			void ADGR_API core::startup() {
				// create Vulkan Instance
				instance.createInstance();

				// initialize the debugger
				debugger.setupDebugMessenger();

				// create the window instance
				if (glfwCreateWindowSurface(myInstance, window, nullptr, &mySurface) != VK_SUCCESS)
					throw std::runtime_error("Failed to create window surface!");

				// pick the needed physical device
				device.pickPhysicalDevice(myInstance);

				// create the logical device
				device.initLogicalDevice(mySurface, &graphicsQueue, &presentQueue);

				// create the SwapChain
				swapchain.initSwapChain(*window, &swapChainImages,
					&swapChainImageFormat, &swapChainExtent);

				// initialize the image views
				swapchain.initImageViews(&swapChainImageViews, swapChainImages, swapChainImageFormat);

				// create the render pass
				pipeline.initRenderPass(swapChainImageFormat);

				// create the uniform buffer descriptor
				uniformBuffer.createDescripterSetLayout();
				uniformBuffer.initDescripterPool(swapChainImages);

				// create the graphics pipeline
				auto vertShaderCode = utils::readFile(vertexShaderPath);
				auto fragShaderCode = utils::readFile(fragmentShaderPath);

				pipeline.initShaders(vertShaderCode, fragShaderCode);
				pipeline.initPipeline(swapChainExtent, &myPipelineLayout, uniformBuffer.getDescriptorSetLayout());
				pipeline.deleteShaders();

				// create frame buffers
				swapchainFrameBuffers.initBuffer(swapChainImageViews, myRenderPass, swapChainExtent);

				// create command pool
				commandBuffer.initCommandPool(myPhysicalDevice, mySurface);

				// create the vertex buffer
				vertexBuffer.initBuffer(myCommandPool, graphicsQueue);

				// create index buffer
				indexBuffer.initBuffer(&vertexBuffer, myCommandPool, graphicsQueue);

				// create uniform buffer
				uniformBuffer.initBuffer(swapChainImages);
				uniformBuffer.initDescripterPool(swapChainImages);
				uniformBuffer.initDescripterSets(swapChainImages);

				myPipelineLayout;

				// other initializing functions
				commandBuffer.initBuffer(myCommandPool, myRenderPass, swapchainFrameBuffers.getFrameBuffers(),
					swapChainExtent, graphicsPipeline, vertexBuffer.getVertexBuffer(), indexBuffer.getIndexBuffer(),
					myPipelineLayout, descriptorSets);
				initSyncObjects(myDevice, &imageAvailableSemaphores, &renderFinishedSemaphores, &inFlightFences);
			}

			void ADGR_API core::shutdown() {
				// clean the swapchain
				swapchain.cleanUp(myDevice, swapchainFrameBuffers.getFrameBuffers(),
					myCommandPool, commandBuffer.getCommanBuffer(), graphicsPipeline, myPipelineLayout,
					myRenderPass, swapChainImageViews, swapChainImages, UniformBuffers, uniformBuffersMemory,
					descriptorPool);

				// delete index and vertex buffers
				indexBuffer.deleteIndexBuffer();
				vertexBuffer.deleteVertexBuffer();

				// delete frames in flight
				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					vkDestroySemaphore(myDevice, renderFinishedSemaphores[i], nullptr);
					vkDestroySemaphore(myDevice, imageAvailableSemaphores[i], nullptr);
				}

				// stop the debugger
				debugger.deleteDebugger();

				// delete the command pool
				vkDestroyCommandPool(myDevice, myCommandPool, nullptr);

				// delete device and surface
				vkDestroyDevice(myDevice, nullptr);
				vkDestroySurfaceKHR(myInstance, mySurface, nullptr);

				// delete the vulkan instance
				instance.deleteInstance();

				// distroy the window
				glfwDestroyWindow(window);
				glfwTerminate();
			}

			void ADGR_API core::recreateSwapChain() {
				// get new width and height
				int width = 0, height = 0;
				while (width == 0 || height == 0) {
					glfwGetFramebufferSize(window, &width, &height);
					glfwWaitEvents();
				}

				vkDeviceWaitIdle(myDevice);

				// cleanup the swapchain
				swapchain.cleanUp(myDevice, swapchainFrameBuffers.getFrameBuffers(),
					myCommandPool, commandBuffer.getCommanBuffer(), graphicsPipeline, myPipelineLayout,
					myRenderPass, swapChainImageViews, swapChainImages, UniformBuffers, uniformBuffersMemory,
					descriptorPool);

				// initialize the swapchain
				swapchain.initSwapChain(*window, &swapChainImages,
					&swapChainImageFormat, &swapChainExtent);

				// initialize image views
				swapchain.initImageViews(&swapChainImageViews, swapChainImages, swapChainImageFormat);

				// initialize render pass
				pipeline.initRenderPass(swapChainImageFormat);

				// initialize the graphics pipeline
				auto vertShaderCode = utils::readFile(vertexShaderPath);
				auto fragShaderCode = utils::readFile(fragmentShaderPath);

				pipeline.initShaders(vertShaderCode, fragShaderCode);
				pipeline.initPipeline(swapChainExtent, &myPipelineLayout, uniformBuffer.getDescriptorSetLayout());
				pipeline.deleteShaders();

				// initialize the frame buffer
				swapchainFrameBuffers.initBuffer(swapChainImageViews, myRenderPass, swapChainExtent);

				// initialize the uniform buffer
				uniformBuffer.initBuffer(swapChainImages);
				uniformBuffer.initDescripterPool(swapChainImages);
				uniformBuffer.initDescripterSets(swapChainImages);

				// initialize the command buffer
				commandBuffer.initBuffer(myCommandPool, myRenderPass, swapchainFrameBuffers.getFrameBuffers(),
					swapChainExtent, graphicsPipeline, VertexBuffer, IndexBuffer, myPipelineLayout,
					descriptorSets);
			}

			void ADGR_API core::drawFrame() {
				vkWaitForFences(myDevice, 1, &inFlightFences[currentFrame],
					VK_TRUE, std::numeric_limits<uint64_t>::max());
				//vkResetFences(c_device, 1, &inFlightFences[currentFrame]);

				uint32_t imageIndex;
				VkResult result = vkAcquireNextImageKHR(myDevice, mySwapchain, std::numeric_limits<uint64_t>::max(),
					imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

				if (result == VK_ERROR_OUT_OF_DATE_KHR) {
					recreateSwapChain();
					return;
				}
				else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
					throw std::runtime_error("failed to acquire swap chain image!");

				uniformBuffer.updateBuffer(imageIndex, swapChainExtent, currentFrame);

				// **********	**********	**********	**********	**********
				/*				static auto startTime = std::chrono::high_resolution_clock::now();

				auto currentTime = std::chrono::high_resolution_clock::now();
				float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

				UniformBufferObject ubo = {};
				ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
				ubo.proj[1][1] *= -1;

				void* data;
				vkMapMemory(myDevice, uniformBuffersMemory.at(currentFrame), 0, sizeof(ubo), 0, &data);
				memcpy(data, &ubo, sizeof(ubo));
				vkUnmapMemory(myDevice, uniformBuffersMemory.at(currentFrame));
				*/
				// **********	**********	**********	**********	**********

				VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

				VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
				VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
				submitInfo.waitSemaphoreCount = 1;
				submitInfo.pWaitSemaphores = waitSemaphores;
				submitInfo.pWaitDstStageMask = waitStages;

				submitInfo.commandBufferCount = 1; 
				submitInfo.pCommandBuffers = commandBuffer.getFrame(imageIndex);

				VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
				submitInfo.signalSemaphoreCount = 1;
				submitInfo.pSignalSemaphores = signalSemaphores;

				vkResetFences(myDevice, 1, &inFlightFences[currentFrame]);

				if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
					throw std::runtime_error("failed to submit draw command buffer!");

				VkPresentInfoKHR presentInfo = {};
				presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

				presentInfo.waitSemaphoreCount = 1;
				presentInfo.pWaitSemaphores = signalSemaphores;

				VkSwapchainKHR swapChains[] = { mySwapchain };
				presentInfo.swapchainCount = 1;
				presentInfo.pSwapchains = swapChains;
				presentInfo.pImageIndices = &imageIndex;

				result = vkQueuePresentKHR(presentQueue, &presentInfo);

				if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || frameBufferResized) {
					frameBufferResized = false;
					recreateSwapChain();
				}
				else if (result != VK_SUCCESS)
					throw std::runtime_error("failed to present swap chain image!");

				currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
			}

			void ADGR_API core::initWindow() {
				glfwInit();

				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

				window = glfwCreateWindow(WIDTH, HEIGHT, "Dynamik Engine", nullptr, nullptr);
				glfwSetWindowUserPointer(window, this);
				glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

				GLFWimage icon;
				//icon = load_icon("E:/Projects/Dynamik Engine/Dynamik/core assets/icons/icon1.png")
				glfwSetWindowIcon(window, 0, NULL);
			}

			void core::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
				auto app = reinterpret_cast<core*>(glfwGetWindowUserPointer(window));
				app->frameBufferResized = true;
			}
		}
	}
}
