#pragma once

#ifndef _DYNAMIK_LOAD_GAME_OBJECTS_H
#define _DYNAMIK_LOAD_GAME_OBJECTS_H

#include "submitAsset.h"
#include "GameObject/GameObject.h"

namespace Dynamik {
	class InputLoader {
		InputContainer myContainer;
	public:
		std::vector<GameObject> run();
		std::vector<std::string> getModelPaths();
		std::vector<std::vector<std::string>> getTexturePaths();
	};
}

#endif // !_DYNAMIK_LOAD_GAME_OBJECTS_H
