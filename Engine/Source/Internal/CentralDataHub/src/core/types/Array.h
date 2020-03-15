#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H
#define _DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H

#include "core/global/datatypes.h"
#include "Pointer.h"
#include "Platform/Windows/memory/StaticAllocator.h"

namespace Dynamik {
	template<class TYPE>
	class StaticAllocator;
	template<class TYPE>
	class POINTER;

	/* TEMPLATED
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
			myData = StaticAllocator<TYPE>::allocate(myAllocationSize);
			setData(myData, (TYPE)0, myAllocationSize);

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
			myData = StaticAllocator<TYPE>::allocate(myAllocationSize);
			setData(myData, value, myAllocationSize);

			isStaticallyAllocated = true;
			myElementPointer = myData;
			myBeginAddr = myData;
		}

		ARRAY(const POINTER<TYPE> arr)
			: myTypeSize(sizeof(TYPE))
		{
			myAllocationSize = _getNextSizeToFit(_getSizeOfRawArray(arr));
			myData = StaticAllocator<TYPE>::allocate(myAllocationSize);
			setData(myData, (TYPE)0, myAllocationSize);
		}

		/* DESTRUCTOR
		 * Default Destructor.
		 */
		~ARRAY()
		{
			StaticAllocator<TYPE>::deAllocate(myData.get());
		}

		/* PRIVATE DATA TYPES */
	public:
		/* DATATYPE
		 * Iterator of the Array.
		 */
		typedef TYPE* ITERATOR;

		/* PRIVATE FUNCTIONS */
	public:
		/* FUNCTION
		 * Set the Array to a pre defined one.
		 *
		 * @param arr: Array to assign this with.
		 */
		void set(const POINTER<TYPE> arr)
		{
			if (isStaticallyAllocated || myData.isValid())
				StaticAllocator<TYPE>::deAllocate(myData.get(), myAllocationSize);

			myAllocationSize = _getNextSizeToFit(_getSizeOfRawArray(arr));
			myData = StaticAllocator<TYPE>::allocate(myAllocationSize);
			moveBytes(myData, arr, myAllocationSize);
		}

		/* FUNCTION
		 * Push Elements to the Array.
		 *
		 * @param data: Data to be added to the Array.
		 */
		void pushBack(const TYPE& data)
		{
			if (myDataCount < myAllocationSize)
			{
				myElementPointer = data;

				myElementPointer += 1;
				myDataCount++;
			}
			else
			{
				if (isStaticallyAllocated)
					return;

				_reAllocateAndPushBack(_getNextSize(), data);
			}
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
		 * Resize the Array to a set size which cannot be resized.
		 *
		 * @param size: The size to be allocated to (number of data to hold).
		 */
		void setSize(const UI32 size) {
			if (myData.isValid())
				StaticAllocator<TYPE>::deAllocate(myData, myAllocationSize);

			myAllocationSize = size * myTypeSize;
			myData = StaticAllocator<TYPE>::allocate(myAllocationSize);
		}

		/* FUNCTION
		 * Resize the Array to a set size and initialize it with values which cannot be resized.
		 *
		 * @param size: The size to be allocated to (number of data to hold).
		 * @param value: The value to fill the array with.
		 */
		void setSizeAndValue(const UI32& size, const TYPE& value) {
			ARRAY(size, value);
		}

		/* FUNCTION
		 * Return the value at the given index.
		 * Flags an error if the given index is invalid.
		 *
		 * @param index: Index to be accessed.
		 */
		TYPE& at(I32 index = 0)
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
		 * Return the value at the given index.
		 * Flags an error if the given index is invalid.
		 *
		 * @param index: Index to be accessed.
		 */
		TYPE& at(I32 index = 0) const
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
			StaticAllocator<TYPE>::deAllocate(myData.get(), myAllocationSize);
			myData = StaticAllocator<TYPE>::allocate(myAllocationSize);
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
			StaticAllocator<TYPE>::deAllocate(myData.get(), myAllocationSize);
			myData = StaticAllocator<TYPE>::allocate(myAllocationSize);
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
			StaticAllocator<TYPE>::deAllocate(myData.get(), myAllocationSize);
			myData = StaticAllocator<TYPE>::allocate(myAllocationSize);
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
		VPTR data() { return myData.get(); }

		/* FUNCTION
		 * Begin iterator of the Array.
		 */
		ITERATOR begin() { return myBeginAddr.get(); }

		/* FUNCTION
		 * End iterator of the Array.
		 */
		ITERATOR end() { return myElementPointer.get(); }

		/* FUNCTION
		 * Insert the content of another Array to the current Array.
		 *
		 * @param arr: Array with the same type to be concatted.
		 */
		void insert(ARRAY<TYPE> arr)
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

			myData.setIndexed(value, index);
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
						if (_localArray->at(_index) > _localArray->at(_itr))
						{
							TYPE _temp = _localArray->at(_index);
							_localArray->update(_localArray->at(_itr), _index);
							_localArray->update(_temp, _itr);
						}
					}
					else
					{
						if (_localArray->at(_index) < _localArray->at(_itr))
						{
							TYPE _temp = _localArray->at(_index);
							_localArray->update(_localArray->at(_itr), _index);
							_localArray->update(_temp, _itr);
						}
					}
					_index++;
					_itr++;
				}
				_indexCount++;
			}
		}

		/* PUBLIC OPERATORS */
	public:
		/* OPERATOR
		 * [] operator overload.
		 * Return the value at the given index.
		 * Raises a flag if the index is invalid.
		 *
		 * @param index: Index to be returned.
		 */
		TYPE& operator[](I32 index)
		{
			return this->at(index);
		}

		/* OPERATOR
		 * [] operator overload.
		 * Return the value at the given index.
		 * Raises a flag if the index is invalid.
		 *
		 * @param index: Index to be returned.
		 */
		TYPE& operator[](I32 index) const
		{
			return at(index);
		}

		/* OPERATOR
		 * == operator overload.
		 * Check if the arrays are similar.
		 *
		 * @param data: Array to be checked with.
		 */
		B1 operator==(ARRAY<TYPE>& data)
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

		ARRAY<TYPE> operator=(const POINTER<TYPE> arr)
		{
			this->set(arr);
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

		/* STATIC
		 * Move a data from one location to another.
		 *
		 * @param destination: Where to place the data.
		 * @param source: Where to move the data from.
		 * @param size: number of bytes to move.
		 */
		template<class SUB_TYPE>
		static void moveBytes(POINTER<SUB_TYPE> destination, const POINTER<SUB_TYPE> source, UI32 size = 0)
		{
			POINTER<SUB_TYPE> _sourceIterator = (POINTER<SUB_TYPE>)source;
			POINTER<SUB_TYPE> _destinationIterator = destination;
			while (size--) {
				*_destinationIterator = *_sourceIterator;
				_destinationIterator++, _sourceIterator++;
			}
		}

		/* STATIC
		 * Fill a given address with a given data value.
		 *
		 * @param address: Where to place the data.
		 * @param value: Value to be filled with
		 * @param size: number of bytes to fill.
		 */
		template<class SUB_TYPE>
		static void setData(POINTER<SUB_TYPE> address, const SUB_TYPE value, UI32 size = 0, const UI32 typeSize = 1)
		{
			POINTER<SUB_TYPE> _iterator = address;
			UI32 _runSize = size / typeSize;
			do {
				_iterator = value;
				_iterator++;
			} while (--_runSize);
			_iterator--;
		}

		/* PRIVATE FUNCTIONS */
	private:
		/* PRIVATE
		 * Basic initializings.
		 */
		void _basicInitialization(TYPE* dataStore, UI32 updatedSize = 0)
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
			moveBytes(myData, (POINTER<TYPE>)newSpace, newSpaceSize);
		}

		/* PRIVATE
		 * Get the next pointer to data from a given index.
		 *
		 * @param index: Index of the pointer to retrieve.
		 */
		inline TYPE* _getPointer(UI32 index = 0)
		{
			TYPE* _localPointer = myData.get();
			(UI32)_localPointer + (index * myTypeSize);
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
		inline UI32 _getSizeOfRawArray(const POINTER<TYPE> arr)
		{
			return sizeof(arr.get()) / sizeof(TYPE);
		}

		/* PRIVATE
		 * Resize the data store.
		 *
		 * @param newSize: New size added to the pre-allocated size to be re-allocated.
		 */
		inline void _reAllocateAndPushBack(UI32 newSize, const TYPE& data)
		{
			POINTER<TYPE> _newArray = StaticAllocator<TYPE>::allocate(newSize + myAllocationSize);
			POINTER<TYPE> _nextElement = _newArray + myDataCount;
			POINTER<TYPE> _localData = myData;
			memcpy(_newArray.get(), myData.get(), myDataCount * myTypeSize);
			_nextElement = data;

			myData = _newArray;
			myBeginAddr = myData;
			myElementPointer = _nextElement + (UI32)1;

			myDataCount++;
			_newArray.turnNull();
			_nextElement.turnNull();

			if (_localData.isValid())
			{
				StaticAllocator<TYPE>::deAllocate(_localData, myAllocationSize);
			}

			myAllocationSize += newSize;
		}

		/* PRIVATE VARIABLES AND CONSTANTS */
	private:
		POINTER<TYPE> myData;		// main data store
		POINTER<TYPE> myBeginAddr;	// array begin iterator
		POINTER<TYPE> myElementPointer;	// pointer to the element

		UI32 myDataCount = 0;		// number of elements stored
		UI32 myAllocationSize = 0;	// allocation size
		UI32 myTypeSize = 0;		// size of the datatype in bytes

		B1 isStaticallyAllocated = false;	// check if the array is initialized to a pre defined size

		struct AllocationDataContainer {

		};
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_ARRAY_H
