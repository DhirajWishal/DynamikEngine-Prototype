#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_PLATFORM_WINDOWS_WINDOW_H
#define _DYNAMIK_PLATFORM_WINDOWS_WINDOW_H

//#ifndef UNICODE
//#define UNICODE
//#endif 

//#include <windows.h>

#include <string>
#include "CentralDataHub.h"

namespace Dynamik {
	struct windowProperties {
		std::wstring title = {};
		unsigned int width = 0;
		unsigned int height = 0;

		bool vSync = false;

		windowProperties(
			const std::wstring& title = DMK_TEXT("Dynamik Engine"),
			unsigned int width = 1280,
			unsigned int height = 720
		) : title(title), width(width), height(height) {
		}
	};

	class Window {
	public:
		Window(windowProperties props) :
			myProps(props) {}
		Window() {}
		~Window() {}

		//void init(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow);

	private:
		windowProperties myProps = {};
	};
}

#endif // !_DYNAMIK_PLATFORM_WINDOWS_WINDOW_H
