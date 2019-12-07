#pragma once

#ifndef _DYNAMIK_GAME_OBJECT_DESTRUCTOR_H
#define _DYNAMIK_GAME_OBJECT_DESTRUCTOR_H

#include "GameObject.h"

namespace Dynamik {
	// Destructor class
	class Destructor : public GameObject {
	public:
		Destructor(GameObjectProperties& props) : GameObject(props) {}
		~Destructor() {}

		virtual void onLoad() {}
		virtual void onActivation(std::string playerID, GameObjectProperties& weaponProps) {}
		virtual void inAir() {}
		virtual void onImpact(GameObjectProperties& objectProps) {}
	};

	struct ProjectileProperties {
		GameObjectProperties& objectProps = {};

		std::string name = "";
		std::string playerID = "";

		float initialVelocity = 0.0f;
		float acceleration = 0.0f;
		float deceleration = 0.0f;
		float currentVelocity = 0.0f;
		float weight = 0.0f;
		float size = 1.0f;
	};

	class Projectile : public Destructor {
	public:
		Projectile(ProjectileProperties& props) : Destructor(props.objectProps) {}
		virtual ~Projectile() {}
	};

	class Explosives : public Destructor {
	public:
		Explosives(GameObjectProperties& props) : Destructor(props) {}
		virtual ~Explosives() {}
	};
}

#endif
