#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H
#define _DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H

#include "core/global/datatypes.h"
#include "Platform/Windows/memory/oneTimeAllocator.h"

namespace Dynamik {
	/*
	 Array datatype for the Dynamik Engine
	*/
	template<class TYPE>
	class ARRAY {
		/* PUBLIC FUNCTIONS */
	public:
		// default constructor
		ARRAY()
			: myTypeSize(sizeof(TYPE)), myMultipleCount(1)
		{
			myAllocationSize = DMK_MEMORY_ALIGN * myTypeSize;
			myData = (TYPE*)myAllocator.allocate(myAllocationSize);
			myElementPointer = myData;
			myBeginAddr = myData;
		}
		// default initializer
		ARRAY(UI32 size)
			: myTypeSize(sizeof(TYPE)), myDataCount(size)
		{
			myAllocationSize = size * myTypeSize;
			myData = (TYPE*)myAllocator.allocate(myAllocationSize);
			myMultipleCount = myAllocationSize / DMK_MEMORY_ALIGN;
			myInitType = ArrayInitializationType::STATIC;
			myElementPointer = myData;
			myBeginAddr = myData;
		}
		// default initializer
		ARRAY(UI32 size, TYPE value)
			: myTypeSize(sizeof(TYPE)), myDataCount(size)
		{
			myAllocationSize = size * myTypeSize;
			myData = (TYPE*)myAllocator.allocate(myAllocationSize);
			memset(myData, value, myAllocationSize);
			myMultipleCount = myAllocationSize / DMK_MEMORY_ALIGN;
			myInitType = ArrayInitializationType::STATIC;
			myElementPointer = myData;
			myBeginAddr = myData;
		}
		// default destructor
		~ARRAY() {}

		// Array iterator
		typedef TYPE* ITERATOR;

		// push elements to the array
		void pushBack(TYPE data) {
			if (myDataCount == myAllocationSize) {
				if (myInitType == ArrayInitializationType::STATIC)
					return;

				UI32 _newSize = _getNextSize();
				myAllocationSize += _newSize;
				myAllocator.extend(_newSize);
				myAllocationCount++;
			}

			*myElementPointer = data;
			myEndAddr = myElementPointer;
			myElementPointer++;
			myDataCount++;
		}

		// get the size of the array
		UI32 size() { return myDataCount; }

		// return the value from the array at a given index
		TYPE at(I32 index = 0) {
			if (index >= myDataCount || (index <= (0 - myDataCount))); // TODO: error handling

			if (index < 0)
			{
				index *= (-1);
				index = (myDataCount - index);
			}

			return myData[index];
		}

		// clear the whole array
		void clear() {
			myAllocator.deAllocate();
			myData = (TYPE*)myAllocator.allocate(myAllocationSize);
			myDataCount = 0;
		}

		// resize the array
		void resize(UI32 size) {
			myAllocationSize = size;
			myAllocator.deAllocate();
			myData = (TYPE*)myAllocator.allocate(myAllocationSize);
			myDataCount = 0;
		}

		// resize the array and initialize it
		void resize(UI32 size, TYPE value) {
			myAllocationSize = size;
			myAllocator.deAllocate();
			myData = (TYPE*)myAllocator.allocate(myAllocationSize);
			_setValue(value, myAllocationSize);
			myDataCount = (myAllocationSize - 1);
		}

		// fill values in a range
		void fill(TYPE value, UI32 startIndex, UI32 endIndex) {
			if ((endIndex >= myDataCount) || (startIndex < endIndex)); // TODO:: error handling

			UI32 _itr = startIndex;
			while (_itr <= endIndex) {
				*_getPointer(_itr) = value;
				_itr++;
			}
		}

		// update a pre existing value
		void update(TYPE value, UI32 index = 0) {
			if (index >= myDataCount); // TODO: error handling

			TYPE* _valuePointer = _getPointer(index);
			_valuePointer = value;
		}

		// get the data address
		VPTR data() { return myData; }

		// begin iterator
		ITERATOR begin() { return myBeginAddr; }

		// end iterator
		ITERATOR end() { return myEndAddr; }

		/* OPERATORS */
		// [] operator overload
		TYPE operator[](I32 index) {
			return at(index);
		}

		// == operator overload
		B1 operator==(ARRAY<TYPE> data) {
			if (this->myDataCount != data.size())
				return false;
			for (UI32 _itr = 0; _itr < this->myDataCount; _itr++)
				if (this->at(_itr) != data.at(_itr))
					return false;

			return true;
		}

		/* STATIC FUNCTIONS */
		// copy data from the source array to the destination array
		template<class SUB_TYPE>
		static void copy(ARRAY<SUB_TYPE>* source, ARRAY<SUB_TYPE>* destination) {
			*destination = *source;
		}

		/* PRIVATE FUNCTIONS AND VARIABLES */
	private:
		// get the next size
		inline UI32 _getNextSize() {
			//return (++myMultipleCount * DMK_MEMORY_ALIGN);
			return DMK_MEMORY_ALIGN * myTypeSize;
		}

		// copy data from one array location to another
		inline void _copyData(VPTR source, VPTR destination, UI32 size) {
			VPTR _sourceDataPointer = source;
			VPTR _destinationDataPointer = destination;

			UI32 _itr = size;
			while (--_itr) {
				//*_sourceDataPointer = *_destinationDataPointer;
			}
		}

		// move data to a destination
		inline void _moveData(VPTR newSpace, UI32 newSpaceSize) {
			memcpy(newSpace, myData, myAllocationSize);
			//_copyData(newSpace, myData, newSpaceSize);
			myAllocator.extend(newSpaceSize);
			myData = (TYPE*)myAllocator.getAddress();
			memcpy(myData, newSpace, newSpaceSize);
		}

		// get the address by using the index
		inline TYPE* _getPointer(UI32 index = 0) {
			TYPE* _localPointer = myData;
			while (index--)_localPointer++;
			return _localPointer;
		}

		// set value to a n amount of elements
		inline void _setValue(TYPE value, UI32 count) {
			UI32 _itr = (count - 1);
			while (--_itr) *_getPointer(_itr) = value;
		}

		TYPE* myData = nullptr;		// main data store
		TYPE* myElementPointer = nullptr;	// pointer to the element
		UI32 myDataIndex = 0;		// index where the next data is placed
		UI32 myDataCount = 0;		// number of elements stored
		UI32 myAllocationSize = 0;			// allocation size
		UI32 myMultipleCount = 0;	// number of 32bit (4 byte) multiples in the store
		UI32 myTypeSize = 0;		// bytes

		ITERATOR myBeginAddr = nullptr;	// array begin iterator
		ITERATOR myEndAddr = nullptr;	// array end iterator

		OneTimeAllocator myAllocator = {};	// allocator

		// array initialization type enum
		enum class ArrayInitializationType {
			STATIC,
			DYNAMIC
		} myInitType = ArrayInitializationType::DYNAMIC; // array initialization variable

		UI32 myAllocationCount = 0;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H
