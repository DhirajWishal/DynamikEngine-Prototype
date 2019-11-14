#pragma once

#ifndef _DYNAMIK_LOAD_GAME_OBJECTS_H
#define _DYNAMIK_LOAD_GAME_OBJECTS_H

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

#endif // !_DYNAMIK_LOAD_GAME_OBJECTS_H
