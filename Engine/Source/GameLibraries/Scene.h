#pragma once

#ifndef _DYNAMIK_SCENE_H
#define _DYNAMIK_SCENE_H

#include <string>
#include <GameObject.h>

namespace Dynamik {
	class Scene {
	public:
		Scene(std::vector<DMKGameObject*>& objects) : myGameObjects(objects) {}
		virtual ~Scene() {}

		std::string name = "";
		UI32 number = 0;
		std::vector<DMKGameObject*> myGameObjects;

		float progress = 0.0f;
	};
}

#endif
