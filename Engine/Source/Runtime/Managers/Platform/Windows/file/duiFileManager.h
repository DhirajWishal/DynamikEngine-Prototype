#pragma once

/*
 Dynamik User Interface (DUI) file manager

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/01/2020
 IDE:		MS Visual Studio Community 2019

 Key words:
 *  # - comment
 * config - configurations (background color, image, variables, constants)
 * res - set variable (resource)
 * custom - custom definitions
 * background - set background variables
 * image - set image
 * render - rendering space (screen space)
 * div - horizontal devision of the screen
 * button - set a button
 * text - renderable text
 * align - align the element
 * action - to-do when an action is done
	* action:clicked - mouse clicked
	* action:scrolled - mouse scrolled
	* action:hover - mouse hovering
	* action:released - mouse button released
	** returns a bool (true/ false) value
 * jump - render a specific file
*/

#ifndef _DYNAMIK_PLATFORM_WINDOWS_DUI_FILE_MANAGER_H
#define _DYNAMIK_PLATFORM_WINDOWS_DUI_FILE_MANAGER_H

#include "Platform.h"

namespace Dynamik {
	namespace utils {
		class  duiFileManager {
			duiFileManager() {}
			~duiFileManager() {}
		};
	}
}

#endif // !_DYNAMIK_PLATFORM_WINDOWS_DUI_FILE_MANAGER_H
