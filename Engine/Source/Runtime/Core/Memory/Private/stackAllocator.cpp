#include "dmkafx.h"
#include "Public/stackAllocator.h"

namespace Dynamik {
	StackAllocator::StackAllocator(UI32 size) : Allocator(size)
	{
	}

	VPTR StackAllocator::allocate(UI32 size, UI32 align, UI32 offset)
	{
		if (memorySize < size)
			return nullptr;

		endPointer += size;
		allocatedSize += size;

		return endPointer;
	}

	void StackAllocator::deAllocate(VPTR data, UI32 size, UI32 offset)
	{
	}
}