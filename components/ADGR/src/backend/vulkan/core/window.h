#pragma once

#include "core/Window.h"

#include "keyEvent.h"
#include "mouseEvent.h"

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
				bool upDownEventU = false;
				bool upDownEventD = false;
				bool rotEventL = false;
				bool rotEventR = false;
				bool rotEventU = false;
				bool rotEventD = false;
			};

			struct cursorEvent {
				float x;
				float y;
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
				cursorEvent getCursorEvent();

				std::tuple<int, mouseEventData*> getEvent() { 
					std::tuple<int, mouseEventData*> data = { keyCodeOne, med };
					return data;
				}

			private:
				void keyEventHandler(int keycode);

				using eventCallbackFunction = std::function<void(Event&, int)>;

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

				void eventCallbackFunc(KeyPressedEvent& event);
				void eventCallbackFunc(KeyReleasedEvent& event);
				void eventCallbackFunc(MouseButtonPressedEvent& event);
				void eventCallbackFunc(MouseButtonReleasedEvent& event);
				void eventCallbackFunc(MouseScrolledEvent& event);
				void eventCallbackFunc(MouseMovedEvent& event);

				void setEventCallback(const eventCallbackFunction& callback)
				{
					myEventCallbackFunction = callback;
				}

				eventCallbackFunction myEventCallbackFunction;

				eventData myData;

				keyEvent kE;
				cursorEvent cE;

				//codes
				mouseEventData* med;
				keyEventData* ked;

				int keyCodeOne;
			};
		}
	}
}
