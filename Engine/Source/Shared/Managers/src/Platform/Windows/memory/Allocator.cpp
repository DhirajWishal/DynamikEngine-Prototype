#include "mngafx.h"
#include "Allocator.h"

namespace Dynamik {
	Allocator::Allocator(UI32 size) : memorySize(size) {
		memoryPool = malloc(size);
	}

	Allocator::~Allocator() {
		free(memoryPool);
	}
}