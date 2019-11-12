#pragma once

#include "submitAsset.h"

namespace Dynamik {
	class InputLoader {
		InputContainer myContainer;
	public:
		void run();
		std::vector<std::string> getTexturePaths();
		std::vector<std::string> getModelPaths();
	};
}
