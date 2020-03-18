#include "MemoryLib/Public/poolAllocator.h"
#include <memory>

namespace Dynamik {
	PoolAllocator::PoolAllocator(UI32 size) : Allocator(size) {
		endPointer = (BYTE*)memoryPool;
	}

	VPTR PoolAllocator::allocate(UI32 size, UI32 align, UI32 offset) {
		if (memorySize < size)
			return nullptr;

		endPointer += size;
		allocatedSize += size;

		return endPointer;
	}

	void PoolAllocator::deAllocate(VPTR data, UI32 size, UI32 offset) {
		free(data);
	}
}