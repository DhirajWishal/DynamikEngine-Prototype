#pragma once
#ifndef _DYNAMIK_MANAGERS_ONE_TIME_MEMORY_ALLOCATOR_H
#define _DYNAMIK_MANAGERS_ONE_TIME_MEMORY_ALLOCATOR_H

#include "Allocator.h"

namespace Dynamik {
	/* ONE TIME HEAP ALLOCATOR */
	class DMK_API OneTimeAllocator : public Allocator {
	public:
		OneTimeAllocator() {}
		OneTimeAllocator(UI32 size) : Allocator(size) {}
		~OneTimeAllocator() {}

		VPTR getAddress();
		UI32 getSize();
	};

	template<class TYPE>
	class DMK_API Ref : public OneTimeAllocator {
	public:
		Ref() {}
		Ref(UI32 size) : OneTimeAllocator(size) {}
		~Ref() {}
	};
}

#endif // !_DYNAMIK_MANAGERS_ONE_TIME_MEMORY_ALLOCATOR_H
