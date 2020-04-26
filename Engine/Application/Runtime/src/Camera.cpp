#include "Camera.h"

/* Update camera data function */

Camera::Camera()
{
}

Camera::~Camera()
{
}

DMKCameraData Camera::update(ARRAY<DMKEventComponent*> eventComponents)
{
	for (auto component : eventComponents)
	{
		switch (component->type)
		{
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_UNKNOWN:
            return myData;
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_KEY_PRESS:
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT:
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_KEY_RELEASE:
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_MOUSE_SCROLL:
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_MOUSE_BUTTON_PRESS:
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_MOUSE_BUTTON_REPEAT:
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_MOUSE_BUTTON_RELEASE:
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_WINDOW_ON_FOCUS:
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_WINDOW_OUTOF_FOCUS:
            break;
        case Dynamik::DMKEventType::DMK_EVENT_TYPE_WINDOW_RESIZE:
            break;
		}
	}

    return myData;
}
