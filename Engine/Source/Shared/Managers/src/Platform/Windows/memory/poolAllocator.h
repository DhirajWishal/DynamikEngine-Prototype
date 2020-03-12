#pragma once
#ifndef _DYNAMIK_MANAGERS_POOL_ALLOCATOR_H
#define _DYNAMIK_MANAGERS_POOL_ALLOCATOR_H

#include "Allocator.h"

namespace Dynamik {
	/* POOL ALLOCATOR CLASS */
	class  PoolAllocator : public Allocator {
	public:
		PoolAllocator() {}
		PoolAllocator(UI32 size);
		~PoolAllocator() {}

		VPTR allocate(UI32 size, UI32 align, UI32 offset) override;
		void deAllocate(VPTR data, UI32 size, UI32 offset) override;

	private:
		BYTE* endPointer = nullptr;
		UI32 allocatedSize = 0;
	};
}

#endif // !_DYNAMIK_MANAGERS_POOL_ALLOCATOR_H
