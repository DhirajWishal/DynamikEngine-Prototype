#pragma once
#ifndef _DYNAMIK_ANIMATED_OBJECT_H
#define _DYNAMIK_ANIMATED_OBJECT_H

#include "GameObject.h"

namespace Dynamik {
	/*
	 Templated Animated object for the Dynamik Engine.
	*/
	template<UI32 BoneCount = 100>
	class DMKAnimatedObject : public DMKGameObject {
	public:
		DMKAnimatedObject() {}
		virtual ~DMKAnimatedObject() {}
	};
}

#endif // !_DYNAMIK_ANIMATED_OBJECT_H
