#pragma once

#ifndef _DYNAMIK_GAME_OBJECT_DESTRUCTION_H
#define _DYNAMIK_GAME_OBJECT_DESTRUCTION_H

#include "GameObject.h"

namespace Dynamik {
	// Destruction class
	class Destruction : public GameObject {
	public:
		Destruction(GameObjectProperties& props) : GameObject(props) {}
		virtual ~Destruction() {}

		virtual void onActivation(std::string& playerID, GameObjectProperties& objectProps) {}
	};

	class Explosion : public Destruction {
	public:
		Explosion(GameObjectProperties& props) : Destruction(props) {}
		virtual ~Explosion() {}
	};

	class Penetration : public Destruction {
	public:
		Penetration(GameObjectProperties& props) : Destruction(props) {}
		virtual ~Penetration() {}
	};
}

#endif
