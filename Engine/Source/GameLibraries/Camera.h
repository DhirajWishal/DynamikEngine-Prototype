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
			fieldOfView = 60.0f;
			cameraNear = 0.001f;
			cameraFar = 256.0f;
		}
		DMKCamera(DMKCameraType ty) : type(ty) 
		{
			fieldOfView = 60.0f;
			cameraNear = 0.001f;
			cameraFar = 256.0f;
		}
		virtual ~DMKCamera() {}

		virtual DMKCameraData update(std::vector<POINTER<DMKEventComponent>> eventComponents) { return DMKCameraData(); }
		virtual MAT4 getViewMatrix() { return myData.viewMatrix; }
		virtual MAT4 getProjectionMatrix() { return myData.projectionMatrix; }

		virtual void setAspectRatio(F32 ratio) { aspectRatio = ratio; }
		virtual void setWindowExtent(UI32 width, UI32 height) { windowWidth = width; windowHeight = height; }

	public:
		DMKCameraType type = DMKCameraType::DMK_CAMERA_TYPE_FRUSTUM;

	protected:
		DMKCameraData myData;

		VEC3 cameraUp = { 0.0f, -1.0f, 0.0f };
		VEC3 cameraFront = { 0.0f, 0.0f, -1.0f };
		VEC3 cameraRight = { 1.0f, 0.0f, 0.0f };
		VEC3 worldUp = glm::vec3(0.0f, -1.0f, 0.0f);

		F32 fieldOfView = 45.0f;
		F32 aspectRatio = 0.5f;
		F32 cameraFar = 256.0f;
		F32 cameraNear = 0.001f;

		UI32 windowWidth = 0;
		UI32 windowHeight = 0;
	};
}

#endif // !_DYNAMIK_CAMERA_H
