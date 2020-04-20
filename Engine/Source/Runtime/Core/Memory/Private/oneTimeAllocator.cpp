#include "MemoryLib/Public/oneTimeAllocator.h"
#include <memory>

namespace Dynamik {
	VPTR OneTimeAllocator::allocate(UI32 size, UI32 align, UI32 offset)
	{
		//if (memoryPool != nullptr)
		//	Allocator::basicDeAlloc(memoryPool);

		memoryPool = Allocator::basicAlloc(size);
		memorySize = size;
		return memoryPool;
	}

	void OneTimeAllocator::deAllocate(VPTR data, UI32 size, UI32 offset)
	{
		Allocator::basicDeAlloc(data);
	}

	// size = ammount to be extended
	void OneTimeAllocator::extend(UI32 size, UI32 offset)
	{
		UI32 _sizeToAllocate = size + memorySize;
		VPTR _buffer = nullptr;
		try {
			_buffer = Allocator::basicAlloc(_sizeToAllocate);

			memmove(_buffer, memoryPool, memorySize);
			allocate(_sizeToAllocate);
			memmove(memoryPool, _buffer, _sizeToAllocate);
			Allocator::basicDeAlloc(_buffer, _sizeToAllocate);
		}
		catch (...) {
			throw;
		}
		memorySize += size;
	}

	VPTR OneTimeAllocator::getAddress() {
		return memoryPool;
	}

	UI32 OneTimeAllocator::getSize() {
		return memorySize;
	}
}