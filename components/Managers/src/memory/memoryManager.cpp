#include "mngafx.h"
#include "memoryManager.h"

namespace Dynamik {
	namespace manager {

		allocator::allocator(uint32 size) : totalSize(size) {
			stackPool = malloc(size);
		}

		allocator::~allocator() {
			free(stackPool);
		}

		void* allocator::allocate(uint32 size, uint32 align, uint32 offset) {

		}
	}
}
