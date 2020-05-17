#pragma once
#ifndef _DYNAMIK_DEFAULT_OBJECT_H
#define _DYNAMIK_DEFAULT_OBJECT_H

#include "GameObject.h"

namespace Dynamik
{
	class DefaultObject : public DMKGameObject {
	public:
		DefaultObject() {}
		~DefaultObject() {}

		void initialize() override;
		DMKUniformBufferData onUpdate(DMKCameraData data) override;
	};
}

#endif // !_DYNAMIK_DEFAULT_OBJECT_H
