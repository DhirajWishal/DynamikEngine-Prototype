#pragma once

#ifndef _DYNAMIK_SCENE_H
#define _DYNAMIK_SCENE_H

#include <vector>
#include <string>
#include <GameObject.h>

namespace Dynamik {
	class Scene {
	public:
		Scene(std::vector<GameObject*>& objects) : myGameObjects(objects) {}
		virtual ~Scene() {}

		std::string name = "";
		uint32_t number = 0;
		std::vector<GameObject*> myGameObjects;

		float progress = 0.0f;
	};
}

#endif
