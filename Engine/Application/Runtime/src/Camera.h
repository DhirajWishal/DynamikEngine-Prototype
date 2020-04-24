#pragma once
#ifndef _APPLICATION_CAMERA_H
#define _APPLICATION_CAMERA_H

#include "GameLibraries/Camera.h"
using namespace Dynamik;

class Camera : public DMKCamera {
public:
	Camera();
	~Camera();

	DMKCameraData update(ARRAY<DMKEventComponent*> eventComponents) override;
};

#endif // !_APPLICATION_CAMERA_H
