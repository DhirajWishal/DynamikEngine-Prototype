#include "smpafx.h"
#include "window.h"

#include "keyEvent.h"
#include "mouseEvent.h"
#include "applicationEvent.h"
#include "keyCodes.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			void window::init() {
				glfwInit();
				glfwInit();

				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#ifdef DMK_DEBUG
				m_window = glfwCreateWindow(WIDTH, HEIGHT, "Dynamik Engine", nullptr, nullptr);

#else
				window = glfwCreateWindow(WIDTH, HEIGHT, "Dynamik Engine", glfwGetPrimaryMonitor(), nullptr);

#endif
				glfwMakeContextCurrent(m_window);

				//setEventCallback(BIND_EVENT_FUNCTION(onEvent));

				glfwSetWindowUserPointer(m_window, this);
				glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
				glfwSetKeyCallback(m_window, onKeyEvent);
				glfwSetMouseButtonCallback(m_window, onMouseButtonEvent);
				glfwSetScrollCallback(m_window, onMouseScrolledEvent);
				glfwSetCursorPosCallback(m_window, onCursorPosEvent);

				const GLubyte* vendor = glGetString(GL_VENDOR); // Returns the vendor
				const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model

				printf("%s\n", vendor);
				printf("%s\n", renderer);
			}

			void window::eventCallbackFunc(Event& event)
			{
				myEvent = &event;
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
					data->eventCallbackFunc(kp_event);
					data->keyEventHandler(keycode);

					break;
				}
				case DMK_RELEASE: {
					KeyReleasedEvent kr_event(keycode);
					data->eventCallbackFunc(kr_event);
					data->kE.turnEventR = false;	// temp
					data->kE.turnEventL = false;	// temp
					data->kE.moveEventU = false;	// temp
					data->kE.moveEventD = false;	// temp
					data->kE.rotEventL = false;	// temp
					data->kE.rotEventR = false;	// temp
					data->kE.rotEventU = false;	// temp
					data->kE.rotEventD = false;	// temp

					break;
				}
				case DMK_REPEAT: {
					KeyPressedEvent krep_event(keycode, 1);
					data->eventCallbackFunc(krep_event);
					data->keyEventHandler(keycode);

					break;
				}
				default:
					data->myEvent = nullptr;
					break;
				}
			}

			void window::onMouseButtonEvent(GLFWwindow* win, int button, int action, int mods) {
				auto data = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));

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
					data->myEvent = nullptr;
					break;
				}
			}

			void window::onMouseScrolledEvent(GLFWwindow* win, double xOffset, double yOffset) {
				auto data = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data->eventCallbackFunc(event);
			}

			void window::onCursorPosEvent(GLFWwindow* win, double xPos, double yPos) {
				auto data = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));

				MouseMovedEvent event((float)xPos, (float)yPos);
				data->eventCallbackFunc(event);
			}

			bool window::createWindowSurface() {
				if (glfwCreateWindowSurface(*m_instance, m_window, nullptr, m_surface) != VK_SUCCESS) {
					std::runtime_error("Failed to create window surface!");
					return false;
				}

				return true;
			}

			void window::clear() {
				glfwDestroyWindow(m_window);
				glfwTerminate();
			}

			void window::pollEvents() {
				glfwPollEvents();
			}

			void window::onUpdate() {
				pollEvents();
			}

			bool window::closeEvent() {
				return glfwWindowShouldClose(m_window);
			}

			void window::onWindowResizeEvent() {
				int width = 0, height = 0;
				while (width == 0 || height == 0) {
					glfwGetFramebufferSize(m_window, &width, &height);
					glfwWaitEvents();
				}
			}

			keyEvent window::getKeyEvent() {
				return kE;
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
					kE.rotEventD = true;
					break;
				case DMK_KEY_LEFT:
					kE.rotEventL = true;
					break;
				case DMK_KEY_RIGHT:
					kE.rotEventR = true;
					break;

				default:
					break;
				}
			}
		}
	}
}