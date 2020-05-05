#pragma once
#ifndef _DYNAMIK_WINDOW_H
#define _DYNAMIK_WINDOW_H

#include <string>
#include "Public/datatypes.h"

namespace Dynamik {
	struct DMKWindowInitInfo {
		std::string title = "Dynamik Engine v1";
		UI32 width = 1280U;
		UI32 height = 720U;
	};

	class DMKWindow {
	public:
		DMKWindow() {}
		virtual ~DMKWindow() {}

		virtual void initialize(DMKWindowInitInfo initInfo) {}
		virtual void terminate() {}
		virtual void updateWindowSize() {}
		virtual void onUpdate() {}
		virtual void pollEvents() {}
		virtual void setIcon(std::string path) {}

	public:
		std::string windowTitle = "";
		UI32 windowWidth = 0, windowHeight = 0;
	};
}

#endif // !_DYNAMIK_WINDOW_H
