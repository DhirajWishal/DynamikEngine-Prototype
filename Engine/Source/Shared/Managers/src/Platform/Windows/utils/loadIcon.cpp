#include "mngafx.h"
#include "loadIcon.h"

namespace Dynamik {
	std::fstream load_icon(std::string path) {
		std::fstream icon;
		icon.open(path, std::ios::in);

		if (!icon.is_open()) {
			std::runtime_error("Icon not found");
			return std::fstream();
		}

		return icon;
	}
}