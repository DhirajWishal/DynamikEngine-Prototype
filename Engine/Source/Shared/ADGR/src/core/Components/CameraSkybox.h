#pragma once
#ifndef _DYNAMIK_ADGR_CAMERA_SKY_BOX_H
#define _DYNAMIK_ADGR_CAMERA_SKY_BOX_H

#include "Camera.h"

namespace Dynamik {
	namespace ADGR {
		class CameraSkybox {
		public:
			CameraSkybox() {}
			~CameraSkybox() {}

			UBO_MP updateCamera(std::deque<DMKEventContainer> container, DMKUpdateInfo updateInfo);

		private:
			glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
			glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			float _rotX = 0.0f;
			float _rotY = 0.0f;
		};
	}
}

#endif // !_DYNAMIK_ADGR_CAMERA_SKY_BOX_H
