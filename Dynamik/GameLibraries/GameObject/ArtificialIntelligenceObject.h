#pragma once

#ifndef _DYNAMIK_GAME_OBJECT_ARTIFICIAL_INTELLIGANCE_H
#define _DYNAMIK_GAME_OBJECT_ARTIFICIAL_INTELLIGANCE_H

#include "GameObject.h"

namespace Dynamik {
	// Artificial Intelligence class
	class ArtificialIntelligence : public GameObject {
	public:
		ArtificialIntelligence(GameObjectProperties& props) : GameObject(props) {}
		virtual ~ArtificialIntelligence() {}
	}; 

	class PathFinder : public ArtificialIntelligence {
	public:
		PathFinder(GameObjectProperties& props) : ArtificialIntelligence(props) {}
		virtual ~PathFinder() {}
	};
}

#endif
