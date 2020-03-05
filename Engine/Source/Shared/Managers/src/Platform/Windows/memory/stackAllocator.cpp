#include "mngafx.h"
#include "stackAllocator.h"

namespace Dynamik {
	stackAllocator::stackAllocator(UI32 size) : Allocator(size)
	{
	}
	
	VPTR stackAllocator::allocate(UI32 size, UI32 align, UI32 offset)
	{
		if (memorySize < size)
			return nullptr;

		endPointer += size;
		allocatedSize += size;

		return endPointer;
	}
	
	void stackAllocator::deAllocate(VPTR data, UI32 size, UI32 offset)
	{
	}
}