#pragma once
#ifndef _DYNAMIK_CAMERA_H
#define _DYNAMIK_CAMERA_H

#include "GameObject.h"
#include "core/Interface/Events/DMKEventComponent.h"

namespace Dynamik {
	struct DMKCameraData {

	};

	class Camera {
	public:
		Camera() {}
		virtual ~Camera() {}
	};
}

#endif // !_DYNAMIK_CAMERA_H
