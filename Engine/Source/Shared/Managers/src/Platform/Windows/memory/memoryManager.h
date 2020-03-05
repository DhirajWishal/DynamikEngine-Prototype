#pragma once

/*
 Types of allocators:
 * Pool Allocator
 * Stack Allocator
 * Double Buffered Pool Allocator
 * Double Buffered Stack Allocator
*/

#ifndef _DYNAMIK_MANAGERS_WINDOWS_MEMORY_MANAGER_H
#define _DYNAMIK_MANAGERS_WINDOWS_MEMORY_MANAGER_H

#include "Allocator.h"

namespace Dynamik {
	/* MEMORY MANAGER - Manage a set amount of memory */
	class DMK_API MemoryManager {
	public:
		MemoryManager() {}
		MemoryManager(UI32 size) : mySize(size) {}
		~MemoryManager() {}

	private:
		UI32 mySize = 0;
		VPTR myAddress = nullptr;
	};
}

#endif // !_DYNAMIK_MANAGERS_MEMORY_MANAGER_H
