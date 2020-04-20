#pragma once
#ifndef _DYNAMIK_DATA_TYPES_ITERATOR_H
#define _DYNAMIK_DATA_TYPES_ITERATOR_H

#include "Pointer.h"

namespace Dynamik {
	/* WRAPPER CLASS
	 * Wrapper class for POINTER to use as an iterator.
	 */
	template<class TYPE>
	class ITERATOR : public POINTER<TYPE> {
	public:
		ITERATOR() {}
		ITERATOR(TYPE* address) : POINTER<TYPE>(address) {}
		~ITERATOR() {}
	};
}

#endif // !_DATA_TYPES_ITERATOR_H
