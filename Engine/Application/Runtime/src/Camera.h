#pragma once
#ifndef _APPLICATION_CAMERA_H
#define _APPLICATION_CAMERA_H

#include "GameLibraries/Camera.h"
#include "Events/EventManager.h"
using namespace Dynamik;

class Camera : public DMKCamera {
public:
    Camera();
    ~Camera();

    DMKCameraData update(std::vector<POINTER<DMKEventComponent>> eventComponents) override;

private:
    void calculateVectors();
    void calculateRay(CursorPosition position);

    F32 angelX = 0.0f;
    F32 angelY = 0.0f;
    F32 angelZ = 0.0f;

    F32 lastX = 0.0f;
    F32 lastY = 0.0f;
    B1 firstMouse = true;

    F32 Yaw = 90.0f;
    F32 Pitch = 0.0f;
};

#endif // !_APPLICATION_CAMERA_H
