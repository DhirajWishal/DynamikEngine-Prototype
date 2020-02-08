#include "mngafx.h"
#include "memoryManager.h"

namespace Dynamik {
	namespace manager {
		Allocator::Allocator(uint32 size) : memorySize(size) {
			memoryPool = malloc(size);
		}

		Allocator::~Allocator() {
			free(memoryPool);
		}

		doubleBufferedStack::doubleBufferedStack(uint32_t size) : Allocator(size) {
			endPointer = (uint8_t*)memoryPool;
		}

		doubleBufferedStack::~doubleBufferedStack() {
		}

		poolAllocator::poolAllocator(uint32_t size) : Allocator(size) {
			endPointer = (uint8_t*)memoryPool;
		}

		void* poolAllocator::allocate(uint32_t size, uint32_t align, uint32_t offset) {
			if (memorySize < size)
				return nullptr;

			endPointer += size;
			allocatedSize += size;

			return endPointer;
		}
	}
}