#pragma once
#ifndef _DYNAMIK_MOUSE_SCROLL_EVENT_COMPONENT_H
#define _DYNAMIK_MOUSE_SCROLL_EVENT_COMPONENT_H

#include "EventComponent.h"

namespace Dynamik {
	class DMKMouseScrollEventComponent : public DMKEventComponent {
	public:
		DMKMouseScrollEventComponent() : DMKEventComponent(DMKEventCategory::DMK_EVENT_CATEGORY_MOUSE_SCROLL) {}
		~DMKMouseScrollEventComponent() {}

		D64 xOffset = 0.0f;
		D64 yOffset = 0.0f;
	};
}

#endif // !_DYNAMIK_MOUSE_SCROLL_EVENT_COMPONENT_H
