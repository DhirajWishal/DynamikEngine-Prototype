#include "mngafx.h"
#include "oneTimeAllocator.h"

namespace Dynamik {
	VPTR OneTimeAllocator::allocate(UI32 size, UI32 align, UI32 offset)
	{
		memoryPool = malloc(size);
		return memoryPool;
	}
	
	void OneTimeAllocator::deAllocate(VPTR data, UI32 size, UI32 offset)
	{
		free(data);
	}

	void OneTimeAllocator::extend(UI32 size, UI32 offset)
	{
		UI32 _sizeToAllocate = size + memorySize;
		VPTR _buffer = malloc(_sizeToAllocate);
		memcpy(_buffer, memoryPool, memorySize);
		allocate(_sizeToAllocate);
		memcpy(memoryPool, _buffer, _sizeToAllocate);
		delete _buffer;
	}
	
	VPTR OneTimeAllocator::getAddress() {
		return memoryPool;
	}

	UI32 OneTimeAllocator::getSize() {
		return memorySize;
	}
}