#include "dmkafx.h"
#include "WindowManager.h"

#include "Platform/Windows/Window/WindowsWindow.h"

namespace Dynamik {
	WindowManager WindowManager::instance;

	void WindowManager::setWindowIcon(std::string path)
	{
		instance.myIconPath = path;
	}

	DMKWindowHandle WindowManager::createWindow(DMKWindowInitInfo initInfo)
	{
		WindowsWindow _window;
		_window.initialize(initInfo);

		DMKWindowHandle _windowHandle = StaticAllocator<WindowsWindow>::allocate();
		StaticAllocator<WindowsWindow>::set(_windowHandle, (WindowsWindow&&)_window);
		return _windowHandle;
	}

	void WindowManager::destroyWindow(DMKWindowHandle windowHandle)
	{
		WindowsWindow _window = *(WindowsWindow*)windowHandle.get();
		_window.terminate();
		StaticAllocator<DMKWindow>::deAllocate(windowHandle);
	}
}