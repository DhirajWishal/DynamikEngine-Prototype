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
			}

			void window::eventCallbackFunc(Event& event)
			{
				myEvent = &event;
			}

			void window::framebufferResizeCallback(GLFWwindow* win, int width, int height) {
				auto app = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));
				*app->frameBufferResized = true;
			}

			void window::onKeyEvent(GLFWwindow* win, int keycode, int scancode,
				int action, int mods) {
				auto data = reinterpret_cast<window*>(glfwGetWindowUserPointer(win));

				switch (action) {
				case DMK_PRESS: {
					KeyPressedEvent kp_event(keycode, 0);
					data->eventCallbackFunc(kp_event);

					break;
				}
				case DMK_RELEASE: {
					KeyReleasedEvent kr_event(keycode);
					data->eventCallbackFunc(kr_event);

					break;
				}
				case DMK_REPEAT: {
					KeyPressedEvent krep_event(keycode, 1);
					data->eventCallbackFunc(krep_event);

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
				if (glfwCreateWindowSurface(*m_instance, m_window, nullptr, m_surface) != VK_SUCCESS)
					std::runtime_error("Failed to create window surface!");
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
		}
	}
}
