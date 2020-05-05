#pragma once
#ifndef _DYNAMIK_SCENE_COMPONENT_H
#define _DYNAMIK_SCENE_COMPONENT_H

#include "GameObject.h"

namespace Dynamik {
	class DMKSceneComponent : public DMKGameObject {
	public:
		DMKSceneComponent() {}
		virtual ~DMKSceneComponent() {}
	};
}

#endif // !_DYNAMIK_SCENE_COMPONENT
