#pragma once
#ifndef _DYNAMIK_WINDOW_MANAGER_H
#define _DYNAMIK_WINDOW_MANAGER_H

#include "Window.h"

namespace Dynamik {
	typedef POINTER<DMKWindow> DMKWindowHandle;

	class WindowManager {
		WindowManager() {}
		~WindowManager() {}

		static WindowManager instance;
	public:
		WindowManager(const WindowManager&) = delete;
		WindowManager(WindowManager&&) = delete;

		static void setWindowIcon(std::string path);

		static DMKWindowHandle createWindow(DMKWindowInitInfo initInfo);
		static void destroyWindow(DMKWindowHandle windowHandle);

	private:
		std::string myIconPath = "";
	};
}

#endif // !_DYNAMIK_WINDOW_MANAGER_H
