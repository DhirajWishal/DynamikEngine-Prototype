#include "MemoryLib/Public/doubleBufferedStackAllocator.h"

namespace Dynamik {
	DoubleBufferedStackAllocator::DoubleBufferedStackAllocator(UI32 size) : DoubleBufferedAllocator(size) {
	}

	DoubleBufferedStackAllocator::~DoubleBufferedStackAllocator()
	{
	}

	void DoubleBufferedStackAllocator::deAllocate()
	{
	}
}