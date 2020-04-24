#pragma once
#ifndef _DYNAMIK_CAMERA_H
#define _DYNAMIK_CAMERA_H

#include "GameObject.h"
#include "Events/DMKEventComponent.h"
#include "GameObjectDescriptors.h"

#include "Public/Array.h"

namespace Dynamik {
	struct DMKCameraData {
		VEC3 cameraPosition = { 0.0f, 0.0f, 0.0f };
		VEC3 cameraUp = { 0.0f, 1.0f, 0.0f };
		VEC3 cameraFront = { 1.0f, 0.0f, 0.0f };
		VEC3 cameraRight = { 1.0f, 0.0f, 0.0f };
	};

	/* DYNAMIK CAMERA PARENT CLASS
	 * User defined camera class for the renderer.
	 */
	class DMKCamera {
	public:
		DMKCamera() {}
		DMKCamera(DMKCameraType ty) : type(ty) {}
		virtual ~DMKCamera() {}

		virtual DMKCameraData update(ARRAY<DMKEventComponent*> eventComponents) { return DMKCameraData(); }
		virtual MAT4 getViewMatrix() { return MAT4(); }

	public:
		DMKCameraType type = DMKCameraType::DMK_CAMERA_TYPE_FRUSTUM;

	protected:
		DMKCameraData myData;
	};
}

#endif // !_DYNAMIK_CAMERA_H
