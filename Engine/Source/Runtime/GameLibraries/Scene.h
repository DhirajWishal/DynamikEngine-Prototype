#pragma once

#ifndef _DYNAMIK_SCENE_H
#define _DYNAMIK_SCENE_H

#include <vector>
#include <string>
#include <GameObject.h>

namespace Dynamik {
	class Scene {
	public:
		Scene(ARRAY<GameObject*>& objects) : myGameObjects(objects) {}
		virtual ~Scene() {}

		std::string name = "";
		UI32 number = 0;
		ARRAY<GameObject*> myGameObjects;

		float progress = 0.0f;
	};
}

#endif
