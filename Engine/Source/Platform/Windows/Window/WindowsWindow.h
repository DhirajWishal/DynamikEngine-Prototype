#pragma once
#ifndef _DYNAMIK_WINDOWS_WINDOW_H
#define _DYNAMIK_WINDOWS_WINDOW_H

#include <deque>
#include <string>

#include "Managers/Window/Window.h"

namespace Dynamik {
	class WindowsWindow : public DMKWindow {
	public:
		WindowsWindow() {}
		~WindowsWindow() {}

		void initialize(DMKWindowInitInfo info) override final;
		void terminate() override final;

		void pollEvents() override final;
		void onUpdate() override final;

		void setIcon(std::string paths) override final;

		void updateWindowSize() override final;

		GLFWwindow* window = nullptr;
	};
}

#endif // !_DYNAMIK_RENDERER_WINDOWS_WINDOW_MANAGER_H
