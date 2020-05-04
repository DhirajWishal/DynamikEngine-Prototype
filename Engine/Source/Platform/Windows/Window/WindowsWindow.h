#pragma once
#ifndef _DYNAMIK_WINDOWS_WINDOW_H
#define _DYNAMIK_WINDOWS_WINDOW_H

#include <deque>
#include <string>

#include "Managers/Window/Window.h"

namespace Dynamik {
	struct  DMKWindowManagerInitInfo {
		UI32 width = 1280;
		UI32 height = 720;
		std::string title = "Dynamik Engine";

		std::string iconPaths;
	};

	class WindowsWindow : public DMKWindow {
	public:
		WindowsWindow() {}
		~WindowsWindow() {}

		void initialize(DMKWindowManagerInitInfo info);
		void terminate();

		GLFWwindow* window = nullptr;
		B1 frameBufferResized = false;

		B1 isFrameBufferResized() { return frameBufferResized; }
		void frameBufferResizedUpdate(B1 state) { frameBufferResized = state; }

		void pollEvents();
		void onUpdate();

		void setIcon(std::string paths);

		void updateWindowSize();

		B1 isWindowCloseEvent = false;
	};
}

#endif // !_DYNAMIK_RENDERER_WINDOWS_WINDOW_MANAGER_H
