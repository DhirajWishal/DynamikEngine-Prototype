#pragma once
#ifndef _DYNAMIK_ADGR_WINDOWS_WINDOW_MANAGER_H
#define _DYNAMIK_ADGR_WINDOWS_WINDOW_MANAGER_H

#include <deque>
#include <string>

#include "keyEvent.h"
#include "mouseEvent.h"
#include "CentralDataHub.h"
#include "Events.h"

namespace Dynamik {
	namespace ADGR {
		struct DMKWindowManagerInitInfo {
			UI32 width = 1280;
			UI32 height = 720;
			std::string title = "Dynamik Engine";

			std::string iconPaths;
		};

		class WindowManager {
		public:
			WindowManager() {}
			~WindowManager() {}

			void initialize(DMKWindowManagerInitInfo info);
			void terminate();

			GLFWwindow* window = nullptr;
			B1 frameBufferResized = false;

			std::deque<DMKEventContainer> getEventContainer() {
				auto container = eventContainer;
				eventContainer.clear();
				return container;
			}

			B1 isFrameBufferResized() { return frameBufferResized; }
			void frameBufferResizedUpdate(B1 state) { frameBufferResized = state; }

			void pollEvents();
			void onUpdate();

			void setIcon(std::string paths);

			UI32 windowWidth;
			UI32 windowHeight;

		private:
			std::deque<DMKEventContainer> eventContainer = {};
			void keyEventHandler(DMKEventType type, I32 keycode = -1, I32 count = 0);
			void mouseButtonEvent(DMKEventType type, I32 keycode = -1, I32 count = 0);
			void mouseScrolledEvent(F32 xOffset = 0.0f, F32 yOffset = 0.0f);
			void mouseMovedEvent(F32 xOffset = 0.0f, F32 yOffset = 0.0f);

			static void framebufferResizeCallback(GLFWwindow* win, I32 width, I32 height);
			static void onKeyEvent(GLFWwindow* window, I32 keycode, I32 scancode,
				I32 action, I32 mods);
			static void onMouseButtonEvent(GLFWwindow* window, I32 button, I32 action, I32 mods);
			static void onMouseScrolledEvent(GLFWwindow* window, double xOffset, double yOffset);
			static void onCursorPosEvent(GLFWwindow* window, double xPos, double yPos);
			static void onwindowCloseEvent(GLFWwindow* window);
		};
	}
}

#endif // !_DYNAMIK_ADGR_WINDOWS_WINDOW_MANAGER_H
