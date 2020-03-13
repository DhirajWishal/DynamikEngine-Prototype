#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H
#define _DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H

#include "core/global/datatypes.h"
#include "Platform/Windows/memory/oneTimeAllocator.h"

namespace Dynamik {
	class OneTimeAllocator;

	/*
	 * Dynamic Array datatype for the Dynamik Engine
	 * This array can store any data defined in the datatype TYPE and supports multiple dimentions.
	 * Tested to be faster than the std::vector<TYPE> library/ datatype.
	 * This also contains utility functions related to array and pointer manipulation.
	 */
	template<class TYPE>
	class ARRAY {
		/* DEFAULTS */
	public:
		/* CONSTRUCTOR
		 * Default Constructor.
		 */
		ARRAY()
			: myTypeSize(sizeof(TYPE))
		{
			myAllocationSize = _getNextSize();
			myData = (TYPE*)myAllocator.allocate(myAllocationSize);

			myElementPointer = myData;
			myBeginAddr = myData;
		}

		/* CONSTRUCTOR
		 * Default Constructor and Initializer.
		 * Sets the size of the Array to a specified amount.
		 * Data added after the limit will not be included.
		 *
		 * @param size: Size to be allocated.
		 */
		ARRAY(UI32 size)
			: myTypeSize(sizeof(TYPE))
		{
			myAllocationSize = size * myTypeSize;
			myData = (TYPE*)myAllocator.allocate(myAllocationSize);

			isStaticallyAllocated = true;
			myElementPointer = myData;
			myBeginAddr = myData;
		}

		/* CONSTRUCTOR
		 * Default Constructor and Initializer.
		 * Sets the size of the Array to a specified amount and initializes it with a value.
		 * Data added after the limit will not be included.
		 *
		 * @param size: Size to be allocated.
		 * @param value: Value to initialize the Array with.
		 */
		ARRAY(UI32 size, const TYPE& value)
			: myTypeSize(sizeof(TYPE))
		{
			myAllocationSize = size * myTypeSize;
			myData = (TYPE*)myAllocator.allocate(myAllocationSize);

			myElementPointer = myData;
			ITERATOR _localIter = myData;
			while (size--) { *_localIter = value, _localIter++; }

			isStaticallyAllocated = true;
			myBeginAddr = myData;
		}

		ARRAY(const TYPE* arr)
			: myTypeSize(sizeof(TYPE)), 
		{

		}

		/* DESTRUCTOR
		 * Default Destructor.
		 */
		~ARRAY() {}

		/* PRIVATE DATA TYPES */
	public:
		/* DATATYPE
		 * Iterator of the Array.
		 */
		typedef TYPE* ITERATOR;

		/* PRIVATE FUNCTIONS */
	public:
		/* FUNCTION
		 * Push Elements to the Array.
		 *
		 * @param data: Data to be added to the Array.
		 */
		void pushBack(const TYPE& data)
		{
			if (myDataCount == myAllocationSize)
			{
				if (isStaticallyAllocated)
					return;

				UI32 _newSize = _getNextSize();
				myAllocationSize += _newSize;
				myAllocator.extend(_newSize);
				//_basicInitialization((TYPE*)myAllocator.getAddress(), myDataCount);

#ifdef DMK_DEBUG
				myAllocationCount++;
#endif
			}

			*myElementPointer = data;

			myElementPointer++;
			myDataCount++;
		}

		/* FUNCTION
		 * Push Elements to the Array.
		 * Compatibility with the 'std::vector<TYPE>::push_back(const TYPE&)' function.
		 *
		 * @param data: Data to be added to the Array.
		 */
		void push_back(const TYPE& value)
		{
			pushBack(value);
		}

		/* FUNCTION
		 * Return the size of the Array.
		 */
		UI32 size() { return myDataCount; }

		/* FUNCTION
		 * Return the size of TYPE.
		 */
		UI32 sizeOfType() { return myTypeSize; }

		/* FUNCTION
		 * Return the current allocation size.
		 * This size is the size available till the next allocation.
		 */
		UI32 allocationSize() { return myAllocationSize; }

		/* FUNCTION
		 * Return the value at the given index.
		 * Flags an error if the given index is invalid.
		 *
		 * @param index: Index to be accessed.
		 */
		TYPE at(I32 index = 0)
		{
			if (index >= myDataCount || (index <= (0 - myDataCount))); // TODO: error handling

			if (index < 0)
			{
				index *= (-1);
				index = (myDataCount - index);
			}

			return myData[index];
		}

		/* FUNCTION
		 * Clear the whole Array.
		 */
		void clear()
		{
			myAllocator.deAllocate();
			myData = (TYPE*)myAllocator.allocate(myAllocationSize);
			myDataCount = 0;
		}

		/* FUNCTION
		 * Resize the Array.
		 *
		 * @param size: Size to which the Array should be resized.
		 */
		void resize(UI32 size)
		{
			myAllocationSize = size;
			myAllocator.deAllocate();
			myData = (TYPE*)myAllocator.allocate(myAllocationSize);
			myDataCount = 0;
		}

		/* FUNCTION
		 * Resize the Array.
		 *
		 * @param size: Size to which the Array should be resized.
		 * @param value: The value to initialize the Array with.
		 */
		void resize(UI32 size, const TYPE& value)
		{
			myAllocationSize = size;
			myAllocator.deAllocate();
			myData = (TYPE*)myAllocator.allocate(myAllocationSize);
			_setValue(value, myAllocationSize);
			myDataCount = (myAllocationSize - 1);
		}

		/* FUNCTION
		 * Fill the array with a value.
		 * If the indexes are invalid, an error is flagged.
		 *
		 * @param value: Value to fill the array with.
		 * @param startIndex: Index of the first place.
		 * @param endIndex: Index of the last place.
		 */
		void fill(const TYPE& value, UI32 startIndex, UI32 endIndex)
		{
			if ((endIndex >= myDataCount) || (startIndex < endIndex)); // TODO:: error handling

			UI32 _itr = startIndex;
			while (_itr <= endIndex)
			{
				*_getPointer(_itr) = value;
				_itr++;
			}
		}

		/* FUNCTION
		 * Update value in an index.
		 * Raises a flag if the index is invalid.
		 *
		 * @param value: Value to be updated.
		 * @param index: Index to be updated.
		 */
		void update(const TYPE& value, UI32 index = 0)
		{
			if (index >= myDataCount); // TODO: error handling

			*_getPointer(index) = value;
		}

		/* FUNCTION
		 * Get data pointer of the Array.
		 */
		VPTR data() { return myData; }

		/* FUNCTION
		 * Begin iterator of the Array.
		 */
		ITERATOR begin() { return myBeginAddr; }

		/* FUNCTION
		 * End iterator of the Array.
		 */
		ITERATOR end() { return myElementPointer; }

		/* FUNCTION
		 * Insert the content of another Array to the current Array.
		 *
		 * @param arr: Array with the same type to be concatted.
		 */
		void insert(const ARRAY<TYPE>& arr)
		{
			ITERATOR _iterator = arr.begin();
			while (_iterator != arr.end())
			{
				pushBack(*_iterator);
				_iterator++;
			}
		}

		/* FUNCTION
		 * Insert the content of another Array to the current Array.
		 *
		 * @param first: Starting iterator.
		 * @param last: End iterator.
		 */
		ITERATOR insert(ITERATOR first, ITERATOR last)
		{
			while (first != last) {
				pushBack(*first);
				first++;
			}
		}

		/* FUNCTION
		 * Emplace a value to a given index.
		 * Raises a flag if the index is invalid.
		 *
		 * @param value: Value to be updated.
		 * @param index: Index to be updated.
		 */
		void emplace(const TYPE& value, UI32 index = 0)
		{
			if (index >= myDataCount); // TODO: error handling

			myData[index] = value;
		}

		/* FUNCTION
		 * Check if the given index is valid.
		 *
		 * @param index: Index to be checked.
		 */
		B1 isValidIndex(I32 index) {
			if (index > 0) {
				if (index < myDataCount)
					return true;
			}
			else {
				index *= (-1);
				if (index > (myDataCount * (-1)))
					return true;
			}

			return false;
		}

		/* FUNCTION
		 * Bubble sort for the array.
		 * Sort the Array in ascending or descending order.
		 *
		 * @param isAsc = true: Sorting type (true = ascending, false = descending)
		 */
		void bubbleSort(bool isAsc = true)
		{
			ARRAY<TYPE>* _localArray = this;
			UI32 _indexCount = 0;
			UI32 _index, _itr;
			while (_indexCount < (myDataCount - 1))
			{
				_index = 0;
				_itr = 1;
				while (_itr < myDataCount - _indexCount)
				{
					if (isAsc)
					{
						if (data[_index] > data[_itr])
							_swap<TYPE>(&data[_index], &data[_itr]);
					}
					else
					{
						if (data[_index] < data[_itr])
							_swap<TYPE>(&data[_index], &data[_itr]);
					}
					_index++;
					_itr++;
				}
				_indexCount++;
			}
		}

		/* OPERATOR
		 * [] operator overload.
		 * Return the value at the given index.
		 * Raises a flag if the index is invalid.
		 *
		 * @param index: Index to be returned.
		 */
		TYPE operator[](I32 index)
		{
			return at(index);
		}

		/* OPERATOR
		 * == operator overload.
		 * Check if the arrays are similar.
		 *
		 * @param data: Array to be checked with.
		 */
		B1 operator==(const ARRAY<TYPE>& data)
		{
			if (this->myDataCount != data.size())
				return false;
			for (UI32 _itr = 0; _itr < this->myDataCount; _itr++)
				if (this->at(_itr) != data.at(_itr))
					return false;

			return true;
		}

		/* OPERATOR
		 * + operator overload.
		 * Concat another Array with the similar type.
		 *
		 * @param data: Array to be concatted with.
		 */
		ARRAY<TYPE> operator+(const ARRAY<TYPE>& data)
		{
			this->insert(data);
			return *this;
		}

		/* STATIC
		 * Copy contents from one Array to another.
		 *
		 * @param source: Source Array.
		 * @param destination: Destination Array.
		 */
		template<class SUB_TYPE>
		static void copy(ARRAY<SUB_TYPE>* source, ARRAY<SUB_TYPE>* destination)
		{
			*destination = *source;
		}

		/* PRIVATE FUNCTIONS */
	private:
		/* PRIVATE
		 * Basic initializings.
		 */
		void _basicInitialization(TYPE* dataStore, UI32 updatedSize = 0;)
		{
			myData = dataStore;
			myElementPointer = dataStore;
			while (updatedSize--) myElementPointer++;
		}

		/* PRIVATE
		 * Get the next allocatable size.
		 */
		inline UI32 _getNextSize()
		{
			return DMK_MEMORY_ALIGN * myTypeSize;
		}

		/* PRIVATE
		 * Get the next allocatable size to fit a given size.
		 *
		 * @param basicSize: Size to be filled with.
		 */
		inline UI32 _getNextSizeToFit(UI32 basicSize)
		{
			UI32 _nextSize = _getNextSize();
			if ((basicSize % _nextSize))
				return (((basicSize / _nextSize) * _nextSize) + _nextSize);

			return (basicSize / _nextSize) * _nextSize;
		}

		/* PRIVATE
		 * Copy data from one location to another.
		 *
		 * @param source: Source address.
		 * @param destination: Destination address.
		 * @param size: Number of bytes to be copied.
		 */
		inline void _copyData(VPTR source, VPTR destination, UI32 size)
		{
			VPTR _sourceDataPointer = source;
			VPTR _destinationDataPointer = destination;

			UI32 _itr = size;
			while (--_itr)
			{
				//*_sourceDataPointer = *_destinationDataPointer;
			}
		}

		/* PRIVATE
		 * Move data from one location to another.
		 *
		 * @param newSpace: Source address.
		 * @param newSpaceSize: Number of bytes to be moved.
		 */
		inline void _moveData(VPTR newSpace, UI32 newSpaceSize)
		{
			memcpy(newSpace, myData, myAllocationSize);
			//_copyData(newSpace, myData, newSpaceSize);
			myAllocator.extend(newSpaceSize);
			myData = (TYPE*)myAllocator.getAddress();
			memcpy(myData, newSpace, newSpaceSize);
		}

		/* PRIVATE
		 * Get the next pointer to data from a given index.
		 *
		 * @param index: Index of the pointer to retrieve.
		 */
		inline TYPE* _getPointer(UI32 index = 0)
		{
			TYPE* _localPointer = myData;
			while (index--)_localPointer++;
			return _localPointer;
		}

		/* PRIVATE
		 * Set a value to a number of elements.
		 *
		 * @param value: Value to be filled with.
		 * @param count: Number of elements to be filled with.
		 */
		inline void _setValue(TYPE value, UI32 count)
		{
			UI32 _itr = (count - 1);
			while (--_itr) *_getPointer(_itr) = value;
		}

		/* PRIVATE
		 * Extend the capacity to fit new size.
		 *
		 * @param size: Size to be extended by.
		 */
		inline void _extendDataToFitNewSize(UI32 size)
		{
			if (size > (mySize - myDataCount))
			{
				UI32 _newSize = _getNextSizeToFit(size - (mySize - myDataCount));
				myAllocationSize += _newSize;
				myAllocator.extend(_newSize);
			}
		}

		/* PRIVATE
		 * Swap two elements.
		 *
		 * @param first: First element.
		 * @param second: Second element;
		 */
		inline void _swap(TYPE* first, TYPE* second)
		{
			TYPE _temp = *first;
			*first = *second;
			*second = _temp;
		}

		/* PRIVATE
		 * Get the size of a raw Array. 
		 *
		 * @param arr: Raw Array.
		 */
		inline UI32 _getSizeOfRawArray(const TYPE* arr)
		{
			return sizeof(arr) / sizeof(arr[0]);
		}

		/* PRIVATE VARIABLES AND CONSTANTS */
	private:
		TYPE* myData = nullptr;		// main data store
		ITERATOR myBeginAddr = nullptr;	// array begin iterator
		ITERATOR myElementPointer = nullptr;	// pointer to the element

		UI32 myDataCount = 0;		// number of elements stored
		UI32 myAllocationSize = 0;	// allocation size
		UI32 myTypeSize = 0;		// size of the datatype in bytes

		B1 isStaticallyAllocated = false;	// check if the array is initialized to a pre defined size

		OneTimeAllocator myAllocator = {};	// dynamic memory allocator

#ifdef DMK_DEBUG
		UI32 myAllocationCount = 0;	// for debug purposes
#endif
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H
