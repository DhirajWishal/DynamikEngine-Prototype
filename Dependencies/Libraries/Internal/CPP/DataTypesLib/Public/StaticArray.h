#pragma once
#ifndef _DYNAMIK_DATA_TYPES_STATIC_ARRAY_H
#define _DYNAMIK_DATA_TYPES_STATIC_ARRAY_H

#include "Array.h"

namespace Dynamik {
	/* WRAPPER CLASS
	 * Wrapper class for the ARRAY data structure.
	 * This must have a preddefined size and will not be extended.
	 */
	template<class TYPE, class Allocator = StaticAllocator<TYPE>>
	class STATIC_ARRAY : public ARRAY<TYPE, Allocator> {
	public:
		STATIC_ARRAY(UI32 size) : ARRAY<TYPE, Allocator>(size) {}
		STATIC_ARRAY(UI32 size, const TYPE& value) : ARRAY<TYPE, Allocator>(size, value) {}
		~STATIC_ARRAY() {}

	private:
	};
}

#endif // !_DATA_TYPES_STATIC_ARRAY_H
