#include "adgrafx.h"
#ifdef DMK_USE_VULKAN
#include "window.h"

#include "keyEvent.h"
#include "mouseEvent.h"
#include "applicationEvent.h"
#include "keyCodes.h"

#include "Platform/windows/resource/imageLoader.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			void windowManager::init(ADGRVulkanDataContainer* container) {
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
				glfwSetWindowCloseCallback(container->window, onwindowCloseEvent);

				glfwSetInputMode(container->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

				container->window;
			}

			void windowManager::framebufferResizeCallback(GLFWwindow* win, I32 width, I32 height) {
				auto app = reinterpret_cast<windowManager*>(glfwGetWindowUserPointer(win));
				app->frameBufferResized = true;
			}

			void windowManager::onKeyEvent(GLFWwindow* win, I32 keycode, I32 scancode, I32 action, I32 mods) {
				auto data = reinterpret_cast<windowManager*>(glfwGetWindowUserPointer(win));
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

			void windowManager::onMouseButtonEvent(GLFWwindow* win, I32 button, I32 action, I32 mods) {
				auto data = reinterpret_cast<windowManager*>(glfwGetWindowUserPointer(win));
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

			void windowManager::onMouseScrolledEvent(GLFWwindow* win, double xOffset, double yOffset) {
				auto data = reinterpret_cast<windowManager*>(glfwGetWindowUserPointer(win));
				data->mouseScrolledEvent(xOffset, yOffset);
			}

			void windowManager::onCursorPosEvent(GLFWwindow* win, double xPos, double yPos) {
				auto data = reinterpret_cast<windowManager*>(glfwGetWindowUserPointer(win));
				data->mouseMovedEvent(xPos, yPos);
			}

			void windowManager::onwindowCloseEvent(GLFWwindow* window) {
			}

			B1 windowManager::createWindowSurface(ADGRVulkanDataContainer* container) {
				if (glfwCreateWindowSurface(container->instance, container->window, nullptr, &container->surface) != VK_SUCCESS) {
					DMK_CORE_FATAL("Failed to create window surface!");
					return false;
				}

				return true;
			}

			void windowManager::clear(ADGRVulkanDataContainer* container) {
				glfwDestroyWindow(container->window);
				glfwTerminate();
			}

			void windowManager::pollEvents() {
				glfwPollEvents();
			}

			void windowManager::onUpdate() {
				pollEvents();
			}

			B1 windowManager::closeEvent(ADGRVulkanDataContainer* container) {
				return glfwWindowShouldClose(container->window);
			}

			void windowManager::onWindowResizeEvent(ADGRVulkanDataContainer* container) {
				I32 width = 0, height = 0;
				while (width == 0 || height == 0) {
					glfwGetFramebufferSize(container->window, &width, &height);
					glfwWaitEvents();
				}
			}

			void windowManager::setWindowIcon(ADGRVulkanDataContainer* container, std::vector<std::string> paths) {
				UI32 _imageCount = paths.size();
				std::vector<GLFWimage> icons;
				std::vector<resource::TextureData> texDatas;
				texDatas.resize(_imageCount);

				for (UI32 i = 0; i < _imageCount; i++) {
					std::string_view path = paths[i];
					GLFWimage icon;
					auto pixels = texDatas[i].loadTexture(path.data(), resource::TEXTURE_TYPE_RGBA);

					if (!pixels)
						DMK_CORE_ERROR("Icon not Loaded!");

					icon.pixels = pixels;
					icon.height = texDatas[i].texHeight;
					icon.width = texDatas[i].texWidth;
					icons.push_back(icon);
				}

				glfwSetWindowIcon(container->window, _imageCount, icons.data());
			}

			void windowManager::keyEventHandler(DMKEventType type, I32 keycode, I32 count) {
				DMKEventContainer _localContainer = {};
				_localContainer.eventType = type;
				_localContainer.code = keycode;
				_localContainer.count = count;
				eventContainer.push_back(_localContainer);
			}

			void windowManager::mouseButtonEvent(DMKEventType type, I32 keycode, I32 count) {
				DMKEventContainer _localContainer = {};
				_localContainer.eventType = type;
				_localContainer.code = keycode;
				_localContainer.count = count;
				eventContainer.push_back(_localContainer);
			}

			void windowManager::mouseScrolledEvent(F32 xOffset, F32 yOffset) {
				DMKEventContainer _localContainer = {};
				_localContainer.eventType = DMKEventType::DMK_EVENT_TYPE_MOUSE_SCROLL;
				_localContainer.xOffset = xOffset;
				_localContainer.yOffset = yOffset;
				eventContainer.push_back(_localContainer);
			}

			void windowManager::mouseMovedEvent(F32 xOffset, F32 yOffset) {
				DMKEventContainer _localContainer = {};
				_localContainer.eventType = DMKEventType::DMK_EVENT_TYPE_MOUSE_MOVED;
				_localContainer.xAxis = xOffset;
				_localContainer.yAxis = yOffset;
				eventContainer.push_back(_localContainer);
			}

			void windowManager::_drawOnce() {
			}
		}
	}
}
#endif // DMK_USE_VULKAN
