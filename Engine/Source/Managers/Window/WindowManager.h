#pragma once
#ifndef _DYNAMIK_WINDOW_MANAGER_H
#define _DYNAMIK_WINDOW_MANAGER_H

#include "Window.h"

namespace Dynamik {
	typedef POINTER<DMKWindow> DMKWindowHandle;

	class DMKWindowManager {
		DMKWindowManager() {}
		~DMKWindowManager() {}

		static DMKWindowManager instance;
	public:
		DMKWindowManager() = delete;
		DMKWindowManager(const DMKWindowManager&) = delete;
		DMKWindowManager(DMKWindowManager&&) = delete;

		static void setWindowIcon(std::string path);

		static DMKWindowHandle createWindow(UI32 width = 1280U, UI32 height = 720U);

	private:
		std::string myIconPath = "";
	};
}

#endif // !_DYNAMIK_WINDOW_MANAGER_H
