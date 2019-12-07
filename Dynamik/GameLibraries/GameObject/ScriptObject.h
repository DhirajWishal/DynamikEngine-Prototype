#pragma once

#ifndef _DYNAMIK_GAME_OBJECT_SCRIPT_H
#define _DYNAMIK_GAME_OBJECT_SCRIPT_H

#include "GameObject.h"

namespace Dynamik {
	// Script class
	class Script : public GameObject {
	public:
		Script(GameObjectProperties& props) : GameObject(props) {}
		virtual ~Script() {}
	};
}

#endif
