#pragma once
#ifndef _DYNAMIK_FILE_MANAGER_H
#define _DYNAMIK_FILE_MANAGER_H

#include <string>

namespace Dynamik {
	class FileManager {
		FileManager() {}
		~FileManager() {}

	public:
		static std::string getExecutableLocation();
	};
}

#endif // !_DYNAMIK_FILE_MANAGER_H
