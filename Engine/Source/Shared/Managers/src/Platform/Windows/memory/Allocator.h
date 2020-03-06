#pragma once
#ifndef _DYNAMIK_MANAGERS_MEMORY_ALLOCATOR_H
#define _DYNAMIK_MANAGERS_MEMORY_ALLOCATOR_H

#include "CentralDataHub.h"
#include "Platform.h"

namespace Dynamik {
	/* MAIN ALLOCATOR CLASS */
	class DMK_API Allocator {
	public:
		Allocator() {}
		Allocator(UI32 size);
		virtual ~Allocator();

		virtual VPTR allocate(UI32 size = 0, UI32 align = 0, UI32 offset = 0) { return nullptr; }
		virtual void deAllocate(VPTR data = nullptr, UI32 size = 0, UI32 offset = 0) {}

		virtual void pack() {}

	protected:
		template<typename T>
		VPTR _incrementBy(VPTR address, UI32 size) {
			return (T*)address + size;
		}

		VPTR memoryPool = nullptr;
		UI32 memorySize = 0;
	};
}

#endif // !_DYNAMIK_MANAGERS_MEMORY_ALLOCATOR_H
