#pragma once
#ifndef _DYNAMIK_KEY_EVENT_COMPONENT_H
#define _DYNAMIK_KEY_EVENT_COMPONENT_H

#include "DMKEventComponent.h"

namespace Dynamik {
	class DMKKeyEventComponent : public DMKEventComponent {
	public:
		DMKKeyEventComponent() : DMKEventComponent(DMKEventCategory::DMK_EVENT_CATEGORY_KEY) {}
		~DMKKeyEventComponent() {}

		I32 keycode = DMK_KEY_UNKNOWN;
		I32 scancode = DMK_KEY_UNKNOWN;
		I32 mods = 0;
	};
}

#endif // !_DYNAMIK_KEY_EVENT_COMPONENT_H
