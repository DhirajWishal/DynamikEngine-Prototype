#pragma once
#ifndef _DYNAMIK_MEMORY_DOUBLE_BUFFERED_POOL_ALLOCATOR_H
#define _DYNAMIK_MEMORY_DOUBLE_BUFFERED_POOL_ALLOCATOR_H

#include "doubleBufferedAllocator.h"

namespace Dynamik {
	/* DOUBLE BUFFERED POOL ALLOCATOR CLASS */
	class   DoubleBufferedPoolAllocator : public DoubleBufferedAllocator {
	public:
		DoubleBufferedPoolAllocator() {}
		DoubleBufferedPoolAllocator(UI32 size);
		~DoubleBufferedPoolAllocator();

		template<typename T>
		T* allocate(UI32 size, UI32 align, UI32 offset) {
			if (size > myMemorySize)
				return nullptr;

			endPointer += size;
			allocatedSize += size;

			return (T*)endPointer;
		}

		void deAllocate();

	private:
		BYTE* endPointer = nullptr;
		UI32 allocatedSize = 0;
	};
}

#endif // !_DOUBLE_BUFFERED_POOL_ALLOCATOR_H
