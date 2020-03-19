/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "mngafx.h"
#include "readFile.h"

namespace Dynamik {
	namespace utils {
		ARRAY<char> readFile(const std::string& filePath) {
			std::ifstream file(filePath, std::ios::ate | std::ios::binary);

			if (!file.is_open())
				throw std::runtime_error("Failed to Open file!");

			size_t fileSize = (size_t)file.tellg();
			ARRAY<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();
			return buffer;
		}
	}
}