#pragma once
#ifndef _DYNAMIK_MANAGERS_STACK_ALLOCATOR_H
#define _DYNAMIK_MANAGERS_STACK_ALLOCATOR_H

#include "Allocator.h"

namespace Dynamik {
	/* Stack ALLOCATOR CLASS */
	class DMK_API stackAllocator : public Allocator {
	public:
		stackAllocator() {}
		stackAllocator(UI32 size);
		~stackAllocator() {}

		VPTR allocate(UI32 size, UI32 align, UI32 offset) override;
		void deAllocate(VPTR data, UI32 size, UI32 offset) override;

	private:
		BYTE* endPointer = nullptr;
		UI32 allocatedSize = 0;
	};
}

#endif // !_DYNAMIK_MANAGERS_STACK_ALLOCATOR_H
