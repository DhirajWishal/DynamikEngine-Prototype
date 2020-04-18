#pragma once
#ifndef _DYNAMIK_ADGR_ANIMATION_CAMERA_H
#define _DYNAMIK_ADGR_ANIMATION_CAMERA_H

#include "Camera.h"

namespace Dynamik {
	namespace ADGR {
		class AnimationCamera : public Camera {
		public:
			AnimationCamera() {}
			~AnimationCamera() {}

			UBO_SKELETAL updateCamera(std::deque<DMKEventContainer> container, DMKUpdateInfo updateInfo);

		private:
			glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
			glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			float _rotX = 0.0f;
			float _rotY = 0.0f;

			Camera_OpenGL glCam{ glm::vec3(0.0f, 0.0f, 3.0f) };
			float lastX = 0.0f;
			float lastY = 0.0f;
			bool firstMouse = true;
			float zoom = 45.0f;
			float deltaTime = 0.0f;
			float lastFrame = 0.0f;
		};
	}
}

#endif // !_DYNAMIK_ADGR_ANIMATION_CAMERA_H
