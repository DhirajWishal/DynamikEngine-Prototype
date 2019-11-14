#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_PLATFORM_WINDOWS_READ_FILE_H
#define _DYNAMIK_PLATFORM_WINDOWS_READ_FILE_H

namespace Dynamik {
	namespace ADGR {
		namespace utils {
			std::vector<char> readFile(const std::string& filePath);
		}
	}
}

#endif // !_DYNAMIK_PLATFORM_WINDOWS_READ_FILE_H
