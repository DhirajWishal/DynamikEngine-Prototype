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
			Allocator(UI32 size);
			virtual ~Allocator();

			virtual VPTR allocate(UI32 size, UI32 align, UI32 offset) { return nullptr; }
			virtual void deAllocate(VPTR data, UI32 size, UI32 offset) {}

			virtual void pack() {}

		protected:
			template<typename T>
			VPTR _incrementBy(VPTR address, UI32 size) {
				return (T*)address + size;
			}

			VPTR memoryPool = nullptr;
			UI32 memorySize = 0;
		};

		/* MAIN DOUBLE BUFFERED ALLOCATOR CLASS */
		class DoubleBufferedAllocator : public Allocator {
			DoubleBufferedAllocator() {}
			DoubleBufferedAllocator(UI32 size) : Allocator(size) {
				lowEndPointer = (BYTE*)memoryPool;
				highEndPointer = lowEndPointer + size;
			}
			virtual ~DoubleBufferedAllocator() {}

			virtual VPTR allocateHigh(UI32 size, UI32 align, UI32 offset) { return nullptr; }
			virtual VPTR allocateLow(UI32 size, UI32 align, UI32 offset) { return nullptr; }

			virtual void packHigh() {}
			virtual void packLow() {}

		protected:
			BYTE* highEndPointer = nullptr;
			UI32 highEndAllocatedSize = 0;

			BYTE* lowEndPointer = nullptr;
			UI32 lowEndAllocatedSize = 0;
		};

		/* POOL ALLOCATOR CLASS */
		class poolAllocator : public Allocator {
		public:
			poolAllocator() {}
			poolAllocator(UI32 size);
			~poolAllocator() {}

			VPTR allocate(UI32 size, UI32 align, UI32 offset) override;
			void deAllocate(VPTR data, UI32 size, UI32 offset) override;

		private:
			BYTE* endPointer = nullptr;
			UI32 allocatedSize = 0;
		};

		/* DOUBLE BUFFERED STACK ALLOCATOR CLASS */
		class doubleBufferedStack : public Allocator{
		public:
			doubleBufferedStack() {}
			doubleBufferedStack(UI32 size);
			~doubleBufferedStack();

			template<typename T>
			T* allocate(UI32 size, UI32 align, UI32 offset) {
				if (size > myMemorySize)
					return nullptr;

				endPointer += size;
				allocatedSize += size;

				return (T*)endPointer;
			}

			void deAllocate() {}

		private:
			BYTE* endPointer = nullptr;
			UI32 allocatedSize = 0;
		};

		/* MEMORY MANAGER - Manage a set amount of memory */
		class MemoryManager {
		public:
			MemoryManager() {}
			MemoryManager(UI32 size) : mySize(size) {}
			~MemoryManager() {}

		private:
			UI32 mySize = 0;
			VPTR myAddress = nullptr;
		};
	}
}

#endif // !_DYNAMIK_MANAGERS_MEMORY_MANAGER_H
