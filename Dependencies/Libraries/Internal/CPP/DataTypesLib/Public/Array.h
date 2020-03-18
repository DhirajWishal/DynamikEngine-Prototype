#pragma once
#ifndef _DYNAMIK_DATA_TYPES_ARRAY_H
#define _DYNAMIK_DATA_TYPES_ARRAY_H

#include "datatypes.h"
#include "Pointer.h"
#include "MemoryLib/Public/StaticAllocator.h"
#include "InitializerList.h"

namespace Dynamik {
	template<class TYPE>
	class StaticAllocator;
	template<class TYPE>
	class POINTER;

	/* TEMPLATED
	 * Dynamic Array data structure for the Dynamik Engine.
	 * This array can store any data defined in the datatype TYPE and supports multiple dimentions.
	 * Tested to be faster than the std::vector<TYPE> library/ datatype.
	 * This also contains utility functions related to array and pointer manipulation.
	 */
	template<class TYPE, class Allocator = StaticAllocator<TYPE>>
	class ARRAY {
		/* DEFAULTS */

		/* STATIC ASSERT
		 * Check if the Template argument is not void.
		 * Compile time check.
		 */
		static_assert(!isType<TYPE, void>::result, "void Is Not Accepted As A Template Argument! ARRAY<TYPE, Allocator>");

		/* STATIC ASSERT
		 * Check if both the template arguments are same.
		 * Compile time check.
		 */
		static_assert(!isType<TYPE, Allocator>::result, "Invalid Template Arguments! ARRAY<TYPE, Allocator>");

		/* PRIVATE DATATYPE
		 * Used as a private datatype.
		 */
		using PTR = POINTER<TYPE>;

		/* PRIVATE DATATYPE
		 * Used as a raw pointer.
		 */
		using RPTR = TYPE*;

	public:
		/* CONSTRUCTOR
		 * Default Constructor.
		 */
		ARRAY()
		{
			_reAllocate(_getNextSize());
		}

		/* CONSTRUCTOR
		 * Default Constructor and Initializer.
		 * Sets the size of the Array to a specified amount.
		 * Data added after the limit will not be included.
		 *
		 * @param size: Size to be allocated.
		 */
		ARRAY(UI32 size)
		{
			myAllocationSize = size * sizeof(TYPE);

			if ((size + myAllocationSize) > maxSize()) return; /* TODO: Error Flagging */

			myBeginPtr = Allocator::allocate(myAllocationSize);
			myNextPtr = myBeginPtr;
			myEndPtr = myBeginPtr;
			myEndPtr += capacity();
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
		{
			myAllocationSize = size * sizeof(TYPE);

			if ((size + myAllocationSize) > maxSize()) return; /* TODO: Error Flagging */

			myBeginPtr = Allocator::allocate(myAllocationSize);
			setData(myBeginPtr, value, capacity());

			myEndPtr = myBeginPtr + size;
			myNextPtr += size;
		}

		/* CONSTRUCTOR
		 * Constructs the Array by initializing the data in another array.
		 *
		 * @param arr: Array used to initialize this.
		 */
		ARRAY(const POINTER<TYPE> arr)
		{
			myAllocationSize = _getNextSizeToFit(_getSizeOfRawArray(arr));

			if (myAllocationSize > maxSize()) return; /* TODO: Error Flagging */

			myBeginPtr = Allocator::allocate(myAllocationSize);
			setData(myBeginPtr, 0, capacity());

			myNextPtr = myBeginPtr;
			myEndPtr = myBeginPtr;
			myEndPtr += capacity();
		}

		/* CONSTRUCTOR
		 * Constructs the Array by initializing the data in an initializer list.
		 *
		 * @param list: Initializer list used to initialize this.
		 */
		ARRAY(InitialzerList<TYPE> list)
		{
			if (list.size() > maxSize()); /* TODO: Error Flagging */

			_reAllocate(list.size());
			for (InitialzerList<TYPE>::ITERATOR _itr = list.begin(); _itr != list.end(); ++_itr)
				pushBack(*_itr);
		}

		/* DESTRUCTOR
		 * Default Destructor.
		 */
		~ARRAY()
		{
			_destroyRange(myBeginPtr, myEndPtr);
			_destroyPointers();
			Allocator::deAllocate(myBeginPtr, myEndPtr);
		}

		/* PRIVATE DATA TYPES */
	public:
		/* DATATYPE
		 * Iterator of the Array.
		 */
		typedef PTR ITERATOR;

		/* PRIVATE FUNCTIONS */
	public:
		/* FUNCTION
		 * Set the Array to a pre defined one.
		 *
		 * @param arr: Array to assign this with.
		 */
		void set(const PTR arr)
		{
			myAllocationSize = _getNextSizeToFit(_getSizeOfRawArray(arr));

			if (myAllocationSize > maxSize()) return; /* TODO: Error Flagging */

			if (myBeginPtr.isValid())
				Allocator::deAllocate(myBeginPtr, myEndPtr);

			myBeginPtr = Allocator::allocate(myAllocationSize);
			moveBytes(myBeginPtr, arr, myAllocationSize);
		}

		/* FUNCTION
		 * Push Elements to the Array.
		 *
		 * @param data: Data to be added to the Array.
		 */
		void pushBack(const TYPE& data)
		{
			if (myDataCount == capacity()) {
				_reAllocateAndPushBack(_getNextSize(), data);
			}
			else
				_addData(data);

			myDataCount++;
		}

		/* FUNCTION
		 * Push Elements to the Array.
		 *
		 * @param data: Data to be added to the Array.
		 */
		void pushBack(TYPE&& data)
		{
			pushBack((TYPE&)data);
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
		 * Get the first element of the Array.
		 */
		TYPE front()
		{
			return myBeginPtr[0];
		}

		/* FUNCTION
		 * Get the first element in the Array and remove it.
		 */
		TYPE popFront()
		{
			TYPE _data = front();

			/* LOGIC */

			return _data;
		}

		/* FUNCTION
		 * Resize the Array to a set size which cannot be resized.
		 *
		 * @param size: The size to be allocated to (number of data to hold).
		 */
		void setSize(const UI32 size)
		{
			if (size > maxSize()) return; /* TODO: Error Flagging */

			if (myBeginPtr.isValid())
				Allocator::deAllocate(myBeginPtr, myAllocationSize);

			myAllocationSize = size * sizeof(TYPE);
			myBeginPtr = Allocator::allocate(myAllocationSize);
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

			return myBeginPtr[index];
		}

		/* FUNCTION
		 * Clear the whole Array.
		 */
		void clear()
		{
			Allocator::deAllocate(myBeginPtr.get(), myAllocationSize);
			myBeginPtr = Allocator::allocate(myAllocationSize);

			myDataCount = 0;
		}

		/* FUNCTION
		 * Resize the Array.
		 *
		 * @param size: Size to which the Array should be resized.
		 */
		void resize(UI32 size)
		{
			if (size > maxSize()) return; /* TODO: Error Flagging */

			myAllocationSize = size;
			Allocator::deAllocate(myBeginPtr.get(), myAllocationSize);
			myBeginPtr = Allocator::allocate(myAllocationSize);

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
			if (size > maxSize()) return; /* TODO: Error Flagging */

			myAllocationSize = size;
			Allocator::deAllocate(myBeginPtr.get(), myAllocationSize);
			myBeginPtr = Allocator::allocate(myAllocationSize);

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

			myBeginPtr[index] = value;
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

		/* FUNCTION
		 * Return the size of the Array.
		 *
		 * @return: Element count.
		 */
		UI32 size() { return myDataCount; }

		/* FUNCTION
		 * Return the maximum size allocatable in an Array.
		 *
		 * @return: Element count.
		 */
		UI32 maxSize() { return ((UI32)-1) / sizeof(TYPE); }

		/* FUNCTION
		 * Return the number of elements that can be stored before the next allocation.
		 *
		 * @return: Element count.
		 */
		UI32 capacity() { return myAllocationSize / sizeof(TYPE); }

		/* FUNCTION
		 * Return the size of TYPE.
		 */
		UI32 sizeOfType() { return sizeof(TYPE); }

		/* FUNCTION
		 * Return the current allocation size.
		 * This size is the size available till the next allocation.
		 *
		 * @return: Byte count.
		 */
		UI32 allocationSize() { return myAllocationSize; }

		/* FUNCTION
		 * Get data pointer of the Array.
		 */
		VPTR data()
		{
			return myBeginPtr;
		}

		/* FUNCTION
		 * Begin iterator of the Array.
		 */
		ITERATOR begin()
		{
			return myBeginPtr;
		}

		/* FUNCTION
		 * End iterator of the Array.
		 */
		ITERATOR end()
		{
			return myNextPtr;
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

		/* OPERATOR
		 * = operator overload.
		 * Get data from a raw array and initialize it to this.
		 *
		 * @para arr: Raw array to be initialized to this.
		 */
		ARRAY<TYPE> operator=(const PTR arr)
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

		/* PRIVATE FUNCTIONS */
	private:
		/* PRIVATE
		 * Basic initializings.
		 */
		void _basicInitialization(TYPE* dataStore, UI32 updatedSize = 0)
		{
			myBeginPtr = dataStore;
			myEndPtr = dataStore;
			myEndPtr += updatedSize;
		}

		/* PRIVATE
		 * Get the next allocatable size.
		 */
		inline UI32 _getNextSize()
		{
			UI32 _oldCapacity = capacity();

			if (_oldCapacity > (maxSize() - _oldCapacity / 2))
				return DMK_MEMORY_ALIGN * sizeof(TYPE);

			UI32 _nextCapacity = _oldCapacity + _oldCapacity / 2;

			if (_nextCapacity < DMK_MEMORY_ALIGN * sizeof(TYPE))
				return DMK_MEMORY_ALIGN * sizeof(TYPE);

			return _nextCapacity;
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
			moveBytes(myBeginPtr, (PTR)newSpace, newSpaceSize);
		}

		/* PRIVATE
		 * Get the next pointer to data from a given index.
		 *
		 * @param index: Index of the pointer to retrieve.
		 */
		inline TYPE* _getPointer(UI32 index = 0)
		{
			RPTR _localPointer = myBeginPtr;
			(UI32)_localPointer + (index * sizeof(TYPE));
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
		inline UI32 _getSizeOfRawArray(const PTR arr)
		{
			return sizeof(arr.get()) / sizeof(TYPE);
		}

		/* PRIVATE FUNCTION
		 * Destroy all the data stored in a given range.
		 * Calls the destructor for all the data stored.
		 *
		 * @param first: Pointer to the first element.
		 * @param last: Pointer to the last element.
		 */
		inline void _destroyRange(PTR first, PTR last)
		{
			TYPE* _data = nullptr;
			while (first != last)
			{
				_singleDestruct(first);
				++first;
			}
		}

		/* PRIVATE
		 * Destroy all the pointers that contained the heap data.
		 * Calls the destructor.
		 */
		inline void _destroyPointers()
		{
			myBeginPtr.~POINTER();
			myEndPtr.~POINTER();
			myNextPtr.~POINTER();
		}

		/* FUNCTION
		 * Add data to the end of the Array.
		 *
		 * @param data: Data to be emplaced.
		 */
		inline void _addData(const TYPE& data)
		{
			Allocator::set(&myBeginPtr[myDataCount], (TYPE&&)data);
			myNextPtr++;
		}

		/* PRIVATE
		 * Resize the data store.
		 *
		 * @param newSize: New size added to the pre-allocated size to be re-allocated.
		 */
		inline void _reAllocate(UI32 newSize)
		{
			PTR _newAlloc = Allocator::allocate(newSize + myAllocationSize);
			myAllocationSize += newSize;
			PTR _endPtr = _newAlloc;
			_endPtr += myAllocationSize;

			try
			{
				if (myBeginPtr.isValid())
				{
					moveBytes(_newAlloc, myBeginPtr, myDataCount);
					Allocator::deAllocate(myBeginPtr, myEndPtr);
				}
			}
			catch (...)
			{
				_destroyRange(_newAlloc, _endPtr);
				Allocator::deAllocate(_newAlloc, _endPtr);

				throw;
			}

			myBeginPtr(_newAlloc);
			myEndPtr(_endPtr);
			myNextPtr = myBeginPtr;
			myNextPtr += myDataCount;
		}

		/* PRIVATE
		 * Resize the data store and add a value to the end.
		 *
		 * @param newSize: New size added to the pre-allocated size to be re-allocated.
		 * @param data: Data to be inserted to the end.
		 */
		inline void _reAllocateAndPushBack(UI32 newSize, const TYPE& data)
		{
			if ((newSize + myAllocationSize) > maxSize()) return; /* TODO: Error Flagging */

			_reAllocate(newSize);

			Allocator::set(&myBeginPtr[myDataCount], (TYPE&&)data);
			myNextPtr++;
		}

		/* PRIVATE
		 * Call the destructor of a given destructor.
		 *
		 * @param data: Data to call the destructor.
		 */
		inline void _singleDestruct(RPTR data)
		{
			data->~TYPE();
		}

		/* PRIVATE VARIABLES AND CONSTANTS */
	private:
		PTR myBeginPtr;
		PTR myEndPtr;
		PTR myNextPtr;

		UI32 myDataCount = 0;		// number of elements stored
		UI32 myAllocationSize = 0;	// allocation size
	};
}
#endif // !_DATA_TYPES_ARRAY_H
