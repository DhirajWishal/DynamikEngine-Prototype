#pragma once
#ifndef _DYNAMIK_WINDOW_RESIZE_EVENT_COMPONENT_H
#define _DYNAMIK_WINDOW_RESIZE_EVENT_COMPONENT_H

#include "DMKEventComponent.h"

namespace Dynamik {
	class DMKWindowResizeEventComponent : public DMKEventComponent {
	public:
		DMKWindowResizeEventComponent() : DMKEventComponent(DMKEventCategory::DMK_EVENT_CATEGORY_APPLICATION) {}
		~DMKWindowResizeEventComponent() {}

		I32 width = 0;
		I32 height = 0;
	};
}

#endif // !_DYNAMIK_WINDOW_RESIZE_EVENT_COMPONENT_H
