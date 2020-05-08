#pragma once
#ifndef _DYNAMIK_MOUSE_BUTTON_EVENT_COMPONENT_H
#define _DYNAMIK_MOUSE_BUTTON_EVENT_COMPONENT_H

#include "EventComponent.h"

namespace Dynamik {
	class DMKMouseButtonEventComponent : public DMKEventComponent {
	public:
		DMKMouseButtonEventComponent() : DMKEventComponent(DMKEventCategory::DMK_EVENT_CATEGORY_MOUSE_BUTTON) {}
		~DMKMouseButtonEventComponent() {}

		I32 button = 0;
		I32 mods = 0;
	};
}

#endif // !_DYNAMIK_MOUSE_BUTTON_EVENT_COMPONENT_H
