#include "mngafx.h"
#include "doubleBufferedStackAllocator.h"

namespace Dynamik {
	doubleBufferedStackAllocator::doubleBufferedStackAllocator(UI32 size) : DoubleBufferedAllocator(size) {

	}
	
	doubleBufferedStackAllocator::~doubleBufferedStackAllocator()
	{
	}
	
	void doubleBufferedStackAllocator::deAllocate()
	{
	}
}