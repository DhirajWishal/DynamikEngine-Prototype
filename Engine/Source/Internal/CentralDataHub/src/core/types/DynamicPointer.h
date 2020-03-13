#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_DYNAMIC_POINTER_H
#define _DYNAMIK_CENTRAL_DATA_HUB_DYNAMIC_POINTER_H

#include "core/global/datatypes.h"
#include "Platform/Windows/memory/OneTimeAllocator.h"
#include "Array.h"
#include "FastString.h"
#include "Pair.h"

namespace Dynamik {
	template<class TYPE>
	class ARRAY;
	class FSTR;
	class OneTimeAllocator;

	/*
	 Dynamic Pointer class for the Dynamik Engine.
	*/
	template<class TYPE>
	class DPTR {
	public:
		DPTR() {}
		DPTR(const TYPE& data) : myAddressSize(sizeof(data)), myTypeSize(sizeof(TYPE))
		{
			myPointer = (TYPE*)Allocator::basicAlloc(sizeof(data));
			*myPointer = data;
		}
		~DPTR() 
		{
			Allocator::basicDeAlloc(myPointer, myAddressSize);
		}

	private:
		TYPE* myPointer = nullptr;
		UI32 myAddressSize = 0;
		UI32 myTypeSize = 0;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_DYNAMIC_POINTER_H
