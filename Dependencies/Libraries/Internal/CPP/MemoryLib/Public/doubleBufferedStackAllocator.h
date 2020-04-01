#pragma once
#ifndef _DYNAMIK_MEMORY_DOUBLE_BUFFERED_STACK_ALLOCATOR_H
#define _DYNAMIK_MEMORY_DOUBLE_BUFFERED_STACK_ALLOCATOR_H

#include "doubleBufferedAllocator.h"
#include "DataTypesLib/Public/Pointer.h"

namespace Dynamik {
	struct DBSIndexBlock {
		POINTER<UI32> address;
		UI32 size = 0;
		UI32 offset = 0;
		UI32 index = 0;
		POINTER<UI32> indexedAddress;
		B1 buffer = false;
	};

	/* DOUBLE BUFFERED STACK ALLOCATOR CLASS */
	class  DoubleBufferedStackAllocator : public DoubleBufferedAllocator {
	public:
		DoubleBufferedStackAllocator() {}
		DoubleBufferedStackAllocator(UI32 size);
		~DoubleBufferedStackAllocator();

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

#endif // !_DOUBLE_BUFFERED_STACK_ALLOCATOR_H
