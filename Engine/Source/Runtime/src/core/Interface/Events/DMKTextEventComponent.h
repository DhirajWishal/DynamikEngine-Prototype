#pragma once
#ifndef _DYNAMIK_TEXT_EVENT_COMPONENT_H
#define _DYNAMIK_TEXT_EVENT_COMPONENT_H

#include "DMKEventComponent.h"

namespace Dynamik {
	class DMK_API DMKTextEventComponent : public DMKEventComponent {
	public:
		DMKTextEventComponent() : DMKEventComponent(DMKEventCategory::DMK_EVENT_CATEGORY_TEXT) {}
		~DMKTextEventComponent() {}

		UI32 charCode = 0;
	};
}

#endif // !_DYNAMIK_TEXT_EVENT_COMPONENT_H
