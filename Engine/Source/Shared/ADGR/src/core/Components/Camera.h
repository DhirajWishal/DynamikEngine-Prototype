#pragma once
#ifndef _DYNAMIK_ADGR_CAMERA_H
#define _DYNAMIK_ADGR_CAMERA_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "UniformBufferObject.h"
#include "eventContainer.h"

namespace Dynamik {
	namespace ADGR {
		class Camera {
		public:
			Camera() {}
			virtual ~Camera() {}

			void update();

			virtual UniformBufferObject updateCamera(std::deque<DMKEventContainer> container, DMKUpdateInfo updateInfo) { return UniformBufferObject(); }

		protected:
			glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
			glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			float _rotX = 0.0f;
			float _rotY = 0.0f;
		};
	}
}

#endif // !_DYNAMIK_ADGR_CAMERA_H
