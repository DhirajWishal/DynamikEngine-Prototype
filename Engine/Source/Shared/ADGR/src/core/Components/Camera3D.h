#pragma once
#ifndef _DYNAMIK_ADGR_CAMERA_3D_H
#define _DYNAMIK_ADGR_CAMERA_3D_H

#include "Camera.h"

namespace Dynamik {
	namespace ADGR {
		class Camera3D : public Camera {
		public:
			Camera3D() {}
			~Camera3D() {}

			UniformBufferObject updateCamera(std::deque<DMKEventContainer> container, DMKUpdateInfo updateInfo) override;
		};
	}
}

#endif // !_DYNAMIK_ADGR_CAMERA_3D_H
