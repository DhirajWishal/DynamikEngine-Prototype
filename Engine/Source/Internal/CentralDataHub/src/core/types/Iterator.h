#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_ITERATOR_H
#define _DYNAMIK_CENTRAL_DATA_HUB_ITERATOR_H

#include "Pointer.h"

namespace Dynamik {
	template<class TYPE>
	class ITERATOR {
	public:
		ITERATOR() {}
		ITERATOR(TYPE* address) : data(address) {}
		~ITERATOR() {}

		ITERATOR next() { return ++ITERATOR.data; }

		POINTER<TYPE> data = nullptr;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_ITERATOR_H
