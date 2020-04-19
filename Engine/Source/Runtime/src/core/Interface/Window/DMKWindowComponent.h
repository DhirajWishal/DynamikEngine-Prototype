#pragma once
#ifndef _DYNAMIK_WINDOW_COMPONENT_H
#define _DYNAMIK_WINDOW_COMPONENT_H

#include "DataTypesLib/Public/datatypes.h"

namespace Dynamik {
	class DMKWindowComponent {
	public:
		DMKWindowComponent() {}
		~DMKWindowComponent() {}

		VPTR windowHandle = nullptr;
	};
}

#endif // !_DYNAMIK_WINDOW_COMPONENT_H
