#include "mngafx.h"
#include "oneTimeAllocator.h"

namespace Dynamik {
	VPTR OneTimeAllocator::getAddress() {
		return memoryPool;
	}

	UI32 OneTimeAllocator::getSize() {
		return memorySize;
	}
}