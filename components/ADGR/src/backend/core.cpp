/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "core.h"

#include "keyCodes.h"

#define BIND_EVENT_FUNCTION(x)	std::bind(&Dynamik::ADGR::core::core::x, this, std::placeholders::_1)

namespace Dynamik {
	namespace ADGR {
		namespace core {

			core::core() {

			}

			core::~core() {

			}

			void ADGR_API core::initWindow() {

				glfwInit();

				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

				window = glfwCreateWindow(WIDTH, HEIGHT, "Dynamik Engine", nullptr, nullptr);
				glfwMakeContextCurrent(window);

				setEventCallback(BIND_EVENT_FUNCTION(onEvent));

				glfwSetWindowUserPointer(window, this);
				glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
				glfwSetKeyCallback(window, onKeyEvent);
				glfwSetMouseButtonCallback(window, onMouseButtonEvent);
				glfwSetScrollCallback(window, onMouseScrolledEvent);
				glfwSetCursorPosCallback(window, onCursorPosEvent);
			}

			void ADGR_API core::startup() {
				// create Vulkan Instance
				instance.createInstance();

				// initialize the debugger
				debugger.setupDebugMessenger();

				// create the window instance
				if (glfwCreateWindowSurface(myInstance, window, nullptr, &mySurface) != VK_SUCCESS)
					std::runtime_error("Failed to create window surface!");

				// pick the needed physical device
				device.pickPhysicalDevice(myInstance, &msaaSamples);

				// create the logical device
				device.initLogicalDevice(mySurface, &graphicsQueue, &presentQueue);

				// create the SwapChain
				swapchain.initSwapChain(*window, &swapChainImages,
					&swapChainImageFormat, &swapChainExtent);

				// initialize the image views
				swapchain.initImageViews(&swapChainImageViews, swapChainImages, swapChainImageFormat, mipLevels);

				// create the render pass
				pipeline.initRenderPass(swapChainImageFormat, myPhysicalDevice, msaaSamples);

				// create the uniform buffer descriptor
				uniformBuffer.createDescripterSetLayout();
				uniformBuffer.initDescripterPool(swapChainImages);

				// create the graphics pipeline
				auto vertShaderCode = utils::readFile(vertexShaderPath);
				auto fragShaderCode = utils::readFile(fragmentShaderPath);

				pipeline.initShaders(vertShaderCode, fragShaderCode);
				pipeline.initPipeline(swapChainExtent, &myPipelineLayout, uniformBuffer.getDescriptorSetLayout(),
					msaaSamples);
				pipeline.deleteShaders();

				// create command pool
				commandBuffer.initCommandPool(myPhysicalDevice, mySurface);

				// initialize colore buffer
				colorBuffer.initResources(swapChainImageFormat, swapChainExtent, msaaSamples, myCommandPool,
					graphicsQueue);

				// initialize the depth buffer
				depthBuffer.initBuffer(swapChainExtent, myCommandPool, graphicsQueue, mipLevels, msaaSamples);

				// create frame buffers
				swapchainFrameBuffers.initBuffer(swapChainImageViews, myRenderPass, swapChainExtent,
					depthImageView, colorImageView);

				// initialze textures
				texture.initTexture(myCommandPool, graphicsQueue);
				texture.initTextureImageView();
				texture.initTextureSampler();

				// initialize model
				myModel.loadModel(MODEL_PATH);

				// create the vertex buffer
				vertexBuffer.initBuffer(myCommandPool, graphicsQueue, myModel);

				// create index buffer
				indexBuffer.initBuffer(&vertexBuffer, myModel, myCommandPool, graphicsQueue);

				// create uniform buffer
				uniformBuffer.initBuffer(swapChainImages);
				uniformBuffer.initDescripterPool(swapChainImages);
				uniformBuffer.initDescripterSets(swapChainImages, textureImageView, textureSampler);

				// other initializing functions
				commandBuffer.initBuffer(myCommandPool, myRenderPass, swapchainFrameBuffers.getFrameBuffers(),
					swapChainExtent, graphicsPipeline, vertexBuffer.getVertexBuffer(), indexBuffer.getIndexBuffer(),
					myPipelineLayout, descriptorSets, myModel);
				initSyncObjects(myDevice, &imageAvailableSemaphores, &renderFinishedSemaphores, &inFlightFences);
			}

			void ADGR_API core::drawFrame() {
				vkWaitForFences(myDevice, 1, &inFlightFences[currentFrame],
					VK_TRUE, std::numeric_limits<uint64_t>::max());
				//vkResetFences(c_device, 1, &inFlightFences[currentFrame]);

				uint32 imageIndex;
				VkResult result = vkAcquireNextImageKHR(myDevice, mySwapchain, std::numeric_limits<uint64_t>::max(),
					imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

				if (result == VK_ERROR_OUT_OF_DATE_KHR) {
					recreateSwapChain();
					return;
				}
				else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
					std::runtime_error("failed to acquire swap chain image!");

				bool arr[2] = { turnEventL, turnEventR };
				bool mov[2] = { moveEventU, moveEventD };
				uniformBuffer.updateBuffer(imageIndex, swapChainExtent, arr, mov);

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
					std::runtime_error("failed to submit draw command buffer!");

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
					std::runtime_error("failed to present swap chain image!");

				currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

				//myWindow->onUpdate();
				myWindowHandler.onUpdate();
			}

			void ADGR_API core::shutdown() {
				// delete color buffer
				colorBuffer.deleteBuffer();

				// delete the depth buffer
				depthBuffer.deleteBuffer();

				// clean the swapchain
				swapchain.cleanUp(myDevice, swapchainFrameBuffers.getFrameBuffers(),
					myCommandPool, commandBuffer.getCommanBuffer(), graphicsPipeline, myPipelineLayout,
					myRenderPass, swapChainImageViews, swapChainImages, UniformBuffers, uniformBuffersMemory,
					descriptorPool);

				// delete the textures
				texture.deleteTexture();

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
				swapchain.initImageViews(&swapChainImageViews, swapChainImages, swapChainImageFormat, mipLevels);

				// initialize render pass
				pipeline.initRenderPass(swapChainImageFormat, myPhysicalDevice, msaaSamples);

				// initialize the graphics pipeline
				auto vertShaderCode = utils::readFile(vertexShaderPath);
				auto fragShaderCode = utils::readFile(fragmentShaderPath);

				pipeline.initShaders(vertShaderCode, fragShaderCode);
				pipeline.initPipeline(swapChainExtent, &myPipelineLayout, uniformBuffer.getDescriptorSetLayout(),
					msaaSamples);
				pipeline.deleteShaders();

				// initialize color buffer
				colorBuffer.initResources(swapChainImageFormat, swapChainExtent, msaaSamples, myCommandPool,
					graphicsQueue);

				// initialize depth buffer
				depthBuffer.initBuffer(swapChainExtent, myCommandPool, graphicsQueue, mipLevels, msaaSamples);

				// initialize the frame buffer
				swapchainFrameBuffers.initBuffer(swapChainImageViews, myRenderPass, swapChainExtent,
					depthImageView, colorImageView);

				// initialize the uniform buffer
				uniformBuffer.initBuffer(swapChainImages);
				uniformBuffer.initDescripterPool(swapChainImages);
				uniformBuffer.initDescripterSets(swapChainImages, textureImageView, textureSampler);

				// initialize the command buffer
				commandBuffer.initBuffer(myCommandPool, myRenderPass, swapchainFrameBuffers.getFrameBuffers(),
					swapChainExtent, graphicsPipeline, VertexBuffer, IndexBuffer, myPipelineLayout,
					descriptorSets, myModel);
			}

			void core::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
				auto app = reinterpret_cast<core*>(glfwGetWindowUserPointer(window));
				app->frameBufferResized = true;
			}

			// setup functions

			void core::setMipLevels(float level) {
				texture.minMipLevel = level;
			}

			void core::onEvent(Event& event) {
				printf("%p\n", event);

				EventDispatcher dispatcher(event);
			}
			using eventCallbackFunction = std::function<void(Event&)>;
			void core::eventCallbackFunc(Event& event)
			{
				//printf("%s", "I was called\n");
			}

			void core::onKeyEvent(GLFWwindow* window, int keycode, int scancode,
				int action, int mods) {
				auto data = reinterpret_cast<core*>(glfwGetWindowUserPointer(window));

				switch (action) {
				case DMK_PRESS: {
					KeyPressedEvent kp_event(keycode, 0);
					data->eventCallbackFunc(kp_event);
					data->keyEventHandler(keycode);

					break;
				}
				case DMK_RELEASE: {
					KeyReleasedEvent kr_event(keycode);
					data->eventCallbackFunc(kr_event);
					data->turnEventR = false;	// temp
					data->turnEventL = false;	// temp
					data->moveEventU = false;	// temp
					data->moveEventD = false;	// temp

					break;
				}
				case DMK_REPEAT: {
					KeyPressedEvent krep_event(keycode, 1);
					data->eventCallbackFunc(krep_event);
					data->keyEventHandler(keycode);

					break;
				}
				}
			}

			void core::onMouseButtonEvent(GLFWwindow* window, int button, int action, int mods) {
				auto data = reinterpret_cast<core*>(glfwGetWindowUserPointer(window));

				switch (action) {
				case DMK_PRESS: {
					MouseButtonPressedEvent mbp_event(button);
					data->eventCallbackFunc(mbp_event);

					break;
				}
				case DMK_RELEASE: {
					MouseButtonReleasedEvent mbr_event(button);
					data->eventCallbackFunc(mbr_event);

					break;
				}
				default:
					break;
				}
			}

			void core::onMouseScrolledEvent(GLFWwindow* window, double xOffset, double yOffset) {
				auto data = reinterpret_cast<core*>(glfwGetWindowUserPointer(window));

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data->eventCallbackFunc(event);
			}

			void core::onCursorPosEvent(GLFWwindow* window, double xPos, double yPos) {
				auto data = reinterpret_cast<core*>(glfwGetWindowUserPointer(window));

				MouseMovedEvent event((float)xPos, (float)yPos);
				data->eventCallbackFunc(event);
			}

			void core::keyEventHandler(int keycode) {

				switch (keycode) {
				case DMK_KEY_A:
					moveEventD = true;
					break;
				case DMK_KEY_B:
					break;
				case DMK_KEY_C:
					break;
				case DMK_KEY_D:
					moveEventU = true;
					break;
				case DMK_KEY_E:
					break;
				case DMK_KEY_F:
					break;
				case DMK_KEY_G:
					break;
				case DMK_KEY_H:
					break;
				case DMK_KEY_I:
					break;
				case DMK_KEY_J:
					break;
				case DMK_KEY_K:
					break;
				case DMK_KEY_L:
					break;
				case DMK_KEY_M:
					break;
				case DMK_KEY_N:
					break;
				case DMK_KEY_O:
					break;
				case DMK_KEY_P:
					break;
				case DMK_KEY_Q:
					break;
				case DMK_KEY_R:
					break;
				case DMK_KEY_S:
					turnEventR = true;
					break;
				case DMK_KEY_T:
					break;
				case DMK_KEY_U:
					break;
				case DMK_KEY_V:
					break;
				case DMK_KEY_W:
					turnEventL = true;
					break;
				case DMK_KEY_X:
					break;
				case DMK_KEY_Y:
					break;
				case DMK_KEY_Z:
					break;
				case DMK_KEY_0:
					break;
				case DMK_KEY_1:
					break;
				case DMK_KEY_2:
					break;
				case DMK_KEY_3:
					break;
				case DMK_KEY_4:
					break;
				case DMK_KEY_5:
					break;
				case DMK_KEY_6:
					break;
				case DMK_KEY_7:
					break;
				case DMK_KEY_8:
					break;
				case DMK_KEY_9:
					break;
				case DMK_KEY_UP:
					break;
				case DMK_KEY_DOWN:
					break;
				case DMK_KEY_LEFT:
					break;
				case DMK_KEY_RIGHT:
					break;

				default:
					break;
				}
			}
		}
	}
}
