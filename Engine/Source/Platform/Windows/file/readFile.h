#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_MANAGERS_WINDOWS_READ_FILE_H
#define _DYNAMIK_MANAGERS_WINDOWS_READ_FILE_H

#include <vector>
#include <string>



namespace Dynamik {
	namespace utils {
		ARRAY<char> readFile(const std::string& filePath);
	}
}

#endif // !_DYNAMIK_MANAGERS_WINDOWS_READ_FILE_H
