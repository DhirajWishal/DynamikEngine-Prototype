#pragma once
#ifndef _DYNAMIK_CAMERA_H
#define _DYNAMIK_CAMERA_H

#include "GameObject.h"
#include "Events/EventComponent.h"
#include "GameObjectDescriptors.h"

#include <vector>

namespace Dynamik {
	/* DYNAMIK CAMERA PARENT CLASS
	 * User defined camera class for the renderer.
	 */
	class DMKCamera {
	public:
		DMKCamera() 
		{
			myData.fieldOfView = 60.0f;
			myData.cameraNear = 0.001f;
			myData.cameraFar = 256.0f;
		}
		DMKCamera(DMKCameraType ty) : type(ty) 
		{
			myData.fieldOfView = 60.0f;
			myData.cameraNear = 0.001f;
			myData.cameraFar = 256.0f;
		}
		virtual ~DMKCamera() {}

		virtual DMKCameraData update(std::vector<POINTER<DMKEventComponent>> eventComponents) { return DMKCameraData(); }
		virtual MAT4 getViewMatrix() { return MAT4(); }

	public:
		DMKCameraType type = DMKCameraType::DMK_CAMERA_TYPE_FRUSTUM;

	protected:
		DMKCameraData myData;
		VEC3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	};
}

#endif // !_DYNAMIK_CAMERA_H
