#pragma once
#ifndef _DYNAMIK_LIGHT_H
#define _DYNAMIK_LIGHT_H

#include "GameObject.h"

namespace Dynamik {
	/* *Add lighting description */
	class DMKLight : public DMKGameObject {
	public:
		DMKLight() { type = DMKObjectType::DMK_OBJECT_TYPE_LIGHT; }
		~DMKLight() {}
	};
}

#endif // !_DYNAMIK_LIGHT_H
