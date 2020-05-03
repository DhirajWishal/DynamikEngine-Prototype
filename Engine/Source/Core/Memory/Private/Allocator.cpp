#include "dmkafx.h"
#include "Public/Allocator.h"
#include <memory>

namespace Dynamik {
	Allocator::Allocator(UI32 size) : memorySize(size)
	{
		memoryPool = Allocator::basicAlloc(size);
	}

	Allocator::~Allocator()
	{
		Allocator::basicDeAlloc(memoryPool);
	}

	VPTR Allocator::basicAlloc(UI32 size, UI32 offset)
	{
		return malloc(size);
	}

	VPTR Allocator::basicAllignedAlloc(UI32 size, UI32 align)
	{
		//return operator new (size, std::align_val_t{ (size_t)align });
		return nullptr;
	}

	void Allocator::basicDeAlloc(VPTR data, UI32 size, UI32 offset)
	{
		free(data);
	}

	void Allocator::basicAllignedDeAlloc(VPTR data, UI32 size, UI32 align)
	{
		//operator delete[](data, size, std::align_val_t{ (size_t)align });
		basicDeAlloc(data);
	}

	void Allocator::clearRange(VPTR begin, VPTR end)
	{
		while (begin != end) {
		}
	}
}