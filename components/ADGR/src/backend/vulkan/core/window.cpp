#include "adgrafx.h"
#include "window.h"

#include "keyEvent.h"
#include "mouseEvent.h"
#include "applicationEvent.h"
#include "keyCodes.h"

#include "Platform/Windows/resource/imageLoader.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			void window::init(ADGRVulkanDataContainer* container) {
				glfwInit();

				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#if defined(DMK_DEBUG)
				container->window = glfwCreateWindow(WIDTH, HEIGHT, "Dynamik Engine", nullptr, nullptr);

#else
				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);

				glfwWindowHint(GLFW_RED_BITS, mode->redBits);
				glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
				glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
				glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

				container->window = glfwCreateWindow(mode->width, mode->height, "Dynamik Engine", monitor, NULL);

#endif
				glfwMakeContextCurrent(container->window);

				glfwSetWindowUserPointer(container->window, this);
				glfwSetFramebufferSizeCallback(container->window, framebufferResizeCallback);
				glfwSetKeyCallback(container->window, onKeyEvent);
				glfwSetMouseButtonCallback(container->window, onMouseButtonEvent);
				glfwSetScrollCallback(container->window, onMouseScrolledEvent);
				glfwSetCursorPosCallback(container->window, onCursorPosEvent);
				glfwSetWindowCloseCallback(container->window, onWindowCloseEvent);

				glfwSetInputMode(container->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

				container->window;
			}

			void window::framebufferResizeCallback(GLFWwindow* win, int width, int height) {
				auto app = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));
				app->frameBufferResized = true;
			}

			void window::onKeyEvent(GLFWwindow* win, int keycode, int scancode, int action, int mods) {
				auto data = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));
				data->keyEventHandler(DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT, -1);

				switch (action) {
				case DMK_PRESS: {
					data->keyEventHandler(DMKEventType::DMK_EVENT_TYPE_KEY_PRESS, keycode);

					if (keycode == DMK_KEY_ESCAPE)
						glfwSetWindowShouldClose(win, GL_TRUE);
					break;
				}
				case DMK_BUTTON_RELEASE: {
					data->keyEventHandler(DMKEventType::DMK_EVENT_TYPE_KEY_RELEASE, keycode);
					break;
				}
				case DMK_REPEAT: {
					data->keyEventHandler(DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT, keycode, 1);
					break;
				}
				}
			}

			void window::onMouseButtonEvent(GLFWwindow* win, int button, int action, int mods) {
				auto data = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));
				data->mouseButtonEvent(DMKEventType::DMK_EVENT_TYPE_UNKNOWN, -1);

				switch (action) {
				case DMK_PRESS: {
					data->mouseButtonEvent(DMKEventType::DMK_EVENT_TYPE_MOUSE_PRESS, button);
					break;
				}
				case DMK_BUTTON_RELEASE: {
					data->mouseButtonEvent(DMKEventType::DMK_EVENT_TYPE_MOUSE_RELEASE, button);
					break;
				}
				}
			}

			void window::onMouseScrolledEvent(GLFWwindow* win, double xOffset, double yOffset) {
				auto data = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));
				data->mouseScrolledEvent(xOffset, yOffset);
			}

			void window::onCursorPosEvent(GLFWwindow* win, double xPos, double yPos) {
				auto data = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));
				data->mouseMovedEvent(xPos, yPos);
			}

			void window::onWindowCloseEvent(GLFWwindow* window) {
			}

			bool window::createWindowSurface(ADGRVulkanDataContainer* container) {
				if (glfwCreateWindowSurface(container->instance, container->window, nullptr, &container->surface) != VK_SUCCESS) {
					DMK_CORE_FATAL("Failed to create window surface!");
					return false;
				}

				return true;
			}

			void window::clear(ADGRVulkanDataContainer* container) {
				glfwDestroyWindow(container->window);
				glfwTerminate();
			}

			void window::pollEvents() {
				glfwPollEvents();
			}

			void window::onUpdate() {
				pollEvents();
			}

			bool window::closeEvent(ADGRVulkanDataContainer* container) {
				return glfwWindowShouldClose(container->window);
			}

			void window::onWindowResizeEvent(ADGRVulkanDataContainer* container) {
				int width = 0, height = 0;
				while (width == 0 || height == 0) {
					glfwGetFramebufferSize(container->window, &width, &height);
					glfwWaitEvents();
				}
			}

			void window::setWindowIcon(ADGRVulkanDataContainer* container, std::string& path) {
				resource::TextureData imageData = {};

				auto pixels = imageData.loadTexture(path, resource::TEXTURE_TYPE_RGBA, true);

				GLFWimage icon[1];
				icon[0].pixels = pixels;
				glfwSetWindowIcon(container->window, 1, icon);

				imageData.freeData(pixels);
			}

			void window::keyEventHandler(DMKEventType type, int keycode, int count) {
				DMKEventContainer _localContainer = {};
				_localContainer.eventType = type;
				_localContainer.code = keycode;
				_localContainer.count = count;
				eventContainer.push_back(_localContainer);
			}

			void window::mouseButtonEvent(DMKEventType type, int keycode, int count) {
				DMKEventContainer _localContainer = {};
				_localContainer.eventType = type;
				_localContainer.code = keycode;
				_localContainer.count = count;
				eventContainer.push_back(_localContainer);
			}

			void window::mouseScrolledEvent(float xOffset, float yOffset) {
				DMKEventContainer _localContainer = {};
				_localContainer.eventType = DMKEventType::DMK_EVENT_TYPE_MOUSE_SCROLL;
				_localContainer.xOffset = xOffset;
				_localContainer.yOffset = yOffset;
				eventContainer.push_back(_localContainer);
			}

			void window::mouseMovedEvent(float xOffset, float yOffset) {
				DMKEventContainer _localContainer = {};
				_localContainer.eventType = DMKEventType::DMK_EVENT_TYPE_MOUSE_MOVED;
				_localContainer.xAxis = xOffset;
				_localContainer.yAxis = yOffset;
				eventContainer.push_back(_localContainer);
			}
		}
	}
}