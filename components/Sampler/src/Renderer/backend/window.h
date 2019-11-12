#pragma once

#include "core/Window.h"
#include "Event.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct windowProperties {
				std::string title;
				unsigned int width;
				unsigned int height;
			};

			struct keyEvent {
				bool turnEventR = false;
				bool turnEventL = false;
				bool moveEventU = false;
				bool moveEventD = false;
				bool rotEventL = false;
				bool rotEventR = false;
				bool rotEventU = false;
				bool rotEventD = false;
			};

			class window : public Window {
			public:
				window() {}
				~window() {}

				void init() override;
				void clear() override;

				bool createWindowSurface();

				void pollEvents() override;
				void onUpdate() override;
				bool closeEvent() override;

				void onWindowResizeEvent();

				bool isFrameBufferResized() { return frameBufferResized; }
				void frameBufferResizedUpdate(bool state) { frameBufferResized = state; }

				keyEvent getKeyEvent();

			private:
				void keyEventHandler(int keycode);

				using eventCallbackFunction = std::function<void(Event&)>;

				GLFWwindow* m_window = windowsWindow;

				VkInstance* m_instance = &instance;
				VkSurfaceKHR* m_surface = &surface;

				const unsigned int HEIGHT = 720;
				const unsigned int WIDTH = 1280;

				bool frameBufferResized = false;

				static void framebufferResizeCallback(GLFWwindow* win, int width, int height);
				static void onKeyEvent(GLFWwindow* window, int keycode, int scancode,
					int action, int mods);
				static void onMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
				static void onMouseScrolledEvent(GLFWwindow* window, double xOffset, double yOffset);
				static void onCursorPosEvent(GLFWwindow* window, double xPos, double yPos);
				void eventCallbackFunc(Event& event);

				void setEventCallback(const eventCallbackFunction& callback)
				{
					myEventCallbackFunction = callback;
				}

				eventCallbackFunction myEventCallbackFunction;

				Event* myEvent;

				keyEvent kE;
			};
		}
	}
}
