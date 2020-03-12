#include "mngafx.h"
#include "Allocator.h"

namespace Dynamik {
	Allocator::Allocator(UI32 size) : memorySize(size)
	{
		memoryPool = Allocator::basicAlloc(size);
	}

	Allocator::~Allocator()
	{
		Allocator::basicDeAlloc(memoryPool);
	}

	VPTR Allocator::basicAlloc(UI32 size, UI32 align, UI32 offset)
	{
		return new VPTR[size];
	}

	void Allocator::basicDeAlloc(VPTR data, UI32 size, UI32 offset)
	{
		delete[] data;
	}
}