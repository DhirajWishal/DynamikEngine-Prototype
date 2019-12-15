#pragma once

#ifndef _DYNAMIK_LEVEL_H
#define _DYNAMIK_LEVEL_H

#include <Scene.h>

namespace Dynamik {
	class Level {
	public:
		Level(std::vector<Scene*>& scenes) : myScenes(scenes) {}
		virtual ~Level() {}

		std::vector<Scene*> myScenes = {};
		std::string name = "";
		std::string ID = "";

		float progress = 0.0f;
	};
}

#endif // !_DYNAMIK_LEVEL_H

