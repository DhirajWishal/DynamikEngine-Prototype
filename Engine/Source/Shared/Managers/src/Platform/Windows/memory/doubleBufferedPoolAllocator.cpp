#include "mngafx.h"
#include "doubleBufferedPoolAllocator.h"

namespace Dynamik {
	DoubleBufferedPoolAllocator::DoubleBufferedPoolAllocator(UI32 size) : DoubleBufferedAllocator(size)
	{
	}

	DoubleBufferedPoolAllocator::~DoubleBufferedPoolAllocator()
	{
	}

	void DoubleBufferedPoolAllocator::deAllocate()
	{
	}
}