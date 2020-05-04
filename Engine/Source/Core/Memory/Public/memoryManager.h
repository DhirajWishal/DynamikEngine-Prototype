#pragma once
#ifndef _DYNAMIK_MEMORY_MANAGER_H
#define _DYNAMIK_MEMORY_MANAGER_H

#include "Public/Pointer.h"

namespace Dynamik {
	/* Dynamik Memory Manager
	 * This uses a Double buffered Stack allocator.
	 */
	class  MemoryManager {
		MemoryManager() {}
		~MemoryManager() {}

		static MemoryManager instance;
	public:
		static void allocate(UI32 size);
		static void extend(UI32 addToSize);
		static void deallocate();

		template<class TYPE>
		static POINTER<TYPE> storeOnLow(const TYPE& data)
		{
			StaticAllocator<TYPE>::set((POINTER<TYPE>)(TYPE*)(((UI64)instance.lowIndexBufferPointer) + instance.lowIndexSize), (TYPE&&)data);
			instance.lowIndexSize += sizeof(data);
		}

	private:
		B1 isAllocated = false;

		/* Main memory pool */
		VPTR myMemoryPool = nullptr;
		UI32 myMemoryPoolSize = 0;

		/* Double buffer pointers */
		VPTR lowIndexBufferPointer = nullptr;
		UI32 lowIndexSize = 0;

		VPTR highIndexBufferPointer = nullptr;
		VPTR highIndexBeginPointer = nullptr;
		UI32 highIndexSize = 0;
	};
}

#endif // !_DYNAMIK_MEMORY_MANAGER_H
