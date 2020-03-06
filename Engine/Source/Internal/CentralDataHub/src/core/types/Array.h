#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H
#define _DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H

#include "core/global/datatypes.h"
#include "Platform/Windows/memory/oneTimeAllocator.h"

namespace Dynamik {
	template<class TYPE>
	class Array {
	public:
		Array() {}
		Array(UI32 size) : mySize(size) {}

		void pushBack(TYPE& data) {
			TYPE* _buffer = (TYPE*)myAllocator.allocate(mySize + 1);
		}

	private:
		TYPE* myData;
		UI32 mySize = 0;

		OneTimeAllocator myAllocator = {};
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H
