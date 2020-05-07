#include "studioafx.h"
#include "Camera.h"

/* Update camera data function */

Camera::Camera()
{
}

Camera::~Camera()
{
}

DMKCameraData Camera::update(ARRAY<POINTER<DMKEventComponent>> eventComponents)
{
	for (auto component : eventComponents)
	{
		switch (component->type)
		{
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_UNKNOWN:
            return myData;
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_PRESS:
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_REPEAT:
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_RELEASE:
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_SCROLL:
            break;
		}
	}

    return myData;
}
