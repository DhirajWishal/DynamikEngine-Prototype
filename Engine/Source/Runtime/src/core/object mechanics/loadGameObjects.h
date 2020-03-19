#pragma once

#ifndef _DYNAMIK_LOAD_GAME_OBJECTS_H
#define _DYNAMIK_LOAD_GAME_OBJECTS_H

#include "submitAsset.h"
#include "GameObject.h"

namespace Dynamik {
	class InputLoader {
		InputContainer myContainer;

		ARRAY<GameObject*> renderObjects = {};
		ARRAY<GameObject*> textureObjects = {};
	public:
		ARRAY<GameObject*> run();
		ARRAY<std::string> getModelPaths();
		ARRAY<ARRAY<std::string>> getTexturePaths();

		ARRAY<GameObject*> getRenderableGameObjects();
	};
}

#endif // !_DYNAMIK_LOAD_GAME_OBJECTS_H
