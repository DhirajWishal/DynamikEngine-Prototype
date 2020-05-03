#include "dmkafx.h"
#include "Public/doubleBufferedPoolAllocator.h"

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