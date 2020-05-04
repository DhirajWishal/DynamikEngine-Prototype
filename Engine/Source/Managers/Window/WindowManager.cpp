#include "dmkafx.h"
#include "WindowManager.h"

namespace Dynamik {
	DMKWindowManager DMKWindowManager::instance;

	void DMKWindowManager::setWindowIcon(std::string path)
	{
		instance.myIconPath = path;
	}

	DMKWindowHandle DMKWindowManager::createWindow(UI32 width, UI32 height)
	{
		return POINTER<DMKWindow>();
	}
}