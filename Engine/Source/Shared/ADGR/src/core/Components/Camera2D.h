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

			glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
			glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			float _rotX = 0.0f;
			float _rotY = 0.0f;
		};
	}
}

#endif // !_DYNAMIK_ADGR_CAMERA_2D_H
