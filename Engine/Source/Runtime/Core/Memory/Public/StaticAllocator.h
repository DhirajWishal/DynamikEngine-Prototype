#pragma once
#ifndef _DYNAMIK_MEMORY_STATIC_ALLOCATOR_H
#define _DYNAMIK_MEMORY_STATIC_ALLOCATOR_H

#include "Allocator.h"
#include "memoryFunctions.h"
#include "Public/constants.h"
#include "Public/datatypes.h"
#include "Public/Pointer.h"
#ifdef DMK_MEMORY_USE_NEW
#include <memory>

#endif

namespace Dynamik {
	template<class TYPE>
	constexpr TYPE&& removeReference(TYPE& value) noexcept
	{
		return static_cast<TYPE&&>(value);
	}

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

		static TYPE* allocate(UI32 size = 1, UI32 align = DMK_MEMORY_ALIGN, UI32 offset = 0)
		{
#if defined(DMK_MEMORY_USE_MALLOC)
			return (TYPE*)malloc(size);

#elif defined(DMK_MEMORY_USE_NEW)
			//return (TYPE*)operator new[](size, std::align_val_t{ (size_t)align });
			return (TYPE*)operator new[](size);
#endif
		}

		static void deAllocate(POINTER<TYPE> data, UI32 size = 1, UI32 align = DMK_MEMORY_ALIGN, UI32 offset = 0)
		{
			//#if defined(DMK_DYNAMIK_MEMORY_USE_MALLOC)
			//			free((VPTR)data.get());
			//
			//#elif defined(DMK_DYNAMIK_MEMORY_USE_NEW)
			//			delete[] data.get();
			//
			//#endif
			if (size > 1)
				operator delete[](data.get(), size);
			else
				operator delete[](data.get());
		}

		static void deAllocate(POINTER<TYPE> first, POINTER<TYPE> last)
		{
			operator delete[](first.get(), last.get());
		}

		static void set(POINTER<TYPE> address, TYPE&& value)
		{
			new ((VPTR)address.get()) TYPE(removeReference<TYPE&&>(value));
		}

		template<class SUB_TYPE = TYPE>
		static SUB_TYPE* castedAllocate(UI32 size = 1, UI32 align = DMK_MEMORY_ALIGN, UI32 offset = 0)
		{
			return new SUB_TYPE(size);
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
		UI32 myAllignment = MEMORY_ALIGN;
		UI32 myOffset = 0;
	};
}

#endif // !_STATIC_ALLOCATOR_H
