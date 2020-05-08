#pragma once
#ifndef _DYNAMIK_EVENT_COMPONENT_H
#define _DYNAMIK_EVENT_COMPONENT_H

#include <GLFW/glfw3.h>
#include "Public/Pointer.h"


namespace Dynamik {
	enum class DMKEventCategory {
		DMK_EVENT_CATEGORY_UNKNOWN,

		DMK_EVENT_CATEGORY_KEY,
		DMK_EVENT_CATEGORY_MOUSE_BUTTON,
		DMK_EVENT_CATEGORY_MOUSE_SCROLL,
		DMK_EVENT_CATEGORY_APPLICATION,
		DMK_EVENT_CATEGORY_JOYSTICK,
		DMK_EVENT_CATEGORY_TEXT,
	};

	enum class DMKEventType {
		DMK_EVENT_TYPE_UNKNOWN,

		DMK_EVENT_TYPE_PRESS,
		DMK_EVENT_TYPE_REPEAT,
		DMK_EVENT_TYPE_RELEASE,

		DMK_EVENT_TYPE_SCROLL,
	};

	class DMKEventComponent {
	public:
		DMKEventComponent() {}
		DMKEventComponent(DMKEventCategory cat) : category(cat) {}
		virtual ~DMKEventComponent() {}

		DMKEventCategory category = DMKEventCategory::DMK_EVENT_CATEGORY_KEY;
		DMKEventType type = DMKEventType::DMK_EVENT_TYPE_UNKNOWN;

		POINTER<GLFWwindow> window;

		B1 isHandled = false;
	};
}

#endif // !_DYNAMIK_EVENT_COMPONENT_H
