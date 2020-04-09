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

			UniformBufferObject updateCamera(std::deque<DMKEventContainer> container, DMKUpdateInfo updateInfo, B1 viewMatrixLock = false) override;

			Camera_OpenGL glCam{ glm::vec3(0.0f, 0.0f, 3.0f) };
			float lastX = 0.0f;
			float lastY = 0.0f;
			bool firstMouse = true;

			float deltaTime = 0.0f;
			float lastFrame = 0.0f;
		};
	}
}

#endif // !_DYNAMIK_ADGR_CAMERA_3D_H
