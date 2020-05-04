#pragma once
#ifndef _STUDIO_CAMERA_H
#define _STUDIO_CAMERA_H

#include "GameLibraries/Camera.h"
using namespace Dynamik;

class Camera : public DMKCamera {
public:
	Camera();
	~Camera();

	DMKCameraData update(ARRAY<POINTER<DMKEventComponent>> eventComponents) override;
};

#endif // !_STUDIO_CAMERA_H
