#pragma once
#ifndef _DYNAMIK_RENDERER_WINDOWS_WINDOW_MANAGER_H
#define _DYNAMIK_RENDERER_WINDOWS_WINDOW_MANAGER_H

#include <deque>
#include <string>

namespace Dynamik {
	struct  DMKWindowManagerInitInfo {
		UI32 width = 1280;
		UI32 height = 720;
		std::string title = "Dynamik Engine";

		std::string iconPaths;
	};

	struct GLFWWindowHandleContainer {
		POINTER<GLFWwindow*> windowPointer;
		UI32 windowWidth = 0;
		UI32 windowHeight = 0;
	};

	class  WindowManager {
	public:
		WindowManager() {}
		~WindowManager() {}

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
		UI32 windowWidth;
		UI32 windowHeight;

		GLFWWindowHandleContainer getHandle();

		B1 isWindowCloseEvent = false;
	};
}

#endif // !_DYNAMIK_RENDERER_WINDOWS_WINDOW_MANAGER_H
