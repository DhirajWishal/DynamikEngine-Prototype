#pragma once
#ifndef _DYNAMIK_ADGR_CAMERA_2D_H
#define _DYNAMIK_ADGR_CAMERA_2D_H

#include "Camera.h"

namespace Dynamik {
	namespace ADGR {
		class Camera2D : public Camera {
		public:
			Camera2D() {}
			~Camera2D() {}

			UniformBufferObject updateCamera(std::deque<DMKEventContainer> container, DMKUpdateInfo updateInfo) override;
		};
	}
}

#endif // !_DYNAMIK_ADGR_CAMERA_2D_H
