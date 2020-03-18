#pragma once
#ifndef _DYNAMIK_MEMORY_STACK_ALLOCATOR_H
#define _DYNAMIK_MEMORY_STACK_ALLOCATOR_H

#include "Allocator.h"

namespace Dynamik {
	/* Stack ALLOCATOR CLASS */
	class  StackAllocator : public Allocator {
	public:
		StackAllocator() {}
		StackAllocator(UI32 size);
		~StackAllocator() {}

		VPTR allocate(UI32 size, UI32 align, UI32 offset) override;
		void deAllocate(VPTR data, UI32 size, UI32 offset) override;

	private:
		BYTE* endPointer = nullptr;
		UI32 allocatedSize = 0;
	};
}

#endif // !_STACK_ALLOCATOR_H
