#pragma once
#ifndef _DYNAMIK_MEMORY_DOUBLE_BUFFERED_ALLOCATOR_H
#define _DYNAMIK_MEMORY_DOUBLE_BUFFERED_ALLOCATOR_H

#include "Allocator.h"

namespace Dynamik {
	/* MAIN DOUBLE BUFFERED ALLOCATOR CLASS */
	class DoubleBufferedAllocator : public Allocator {
	public:
		DoubleBufferedAllocator() {}
		DoubleBufferedAllocator(UI32 size);
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
}

#endif // !_DOUBLE_BUFFERED_ALLOCATOR_H
