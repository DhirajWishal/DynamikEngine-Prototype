#pragma once
#ifndef _DYNAMIK_DATA_TYPES_HEAP_CONTAINER_H
#define _DYNAMIK_DATA_TYPES_HEAP_CONTAINER_H

#include "Array.h"

namespace Dynamik {
	template<class TYPE>
	class HeapStore {
	public:
		HeapStore() {}
		~HeapStore() {}

		void add(const TYPE& data) {
			store.pushBack(data);
		}

	private:
		ARRAY<TYPE> store;
	};
}

#endif // !_DYNAMIK_DATA_TYPES_HEAP_CONTAINER_H
