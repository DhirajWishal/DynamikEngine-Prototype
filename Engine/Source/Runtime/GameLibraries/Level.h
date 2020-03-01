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
		std::wstring name = L"";
		std::wstring ID = L"";

		float progress = 0.0f;
	};
}

#endif // !_DYNAMIK_LEVEL_H
