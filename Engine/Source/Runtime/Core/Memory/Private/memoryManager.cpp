#include "dmkafx.h"
#include "Public/MemoryManager.h"

namespace Dynamik {
	MemoryManager MemoryManager::instance;
	
	void MemoryManager::allocate(UI32 size)
	{
		if (instance.isAllocated)
		{
			DMK_CORE_WARN("Reallocating the pool while it is already allocated!");
			StaticAllocator<VPTR>::deAllocate(instance.myMemoryPool, instance.myMemoryPoolSize);
		}

		instance.myMemoryPool = StaticAllocator<VPTR>::allocate(size);
		instance.myMemoryPoolSize = size;
	}

	void MemoryManager::extend(UI32 addToSize)
	{
		UI32 _newSize = instance.myMemoryPoolSize + addToSize;
		VPTR _tmpPool = StaticAllocator<VPTR>::allocate(_newSize);
		UI32 _gapSize = (_newSize - instance.lowIndexSize) - instance.highIndexSize;

		memcpy(_tmpPool, instance.lowIndexBufferPointer, instance.lowIndexSize);
		memcpy((VPTR)((((UI64)_tmpPool)) + _newSize), instance.highIndexBeginPointer, instance.highIndexSize);

		deallocate();
		instance.myMemoryPool = _tmpPool;
		instance.myMemoryPoolSize = _newSize;
	}
	
	void MemoryManager::deallocate()
	{
		DMK_CORE_WARN("Deallocating Memory Manager!");
		StaticAllocator<VPTR>::deAllocate(instance.myMemoryPool, instance.myMemoryPoolSize);

		instance.myMemoryPool = nullptr;
		instance.myMemoryPoolSize = 0;

		instance.lowIndexBufferPointer = nullptr;
		instance.lowIndexSize = 0;

		instance.highIndexBufferPointer = nullptr;
		instance.highIndexBeginPointer = nullptr;
		instance.highIndexSize = 0;
	}
}