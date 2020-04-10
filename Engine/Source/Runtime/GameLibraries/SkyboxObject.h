#pragma once
#ifndef _DYNAMIK_SKYBOX_OBJECT_H
#define _DYNAMIK_SKYBOX_OBJECT_H

#include "GameObject.h"

namespace Dynamik {
	class DMKSkyboxObject : public GameObject {
	public:
		DMKSkyboxObject()
		{
			myProperties.type = DMKObjectType::DMK_OBJECT_TYPE_SKYBOX;
		}
		~DMKSkyboxObject() {}
	};
}

#endif // !_DYNAMIK_SKYBOX_OBJECT_H
