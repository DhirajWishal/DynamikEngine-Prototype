#pragma once

#ifndef _DYNAMIK_ADGR_OPENGL_WINDOW_H
#define _DYNAMIK_ADGR_OPENGL_WINDOW_H
#ifdef DMK_USE_OPENGL

#include "opengl/core/data structures/opengl.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct windowProperties {
				std::string title;
				unsigned I32 width;
				unsigned I32 height;
			};

			struct keyEvent {
				B1 turnEventR = false;
				B1 turnEventL = false;
				B1 moveEventU = false;
				B1 moveEventD = false;
				B1 upDownEventU = false;
				B1 upDownEventD = false;
				B1 rotEventL = false;
				B1 rotEventR = false;
				B1 rotEventU = false;
				B1 rotEventD = false;

				B1 reCompileShaders = false;
			};

			struct cursorEvent {
				F32 x;
				F32 y;
			};

			class windowManagerGL {
			public:
				windowManagerGL() {}
				~windowManagerGL() {}

				void init(ADGROpenGLDataContainer* container);
				void clear(ADGROpenGLDataContainer* container);

				void pollEvents();
				void onUpdate();
				B1 closeEvent(ADGROpenGLDataContainer* container);

				void onwindowResizeEvent(ADGROpenGLDataContainer* container);

				void setwindowIcon(ADGROpenGLDataContainer* container, std::vector<std::string> paths);

				B1 isFrameBufferResized() { return frameBufferResized; }
				void frameBufferResizedUpdate(B1 state) { frameBufferResized = state; }

				std::deque<DMKEventContainer> getEventContainer() {
					auto container = eventContainer;
					eventContainer.clear();
					return container;
				}

			private:
				std::deque<DMKEventContainer> eventContainer = {};
				void keyEventHandler(DMKEventType type, I32 keycode = -1, I32 count = 0);
				void mouseButtonEvent(DMKEventType type, I32 keycode = -1, I32 count = 0);
				void mouseScrolledEvent(F32 xOffset = 0.0f, F32 yOffset = 0.0f);
				void mouseMovedEvent(F32 xOffset = 0.0f, F32 yOffset = 0.0f);

				void _drawOnce();

				using eventCallbackFunction = std::function<void(Event&, I32)>;

				const unsigned I32 HEIGHT = 720;
				const unsigned I32 WIDTH = 1280;

				B1 frameBufferResized = false;

				static void framebufferResizeCallback(GLFWwindow* win, I32 width, I32 height);
				static void onKeyEvent(GLFWwindow* window, I32 keycode, I32 scancode,
					I32 action, I32 mods);
				static void onMouseButtonEvent(GLFWwindow* window, I32 button, I32 action, I32 mods);
				static void onMouseScrolledEvent(GLFWwindow* window, double xOffset, double yOffset);
				static void onCursorPosEvent(GLFWwindow* window, double xPos, double yPos);
				static void onwindowCloseEvent(GLFWwindow* window);

				void setEventCallback(const eventCallbackFunction& callback) {
					myEventCallbackFunction = callback;
				}

				eventCallbackFunction myEventCallbackFunction;
			};
		}
	}
}

#endif // DMK_USE_OPENGL

#endif // !_DYNAMIK_ADGR_OPENGL_WINDOW_H

