#include "mngafx.h"
#include "doubleBufferedAllocator.h"

namespace Dynamik {
	DoubleBufferedAllocator::DoubleBufferedAllocator(UI32 size) : Allocator(size) {
		lowEndPointer = (BYTE*)memoryPool;
		highEndPointer = lowEndPointer + size;
	}
}