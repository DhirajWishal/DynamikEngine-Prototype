#pragma once
#ifndef _DYNAMIK_MANAGERS_STATIC_ALLOCATOR_H
#define _DYNAMIK_MANAGERS_STATIC_ALLOCATOR_H

#include "memoryFunctions.h"
#include "core/global/constants.h"
#include "core/global/datatypes.h"
#include "core/types/Pointer.h"
#ifdef DMK_MEMORY_USE_NEW
#include <memory>

#endif

namespace Dynamik {
	/* TEMPLATED
	 * BASE: Allocator
	 * Static Allocator for the Dynamik Engine.
	 * This allocator is used for manual heap allocation and deletion.
	 * This class is used as the default memory allocator for the ARRAY<TYPE> object.
	 */
	template<class TYPE>
	class StaticAllocator : public Allocator {
	public:
		StaticAllocator() {}
		~StaticAllocator() {}

		static POINTER<TYPE> allocate(UI32 size = 0, UI32 align = DMK_MEMORY_ALIGN, UI32 offset = 0)
		{
#if defined(DMK_MEMORY_USE_MALLOC)
			return (TYPE*)malloc(size);

#elif defined(DMK_MEMORY_USE_NEW)
			return (TYPE*)operator new[](size, std::align_val_t{ (size_t)align });
#endif
		}

		static void deAllocate(POINTER<TYPE> data, UI32 size = 0, UI32 align = DMK_MEMORY_ALIGN, UI32 offset = 0)
		{
			//#if defined(DMK_MEMORY_USE_MALLOC)
			//			free((VPTR)data.get());
			//
			//#elif defined(DMK_MEMORY_USE_NEW)
			//			delete[] data.get();
			//
			//#endif
			setData(data, (TYPE)0, size);
			operator delete[](data.get(), size);
		}

		static void deAllocate(POINTER<TYPE> first, POINTER<TYPE> last)
		{
			operator delete[](first.get(), (size_t)(last - first));
		}
	};

	/* TEMPLATED
	 * Dynamic Container to store heap allocated data.
	 */
	template<class TYPE>
	struct DynamicContainer {
		DynamicContainer(TYPE* data, UI32 size) : myData(data), myAllocatedSize(size) {}
		DynamicContainer(UI32 size, UI32 align = DMK_MEMORY_ALIGN, UI32 offset = 0)
			: myAllocatedSize(size), myAllignment(align), myOffset(offset)
		{
			myData = StaticAllocator<TYPE>::allocate(size, align, offset);
		}
		~DynamicContainer()
		{
			StaticAllocator<TYPE>::deAllocate(myData.get(), myAllocatedSize, myAllignment, myOffset);
		}

		POINTER<TYPE> myData;

		UI32 myAllocatedSize = 0;
		UI32 myAllignment = DMK_MEMORY_ALIGN;
		UI32 myOffset = 0;
	};
}

#endif // !_DYNAMIK_MANAGERS_STATIC_ALLOCATOR_H
