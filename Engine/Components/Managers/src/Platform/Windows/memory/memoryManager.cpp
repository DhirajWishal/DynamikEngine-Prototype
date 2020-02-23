#include "mngafx.h"
#include "memoryManager.h"

namespace Dynamik {
	namespace manager {
		Allocator::Allocator(UI32 size) : memorySize(size) {
			memoryPool = malloc(size);
		}

		Allocator::~Allocator() {
			free(memoryPool);
		}

		doubleBufferedStack::doubleBufferedStack(UI32 size) : Allocator(size) {
			endPointer = (BYTE*)memoryPool;
		}

		doubleBufferedStack::~doubleBufferedStack() {
		}

		poolAllocator::poolAllocator(UI32 size) : Allocator(size) {
			endPointer = (BYTE*)memoryPool;
		}

		VPTR poolAllocator::allocate(UI32 size, UI32 align, UI32 offset) {
			if (memorySize < size)
				return nullptr;

			endPointer += size;
			allocatedSize += size;

			return endPointer;
		}
		
		void poolAllocator::deAllocate(VPTR data, UI32 size, UI32 offset) {
			free(data);
		}
	}
}