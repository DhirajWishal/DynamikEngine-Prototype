#pragma once
#ifndef _DYNAMIK_SKYBOX_OBJECT_H
#define _DYNAMIK_SKYBOX_OBJECT_H

#include "GameObject.h"

namespace Dynamik {
	class DMKSkyboxObject : public DMKGameObject {
	public:
		DMKSkyboxObject()
		{
			type = DMKObjectType::DMK_OBJECT_TYPE_SKYBOX;
		}
		~DMKSkyboxObject() {}
	};
}

#endif // !_DYNAMIK_SKYBOX_OBJECT_H
