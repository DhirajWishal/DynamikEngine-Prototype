#pragma once
#ifndef _DYNAMIK_MEMORY_ALLOCATOR_H
#define _DYNAMIK_MEMORY_ALLOCATOR_H

#include "DataTypesLib/Public/constants.h"
#include "DataTypesLib/Public/datatypes.h"

namespace Dynamik {
	/* MAIN ALLOCATOR CLASS */
	class Allocator {
	public:
		Allocator() {}
		Allocator(UI32 size);
		virtual ~Allocator();

		virtual VPTR allocate(UI32 size = 0, UI32 align = DMK_MEMORY_ALIGN, UI32 offset = 0) { return nullptr; }
		virtual void deAllocate(VPTR data = nullptr, UI32 size = 0, UI32 offset = 0) {}
		virtual void extend(UI32 size = 0, UI32 offset = 0) {}

		virtual void pack() {}

		_declspec(allocator) static VPTR basicAlloc(UI32 size = 0, UI32 offset = 0);
		_declspec(allocator) static VPTR basicAllignedAlloc(UI32 size = 0, UI32 align = DMK_MEMORY_ALIGN);
		static void basicDeAlloc(VPTR data = nullptr, UI32 size = 0, UI32 offset = 0);
		static void basicAllignedDeAlloc(VPTR data, UI32 size = 0, UI32 align = DMK_MEMORY_ALIGN);

		static void clearRange(VPTR begin, VPTR end);

		// set memory of a given pointer to a given value
		template<class TYPE>
		static void setAddress(TYPE* pointer, TYPE value, UI32 count)
		{
			while (count--)
			{
				*pointer = value;
				pointer++;
			}
		}

	protected:
		template<typename T>
		VPTR _incrementBy(VPTR address, UI32 size) {
			return (T*)address + size;
		}

		VPTR memoryPool = nullptr;
		UI32 memorySize = 0;
		UI32 myAlign = 0;
	};
}

#endif // !_DYNAMIK_MEMORY_ALLOCATOR_H
