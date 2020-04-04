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

			glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
			glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		};
	}
}

#endif // !_DYNAMIK_ADGR_CAMERA_3D_H
