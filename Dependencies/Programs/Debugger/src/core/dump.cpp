#include <fstream>
#include <iostream>
#include <string>
#include "log.h"

#include "dump.h"

namespace Dynamik {
	void dumpData(std::string msg) {
		std::fstream file("debug_dump.txt", std::ios::app);

		if (!file.is_open())
			DMK_CORE_ERROR("DEBUGGER DATA DUMPER FILE NOT OPEN!");
		else
			file << msg.c_str();

		file.close();
	}
}