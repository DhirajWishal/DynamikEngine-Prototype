#pragma once
#ifndef _DYNAMIK_CAMERA_H
#define _DYNAMIK_CAMERA_H

#include "GameObject.h"
#include "Events/DMKEventComponent.h"

#include "Public/Array.h"

namespace Dynamik {
	struct DMKCameraData {
		VEC3 cameraPosition = { 0.0f, 0.0f, 0.0f };
		VEC3 cameraUp = { 0.0f, 1.0f, 0.0f };
		VEC3 cameraRight = { 1.0f, 0.0f, 0.0f };
	};

	enum class DMKCameraType {
		DMK_CAMERA_TYPE_FRUSTUM,
		DMK_CAMERA_TYPE_ORTHOGRAPHIC,
	};

	/* DYNAMIK CAMERA PARENT CLASS
	 * User defined camera class for the renderer.
	 */
	class Camera {
	public:
		Camera() {}
		Camera(DMKCameraType ty) : type(ty) {}
		virtual ~Camera() {}

		virtual DMKCameraData update(ARRAY<DMKEventComponent*> eventComponents) {}
		virtual MAT4 getViewMatrix() {}

	public:
		DMKCameraType type = DMKCameraType::DMK_CAMERA_TYPE_FRUSTUM;

	protected:
		DMKCameraData myData;
	};
}

#endif // !_DYNAMIK_CAMERA_H
