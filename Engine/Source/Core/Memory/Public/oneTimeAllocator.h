#pragma once
#ifndef _DYNAMIK_MEMORY_ONE_TIME_DYNAMIK_MEMORY_ALLOCATOR_H
#define _DYNAMIK_MEMORY_ONE_TIME_DYNAMIK_MEMORY_ALLOCATOR_H

#include "Allocator.h"

namespace Dynamik {
	class Allocator;

	/* ONE TIME HEAP ALLOCATOR */
	class OneTimeAllocator : public Allocator {
	public:
		OneTimeAllocator() {}
		OneTimeAllocator(UI32 size) : Allocator(size) {}
		~OneTimeAllocator() {}

		VPTR allocate(UI32 size = 0, UI32 align = 0, UI32 offset = 0) override;
		void deAllocate(VPTR data = nullptr, UI32 size = 0, UI32 offset = 0) override;
		void extend(UI32 size = 0, UI32 offset = 0) override;

		VPTR getAddress();
		UI32 getSize();
	};

	template<class TYPE>
	class Ref : public OneTimeAllocator {
	public:
		Ref() {}
		Ref(UI32 size) : OneTimeAllocator(size) {}
		~Ref() {}
	};
}

#endif // !_ONE_TIME_DYNAMIK_MEMORY_ALLOCATOR_H
