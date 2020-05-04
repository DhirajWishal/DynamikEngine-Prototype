#pragma once
#ifndef _DYNAMIK_WINDOW_H
#define _DYNAMIK_WINDOW_H

#include <string>
#include "Public/datatypes.h"

namespace Dynamik {
	class DMKWindow {
	public:
		DMKWindow() {}
		DMKWindow(UI32 width, UI32 height) : windowWidth(width), windowHeight(height) {}
		virtual ~DMKWindow() {}

		UI32 windowWidth = 0, windowHeight = 0;
		std::string windowTitle = "";
	};
}

#endif // !_DYNAMIK_WINDOW_H
