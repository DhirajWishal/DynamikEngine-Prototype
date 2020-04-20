#pragma once
#ifndef _DYNAMIK_DATA_TYPES_DYNAMIC_POINTER_H
#define _DYNAMIK_DATA_TYPES_DYNAMIC_POINTER_H

#include "datatypes.h"
#include "Public/StaticAllocator.h"
#include "Array.h"
#include "FastString.h"
#include "Pair.h"

namespace Dynamik {
	template<class TYPE>
	class StaticAllocator;
	template<class TYPE>
	struct DynamicContainer;

	/*
	 * Dynamic Pointer class for the Dynamik Engine.
	 */
	template<class TYPE>
	class DPTR {
		/* DEFAULTS */
	public:
		/* CONSTRUCTOR
		 * Default constructor.
		 */
		DPTR() {}

		/* CONSTRUCTOR
		 * Default constructor.
		 *
		 * @param data: Data to be stored.
		 */
		DPTR(const TYPE& data) : myAllocatedSize(sizeof(data))
		{
			myPointer = StaticAllocator<TYPE>::allocate(myAllocatedSize, myAlignment, myOffset);
			myPointer = data;
		}

		/* DESTRUCTOR
		 * Default destructor.
		 */
		~DPTR()
		{
			StaticAllocator<TYPE>::deAllocate(myPointer, myAllocatedSize, myAlignment, myOffset);
			isDeAllocated = true;
		}

		/* PUBLIC FUNCTIONS */
	public:
		/* FUNCTION
		 * Returns the address of the stored data.
		 */
		POINTER<TYPE> getAddress() { return myPointer; }

		/* FUNCTION
		 * Returns the address of the data store.
		 */
		POINTER<TYPE*> getAddressOfStore() { return (POINTER<TYPE*>)myPointer.getAddress(); }

		/* FUNCTION
		 * Return the allocated size of the data.
		 */
		UI32 getAllocatedSize() { return myAllocatedSize; }

		/* PUBLIC OPERATORS */
	public:
		/* OPERATOR
		 * Assign the pointer of the other Dynamic Pointer to this.
		 */
		DPTR operator()(const DPTR& ptr)
		{
			this->myPointer = ptr.myPointer;
			return *this;
		}

		/* PRIVATE VARIABLES AND CONSTANTS */
	private:
		POINTER<TYPE> myPointer;	// pointer to the data

		UI32 myAllocatedSize = 0;	// size of the allocation
		UI32 myAlignment = DMK_MEMORY_ALIGN;	// alignment size
		UI32 myOffset = 0;	// memory offset
		bool isDeAllocated = false;	// check if the pointer is de-allocated
	};
}

#endif // !_DATA_TYPES_DYNAMIC_POINTER_H
