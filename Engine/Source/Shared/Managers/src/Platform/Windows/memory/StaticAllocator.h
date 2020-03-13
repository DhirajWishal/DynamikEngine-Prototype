#pragma once
#ifndef _DYNAMIK_MANAGERS_STATIC_ALLOCATOR_H
#define _DYNAMIK_MANAGERS_STATIC_ALLOCATOR_H

#include "core/global/constants.h"
#include "core/global/datatypes.h"
#ifdef DMK_MEMORY_USE_NEW
#include <memory>

#endif

namespace Dynamik {
	template<class TYPE>
	class StaticAllocator {
	public:
		StaticAllocator() {}
		~StaticAllocator() {}

		static TYPE* allocate(UI32 size = 0, UI32 align = DMK_MEMORY_ALIGN, UI32 offset = 0)
		{
#if defined(DMK_MEMORY_USE_MALLOC)
			return (TYPE*)malloc(size);

#elif defined(DMK_MEMORY_USE_NEW)
			return operator new[](size, std::align_val_t{ (size_t)align });
#endif
		}

		static void deAllocate(VPTR data, UI32 size = 0, UI32 align = DMK_MEMORY_ALIGN, UI32 offset = 0)
		{
#if defined(DMK_MEMORY_USE_MALLOC)
			free(data);

#elif defined(DMK_MEMORY_USE_NEW)
			delete[] data;

#endif
		}
	};
}

#endif // !_DYNAMIK_MANAGERS_STATIC_ALLOCATOR_H
