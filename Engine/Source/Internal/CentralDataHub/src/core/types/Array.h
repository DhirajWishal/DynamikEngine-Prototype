#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H
#define _DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H

#include "core/global/datatypes.h"

namespace Dynamik {
	template<class TYPE>
	class Array {
	public:
		Array() {}
		Array(UI32 size) : mySize(size) {}

	private:
		TYPE* myData;
		UI32 mySize = 0;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H
