#pragma once

/*
 Types of allocators:
 * Pool Allocator
 * Stack Allocator
 * Double Buffered Pool Allocator
 * Double Buffered Stack Allocator
*/

#ifndef _DYNAMIK_MANAGERS_WINDOWS_MEMORY_MANAGER_H
#define _DYNAMIK_MANAGERS_WINDOWS_MEMORY_MANAGER_H

#include "CentralDataHub.h"

namespace Dynamik {
	namespace manager {
		/* MAIN ALLOCATOR CLASS */
		class Allocator {
		public:
			Allocator() {}
			Allocator(uint32 size);
			virtual ~Allocator();

			virtual void* allocate(uint32_t size, uint32_t align, uint32_t offset) { return nullptr; }
			virtual void deAllocate(uint32_t size, void* data, uint32_t offset) {}

			virtual void pack() {}

		protected:
			void* memoryPool = nullptr;
			uint32_t memorySize = 0;
		};

		/* MAIN DOUBLE BUFFERED ALLOCATOR CLASS */
		class DoubleBufferedAllocator : public Allocator {
			DoubleBufferedAllocator() {}
			DoubleBufferedAllocator(uint32_t size) : Allocator(size) {
				lowEndPointer = (uint8_t*)memoryPool;
				highEndPointer = lowEndPointer + size;
			}
			virtual ~DoubleBufferedAllocator() {}

			virtual void* allocateHigh(uint32_t size, uint32_t align, uint32_t offset) { return nullptr; }
			virtual void* allocateLow(uint32_t size, uint32_t align, uint32_t offset) { return nullptr; }

			virtual void packHigh() {}
			virtual void packLow() {}

		protected:
			uint8_t* highEndPointer = nullptr;
			uint32_t highEndAllocatedSize = 0;

			uint8_t* lowEndPointer = nullptr;
			uint32_t lowEndAllocatedSize = 0;
		};

		/* POOL ALLOCATOR CLASS */
		class poolAllocator : public Allocator {
		public:
			poolAllocator() {}
			poolAllocator(uint32_t size);
			~poolAllocator() {}

			void* allocate(uint32_t size, uint32_t align, uint32_t offset) override;

		private:
			uint8_t* endPointer = nullptr;
			uint32_t allocatedSize = 0;
		};

		/* DOUBLE BUFFERED STACK ALLOCATOR CLASS */
		class doubleBufferedStack : public Allocator{
		public:
			doubleBufferedStack() {}
			doubleBufferedStack(uint32_t size);
			~doubleBufferedStack();

			template<typename T>
			T* allocate(uint32_t size, uint32_t align, uint32_t offset) {
				if (size > myMemorySize)
					return nullptr;

				endPointer += size;
				allocatedSize += size;

				return (T*)endPointer;
			}

			void deAllocate();

		private:
			uint8_t* endPointer = nullptr;
			uint32_t allocatedSize = 0;
		};
	}
}

#endif // !_DYNAMIK_MANAGERS_MEMORY_MANAGER_H
