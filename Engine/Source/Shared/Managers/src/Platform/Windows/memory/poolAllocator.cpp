#include "mngafx.h"
#include "poolAllocator.h"

namespace Dynamik {
	poolAllocator::poolAllocator(UI32 size) : Allocator(size) {
		endPointer = (BYTE*)memoryPool;
	}

	VPTR poolAllocator::allocate(UI32 size, UI32 align, UI32 offset) {
		if (memorySize < size)
			return nullptr;

		endPointer += size;
		allocatedSize += size;

		return endPointer;
	}

	void poolAllocator::deAllocate(VPTR data, UI32 size, UI32 offset) {
		free(data);
	}
}