#pragma once

#ifndef _DYNAMIK_LOAD_GAME_OBJECTS_H
#define _DYNAMIK_LOAD_GAME_OBJECTS_H

#include "submitAsset.h"
#include "GameObject.h"

namespace Dynamik {
	class InputLoader {
		InputContainer myContainer;

		std::vector<GameObject*> renderObjects = {};
		std::vector<GameObject*> textureObjects = {};
	public:
		std::vector<GameObject*> run();
		std::vector<std::wstring> getModelPaths();
		std::vector<std::vector<std::wstring>> getTexturePaths();

		std::vector<GameObject*> getRenderableGameObjects();
	};
}

#endif // !_DYNAMIK_LOAD_GAME_OBJECTS_H
