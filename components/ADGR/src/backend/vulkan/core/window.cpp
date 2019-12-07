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

				m_window = container->window;
			}

			void window::eventCallbackFunc(KeyPressedEvent& event) {
				myData.event = &event;
			}

			void window::framebufferResizeCallback(GLFWwindow* win, int width, int height) {
				auto app = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));
				app->frameBufferResized = true;
			}

			void window::onKeyEvent(GLFWwindow* win, int keycode, int scancode,
				int action, int mods) {
				auto data = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));

				switch (action) {
				case DMK_PRESS: {
					KeyPressedEvent kp_event(keycode, 0);
					data->keyEventHandler(keycode);

					if (keycode == DMK_KEY_ESCAPE)
						glfwSetWindowShouldClose(win, GL_TRUE);

					break;
				}
				case DMK_BUTTON_RELEASE: {
					KeyReleasedEvent kr_event(keycode);
					data->kE.turnEventR = false;	// temp
					data->kE.turnEventL = false;	// temp
					data->kE.moveEventU = false;	// temp
					data->kE.moveEventD = false;	// temp
					data->kE.rotEventL = false;		// temp
					data->kE.rotEventR = false;		// temp
					data->kE.rotEventU = false;		// temp
					data->kE.rotEventD = false;		// temp
					data->kE.reCompileShaders = false;		// temp
					data->keyCodeOne = NULL;

					break;
				}
				case DMK_REPEAT: {
					KeyPressedEvent krep_event(keycode, 1);
					data->keyEventHandler(keycode);

					break;
				}
				default:
					data->myData.event = nullptr;
					break;
				}
			}

			void window::onMouseButtonEvent(GLFWwindow* win, int button, int action, int mods) {
				auto data = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));

				switch (action) {
				case DMK_PRESS: {
					MouseButtonPressedEvent mbp_event(button);

					break;
				}
				case DMK_BUTTON_RELEASE: {
					MouseButtonReleasedEvent mbr_event(button);

					break;
				}
				default:
					data->myData.event = nullptr;
					break;
				}
			}

			void window::onMouseScrolledEvent(GLFWwindow* win, double xOffset, double yOffset) {
				auto data = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
			}

			void window::onCursorPosEvent(GLFWwindow* win, double xPos, double yPos) {
				auto data = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));
				data->cE.x = (float)xPos;
				data->cE.y = (float)yPos;

				MouseMovedEvent event((float)xPos, (float)yPos);
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

			void window::onWindowResizeEvent() {
				int width = 0, height = 0;
				while (width == 0 || height == 0) {
					glfwGetFramebufferSize(m_window, &width, &height);
					glfwWaitEvents();
				}
			}

			void window::setWindowIcon(std::string& path) {
				resource::TextureData imageData = {};

				auto pixels = imageData.loadTexture(path, resource::TEXTURE_TYPE_RGBA, true);

				GLFWimage icon[1];
				icon[0].pixels = pixels;
				glfwSetWindowIcon(m_window, 1, icon);

				imageData.freeData(pixels);
			}

			keyEvent window::getKeyEvent() {
				return kE;
			}

			cursorEvent window::getCursorEvent()
			{
				return cE;
			}

			void window::keyEventHandler(int keycode) {
				switch (keycode) {
				case DMK_KEY_A:
					kE.moveEventD = true;
					break;
				case DMK_KEY_B:
					break;
				case DMK_KEY_C:
					break;
				case DMK_KEY_D:
					kE.moveEventU = true;
					break;
				case DMK_KEY_E:
					break;
				case DMK_KEY_F:
					keyCodeOne = keycode;
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
					kE.reCompileShaders = true;
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
					kE.turnEventR = true;
					break;
				case DMK_KEY_T:
					break;
				case DMK_KEY_U:
					break;
				case DMK_KEY_V:
					break;
				case DMK_KEY_W:
					kE.turnEventL = true;
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
					kE.rotEventU = true;
					break;
				case DMK_KEY_DOWN:
					//kE.rotEventD = true;
					break;
				case DMK_KEY_LEFT:
					kE.rotEventL = true;
					break;
				case DMK_KEY_RIGHT:
					kE.rotEventR = true;
					break;
				case DMK_KEY_SPACE:
					kE.rotEventD = true;
					break;

				default:
					break;
				}
			}
		}
	}
}