#pragma once
#ifndef _DYNAMIK_DROP_EVENT_COMPONENT_H
#define _DYNAMIK_DROP_EVENT_COMPONENT_H

#include "EventComponent.h"
#include <string>
#include <vector>

namespace Dynamik {
	class DMKDropPathEventComponent : public DMKEventComponent {
	public:
		DMKDropPathEventComponent() : DMKEventComponent(DMKEventCategory::DMK_EVENT_CATEGORY_APPLICATION) {}
		~DMKDropPathEventComponent() {}

		I32 count = 0;
		std::vector<std::string> paths;
	};
}

#endif // !_DYNAMIK_DROP_EVENT_COMPONENT_H
