#include "mngafx.h"
#include "doubleBufferedPoolAllocator.h"

namespace Dynamik {
	doubleBufferedPoolAllocator::doubleBufferedPoolAllocator(UI32 size) : DoubleBufferedAllocator(size)
	{
	}
	
	doubleBufferedPoolAllocator::~doubleBufferedPoolAllocator()
	{
	}
	
	void doubleBufferedPoolAllocator::deAllocate()
	{
	}
}