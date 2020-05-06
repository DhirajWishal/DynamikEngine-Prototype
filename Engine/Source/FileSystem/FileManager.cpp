#include "dmkafx.h"
#include "FileManager.h"

#include <filesystem>

#define MAX_PATH_SIZE 256

namespace Dynamik {
	std::string FileManager::getCurrentDirectory()
	{
		return std::filesystem::current_path().generic_string();
	}
	
	std::string FileManager::getExecutableLocation()
	{
		CHR _chrBuffer[MAX_PATH_SIZE] = "";
		GetModuleFileName(NULL, _chrBuffer, MAX_PATH_SIZE);

		return _chrBuffer;
	}
}