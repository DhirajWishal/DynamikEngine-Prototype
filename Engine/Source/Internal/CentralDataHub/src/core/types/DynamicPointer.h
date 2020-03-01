#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_DYNAMIC_POINTER_H
#define _DYNAMIK_CENTRAL_DATA_HUB_DYNAMIC_POINTER_H

#include "core/global/datatypes.h"

namespace Dynamik {
	class DynamicPointer {
	public:
		DynamicPointer() {}
		DynamicPointer(VPTR ptr);
		DynamicPointer(UI32 size);
		~DynamicPointer();

	private:
		VPTR myPointer = nullptr;
		UI32 myAddressSize = 0;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_DYNAMIC_POINTER_H
