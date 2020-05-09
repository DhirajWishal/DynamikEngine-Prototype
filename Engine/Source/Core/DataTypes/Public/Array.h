#pragma once
#ifndef _DYNAMIK_DATA_TYPES_ARRAY_H
#define _DYNAMIK_DATA_TYPES_ARRAY_H

/*
 Dynamic Array for the Dynamik Engine.

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		22/03/2020
 IDE:		MS Visual Studio 2019
*/

#include "datatypes.h"
#include "Pointer.h"
#include "Public/StaticAllocator.h"
#include "InitializerList.h"
#include "Thread.h"

#include <vector>	/* Required when converting to std::vector<TYPE> */

#include <future>	/* REMOVE AFTER ADDING THE THREAD MANAGER */
#include <functional>	/* Required for Lambdas */

namespace Dynamik {
	template<class TYPE>
	class StaticAllocator;	// Static Allocator declaration
	template<class TYPE>
	class POINTER;			// Pointer declaration
	template<class TYPE>
	class InitializerList;	// Initializer List declaration
	class Thread;			// Thread declaration

	/* PUBLIC ENUM
	 * Dynamik Array Destructor Call Modes.
	 * This either allows the destructor to call all the destructors of stored variables or orphan them all.
	 */
	enum class DMKArrayDestructorCallMode {
		DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_NONE,	// Does not call the destructor in data. Execution speed is high.
		DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_ALL,	// Calls destructor in all elements. Execution speed depends on the time taken to destroy.
		DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_ALL_THREADED,	// Calls the destructor for each element in a thread.
		DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_UNDEFINED	// Undefined
	};

	/* TEMPLATED
	 * Dynamic Array data structure for the Dynamik Engine.
	 * This array can store any data defined in the data type TYPE and supports multiple dimensions.
	 * Tested to be faster than the ARRAY<TYPE> library/ data type.
	 * This also contains utility functions related to array and pointer manipulation.
	 *
	 * This can also be used as:
	 *     Dynamic Array
	 *     Stack
	 *     Queue
	 *     Deque (Dequeue)
	 *
	 * @warn: The Dynamic Array does not call the destructor for all the stored elements at default.
				If needed to call, DestructorCallMode must be set to either ALL or ALL THREADED.
	 */
	template<class TYPE, DMKArrayDestructorCallMode DestructorCallMode = DMKArrayDestructorCallMode::DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_NONE, class Allocator = StaticAllocator<TYPE>>
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
		 * Used as a private data type.
		 */
		using PTR = POINTER<TYPE>;

		/* PRIVATE DATATYPE
		 * Used as a raw pointer.
		 */
		using RPTR = TYPE*;

		/* HELPER STRUCTURE
		 * Store pointer data temporarily.
		 */
		struct _pointerContainer {
			_pointerContainer() {}	// default constructor
			~_pointerContainer() {}	// default destructor

			PTR _beginPtr;	// begin pointer
			PTR _endPtr;	// end pointer
			PTR _nextPtr;	// next pointer
		};

		/* HELPER CLASS
		 * BASE: Thread
		 * Thread class to call the destructor for all the elements stored within.
		 * This is handled by the thread manager.
		 */
		class _internalThread : public Thread {
		public:
			_internalThread() {} // default constructor
			_internalThread(PTR _first, PTR _last) : _firstPtr(_first), _lastPtr(_last) {}	// default constructor
			~_internalThread() {} // default destructor

			void _destroy() {	// main _destroy function
				while (_firstPtr != _lastPtr)	// call the destructor for each address
				{
					_firstPtr.dereference().~TYPE();
					_firstPtr++;
				}
			}

		private:
			PTR _firstPtr;	// first pointer
			PTR _lastPtr;	// last pointer
		};

	public:
		/* CONSTRUCTOR
		 * Default Constructor.
		 */
		ARRAY()
		{
			_setLocalDataToDefault();
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
			if (size)
			{
				if ((size + _getAllocatableSize(size)) > maxSize()) return; /* TODO: Error Flagging */

				_reAllocateBack(_getAllocatableSize(size));
			}
			else
				_reAllocateBack(_getNextSize());

			myDataCount = size;
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
			if (size)
			{
				UI32 _allocatableSize = _getAllocatableSize(size);
				if ((size + _allocatableSize) > maxSize()) return; /* TODO: Error Flagging */

				_reAllocateBack(_allocatableSize);
				_fillWithData(size, TYPE());
			}
			else
			{
				_reAllocateBack(_getNextSize());
				_fillWithData(capacity(), TYPE());
			}

			myDataCount = size;
			myNextPtr += size;
		}

		/* CONSTRUCTOR
		 * Constructs the Array by initializing the data in another array.
		 *
		 * @param arr: Array used to initialize this.
		 */
		ARRAY(const POINTER<TYPE> arr)
		{
			if (_getAllocationSize() > maxSize()) return; /* TODO: Error Flagging */

			myDataCount = _getSizeOfRawArray(arr);
			_reAllocateBack(_getNextSizeToFit(myDataCount));
			setData(myBeginPtr, 0, capacity());
		}

		/* CONSTRUCTOR
		 * Constructs the Array by initializing the data in an initializer list.
		 *
		 * @param list: Initializer list used to initialize this.
		 */
		ARRAY(InitializerList<TYPE> list, UI32 size = 1)
		{
			if (list.size() > maxSize()); /* TODO: Error Flagging */

			if (list.size() <= 1)
			{
				if ((list.size() + _getNextSizeToFit(_getAllocatableSize(list.size()))) > maxSize()) return; /* TODO: Error Flagging */

				if (list.size())
					_reAllocateBack(_getNextSizeToFit(_getAllocatableSize(list.size())));
				else
					_reAllocateBack(_getNextSize());

				if (list.size())
					moveBytes(myBeginPtr, (PTR)list.begin(), (PTR)list.end());
			}
			else
			{
				_reAllocateBack(_getNextSizeToFit(_getAllocatableSize(list.size())));
				moveBytes(myBeginPtr, (PTR)list.begin(), (PTR)list.end());
			}

			myDataCount = list.size();
			myNextPtr += myDataCount;
		}

		/* CONSTRUCTOR
		 * Constructs the Array by initializing the data in an initializer list.
		 *
		 * @param list: Initializer list used to initialize this.
		 */
		ARRAY(std::initializer_list<TYPE> list, UI32 size = 1)
		{
			if (list.size() > maxSize()); /* TODO: Error Flagging */

			if (list.size() <= 1)
			{
				if ((list.size() + _getNextSizeToFit(_getAllocatableSize(list.size()))) > maxSize()) return; /* TODO: Error Flagging */

				if (list.size())
					_reAllocateBack(_getNextSizeToFit(_getAllocatableSize(list.size())));
				else
					_reAllocateBack(_getNextSize());

				if (list.size())
					moveBytes(myBeginPtr, (PTR)list.begin(), (PTR)list.end());
			}
			else
			{
				_reAllocateBack(_getNextSizeToFit(_getAllocatableSize(list.size())));
				moveBytes(myBeginPtr, (PTR)list.begin(), (PTR)list.end());
			}

			myDataCount = list.size();
			myNextPtr += myDataCount;
		}

		/* CONSTRUCTOR
		 * Constructs the Array by using std::vector<TYPE>.
		 *
		 * @param vector: Other vector.
		 */
		ARRAY(std::vector<TYPE> vector)
		{
			if (vector.size() > maxSize()); /* TODO: Error Flagging */

			_reAllocateBack(_getAllocatableSize(vector.size()));
			moveBytes(myBeginPtr, (PTR)vector.begin()._Unwrapped(), (PTR)vector.end()._Unwrapped());

			myDataCount = vector.size();
			myNextPtr += myDataCount;
		}

		ARRAY(const ARRAY<TYPE>& arr)
		{
			if (arr.size())
			{
				_reAllocateBack(_getAllocatableSize(arr.capacity()));

				set(arr.begin(), arr.end());
			}
			else
				_reAllocateBack(_getNextSize());
		}

		/* DESTRUCTOR
		 * Default Destructor.
		 */
		~ARRAY()
		{
			_terminate();
		}

		/* PRIVATE DATA TYPES */
	public:
		/* DATATYPE
		 * Iterator of the Array.
		 */
		typedef PTR ITERATOR;
		typedef ITERATOR iterator;	// for vector class compatibility

		/* PRIVATE FUNCTIONS */
	public:
		/* FUNCTION
		 * Set the Array to a pre defined one.
		 *
		 * @param arr: Array to assign this with.
		 */
		void set(const PTR arr)
		{
			if (_getAllocationSize() > maxSize()) return; /* TODO: Error Flagging */

			if (myBeginPtr.isValid())
				Allocator::deAllocate(myBeginPtr, myEndPtr);

			myDataCount = _getSizeOfRawArray(arr);
			_reAllocateBack(_getNextSizeToFit(myDataCount));
			moveBytes(myBeginPtr, arr, _getAllocationSize());
		}

		/* FUNCTION
		 * Set a value to a given index.
		 *
		 * @param index: Index where to set the value.
		 * @param value: Value to be updated.
		 */
		void set(UI32 index, const TYPE& value)
		{
			if (!isValidIndex(index)) return; /* TODO: Error Flagging */

			myNextPtr = &myBeginPtr[_getProcessedIndex(index)];
			myNextPtr = (TYPE)value;
			myNextPtr++;
			myDataCount++;
		}

		/* FUNCTION
		 * Set the values from an iterator to this.
		 *
		 * @param first: First iterator.
		 * @param last: Last iterator.
		 */
		void set(ITERATOR first, ITERATOR last)
		{
			UI32 _byteSize = last.getPointerAsInteger() - first.getPointerAsInteger();
			if (_byteSize)
			{
				myDataCount = (_byteSize) / typeSize();
				if (_byteSize > _getAllocationSize())
					_reAllocateAssign(_getAllocatableSize(_byteSize));

				moveBytes(myBeginPtr, first, last);
				myNextPtr += myDataCount;
			}
			else
			{
				if (!myBeginPtr.isValid())
					_reAllocateBack(_getNextSize());
			}
		}

		/* FUNCTION
		 * Set values to this using an initializer list.
		 *
		 * @param list: List to be used to initialize this.
		 */
		void set(InitializerList<TYPE> list)
		{
			if (list.size() > maxSize()); /* TODO: Error Flagging */

			if (list.size() > capacity())
				_reAllocateAssign(_getAllocatableSize(list.size()));

			moveBytes(myBeginPtr, list.begin(), list.end());
			myDataCount = list.size();
			myNextPtr += myDataCount;
		}

		/* FUNCTION
		 * Set values to this using an initializer list.
		 *
		 * @param list: List to be used to initialize this.
		 */
		void set(std::initializer_list<TYPE> list)
		{
			if (list.size() > maxSize()); /* TODO: Error Flagging */

			if (list.size() > capacity())
				_reAllocateAssign(_getAllocatableSize(list.size()));

			moveBytes(myBeginPtr, (PTR)list.begin(), (PTR)list.end());
			myDataCount = list.size();
			myNextPtr += myDataCount;
		}

		/* CONSTRUCTOR
		 * Constructs the Array by using another Array.
		 *
		 * @param array: The other array.
		 */
		void set(const ARRAY<TYPE>& array)
		{
			set(array.begin(), array.end());
		}

		/* FUNCTION
		 * Push Elements to the end of the Array.
		 *
		 * @param data: Data to be added to the Array (lvalue).
		 */
		void pushBack(const TYPE& data)
		{
			if (myDataCount > (capacity() * 2))
				myDataCount = 0;

			if (myNextPtr.getPointerAsInteger() >= myEndPtr.getPointerAsInteger())
				_reAllocateAndPushBack(_getNextSize(), data);
			else
				_addDataBack(data);

			myDataCount++;
		}

		/* FUNCTION
		 * Push Elements to the end of the Array.
		 *
		 * @param data: Data to be added to the Array (rvalue).
		 */
		void pushBack(TYPE&& data)
		{
			pushBack((TYPE&)data);
		}

		/* FUNCTION
		 * Push Elements to the end of the Array.
		 * Compatibility with the 'ARRAY<TYPE>::push_back(const TYPE&)' function.
		 *
		 * @param data: Data to be added to the Array (lvalue).
		 */
		void push_back(const TYPE& value)
		{
			pushBack(value);
		}

		/* FUNCTION
		 * Push Elements to the end of the Array.
		 * Compatibility with the 'ARRAY<TYPE>::push_back(const TYPE&)' function.
		 *
		 * @param data: Data to be added to the Array (rvalue).
		 */
		void push_back(TYPE&& value)
		{
			pushBack(value);
		}

		/* FUNCTION
		 * Get the last element in the Array.
		 */
		TYPE back()
		{
			return myBeginPtr[myDataCount - 1];
		}

		/* FUNCTION
		 * Get the last element in the Array and remove it.
		 */
		TYPE popBack()
		{
			TYPE _data = myBeginPtr[myDataCount - 1];
			myNextPtr--;
			myDataCount--;
			return _data;
		}

		/* FUNCTION
		 * Push Elements to the beginning of the Array.
		 *
		 * @param data: Data to be added to the Array (lvalue).
		 */
		void pushFront(const TYPE& data)
		{
			if (myDataCount > (capacity() * 2))
				myDataCount = 0;

			if (myNextPtr.getPointerAsInteger() >= myEndPtr.getPointerAsInteger())
				_reAllocateAndPushFront(_getNextSize(), data);
			else
				_addDataBack(data);

			myDataCount++;
		}

		/* FUNCTION
		 * Push Elements to the beginning of the Array.
		 *
		 * @param data: Data to be added to the Array (rvalue).
		 */
		void pushFront(TYPE&& data)
		{
			pushFront((TYPE&)data);
		}

		/* FUNCTION
		 * Get the first element of the Array.
		 */
		TYPE front()
		{
			if (myBeginPtr.isValid())
				return myBeginPtr[0];

			return TYPE();
		}

		/* FUNCTION
		 * Get the first element in the Array and remove it.
		 */
		TYPE popFront()
		{
			return _popFrontReallocate();
		}

		/* FUNCTION
		 * Remove an element at a given index.
		 *
		 * @param index: The element index to be removed.
		 */
		void remove(UI32 index = 0)
		{
			if (!isValidIndex(index)) return; /* TODO: Error Flagging. */

			myBeginPtr[_getProcessedIndex(index)] = (TYPE)0;
			_compactAfterRemove(index);
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
				Allocator::deAllocate(myBeginPtr, _getAllocationSize());

			_reAllocateBack(_getAllocatableSize(size));
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
			if (index >= _getSizeOfThis() || (index <= (0 - _getSizeOfThis()))); // TODO: error handling

			UI32 _processedIndex = _getProcessedIndex(index);

			return myBeginPtr[_processedIndex];
		}

		/* FUNCTION
		 * Clear the whole Array.
		 */
		void clear()
		{
			_terminate();
			_setLocalDataToDefault();
		}

		/* FUNCTION
		 * Resize the Array.
		 *
		 * @param size: Size to which the Array should be resized.
		 */
		void resize(UI32 size)
		{
			if (size)
			{
				if (size > maxSize()) return; /* TODO: Error Flagging */

				if (myBeginPtr.getPointerAsInteger() != myEndPtr.getPointerAsInteger())
					Allocator::deAllocate(myBeginPtr.get(), _getAllocationSize());

				_reAllocateAssign(_getAllocatableSize(size));
				_fillWithData(capacity(), TYPE());

				myDataCount = size;
			}
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

			if (myBeginPtr.getPointerAsInteger() != myEndPtr.getPointerAsInteger())
				Allocator::deAllocate(myBeginPtr.get(), _getAllocationSize());

			_reAllocateAssign(_getAllocatableSize(size));
			_fillWithData(capacity(), (TYPE&&)value);

			_setValue(value, capacity());
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
			if ((endIndex >= _getSizeOfThis()) || (startIndex < endIndex)); // TODO:: error handling

			while (startIndex <= endIndex)
			{
				*_getPointer(startIndex) = value;
				startIndex++;
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
			if (index >= _getSizeOfThis()); // TODO: error handling

			*_getPointer(index) = value;
		}

		/* FUNCTION
		 * Insert the content of another Array to the current Array.
		 *
		 * @param arr: Array with the same type to be contacted.
		 */
		void insert(ARRAY<TYPE> arr)
		{
			if (arr.size() > capacity())
				_reAllocateAssign(_getAllocatableSize(arr.size()));

			moveBytes(myBeginPtr, arr.begin(), arr.end());
		}

		/* FUNCTION
		 * Insert the content of another Array to the current Array.
		 *
		 * @param first: Starting iterator.
		 * @param last: End iterator.
		 */
		ITERATOR insert(ITERATOR first, ITERATOR last)
		{
			if ((UI64)(last - first) > _getAllocationSize())
				_reAllocateAssign(last.getPointerAsInteger() - first.getPointerAsInteger());

			moveBytes(myBeginPtr, first, last);
			UI32 _size = (UI64)(last - first);
			myDataCount += _size;
			myNextPtr += _size;
			return myNextPtr;
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
			if (index >= _getSizeOfThis()); // TODO: error handling

			myBeginPtr[index] = value;
		}

		/* FUNCTION
		 * Check if the given index is valid.
		 *
		 * @param index: Index to be checked.
		 */
		B1 isValidIndex(I32 index) {
			if (index > 0)
			{
				if (index < _getAllocationSize())
					return true;
			}
			else
			{
				index *= (-1);
				if (index > (_getAllocationSize() * (-1)))
					return true;
			}

			return false;
		}

		/* FUNCTION
		 * Check if a given index range is valid.
		 *
		 * @param first: First index.
		 * @param last: Last index.
		 */
		B1 isValidIndexRange(I32 first, I32 last)
		{
			return isValidIndex(first) && isValidIndex(last);
		}

		/* FUNCTION
		 * Bubble sort for the array.
		 * Sort the Array in ascending or descending order.
		 *
		 * @param isAsc = true: Sorting type (true = ascending, false = descending)
		 */
		void bubbleSort(bool isAsc = true)
		{
			ARRAY<TYPE> _localArray = this;
			UI32 _indexCount = 0;
			UI32 _index, _itr;
			while (_indexCount < (_getSizeOfThis() - 1))
			{
				_index = 0;
				_itr = 1;
				while (_itr < _getSizeOfThis() - _indexCount)
				{
					if (isAsc)
					{
						if (_localArray.at(_index) > _localArray.at(_itr))
						{
							TYPE _temp = _localArray.at(_index);
							_localArray.update(_localArray.at(_itr), _index);
							_localArray.update(_temp, _itr);
						}
					}
					else
					{
						if (_localArray.at(_index) < _localArray.at(_itr))
						{
							TYPE _temp = _localArray.at(_index);
							_localArray.update(_localArray.at(_itr), _index);
							_localArray.update(_temp, _itr);
						}
					}
					_index++;
					_itr++;
				}
				_indexCount++;
			}
		}

		/* FUNCTION
		 * Get a sub Array from this.
		 *
		 * @param from: First index.
		 * @param toWhere: Last index.
		 */
		ARRAY<TYPE> subArray(UI32 from, UI32 toWhere)
		{
			if (!isValidIndexRange(from, toWhere)) return; /* TODO: Error Flagging */

			ARRAY<TYPE> _local(toWhere - from);

			for (; from <= toWhere; from++)
				_local.pushBack(at(from));

			return _local;
		}

		/* FUNCTION
		 * Get a sub Array from this.
		 *
		 * @param from: First index.
		 * @param toWhere: Last index.
		 */
		ARRAY<TYPE> subArray(SI32 from, SI32 toWhere)
		{
			if (!isValidIndexRange(from, toWhere)) return; /* TODO: Error Flagging */

			ARRAY<TYPE> _local((from - toWhere) * -1);

			for (; from <= toWhere; from++)
				_local.pushBack(at(from));

			return _local;
		}

		/* FUNCTION
		 * Convert this to a std::vector<TYPE>.
		 */
		std::vector<TYPE> toVector()
		{
			std::vector<TYPE> _localVector;
			for (ITERATOR _itr = begin(); _itr != end(); _itr++)
				_localVector.push_back(*_itr);

			return _localVector;
		}

		/* FUNCTION
		 * Return the size of the Array.
		 *
		 * @return: Element count.
		 */
		const UI32 size() const noexcept { return _getSizeOfThis(); }

		/* FUNCTION
		 * Return the maximum size allocatable in an Array.
		 *
		 * @return: Element count.
		 */
		const UI32 maxSize() const noexcept { return ((UI32)-1) / sizeof(TYPE); }

		/* FUNCTION
		 * Return the number of elements that can be stored before the next allocation.
		 *
		 * @return: Element count.
		 */
		const UI32 capacity() const noexcept
		{
			if (_getAllocationSize() == 0 || sizeof(TYPE) == 0)
				return 0;

			return _getAllocationSize() / sizeof(TYPE);
		}

		/* FUNCTION
		 * Return the size of TYPE.
		 */
		const UI32 typeSize() const noexcept { return sizeof(TYPE); }

		/* FUNCTION
		 * Return the current allocation size.
		 * This size is the size available till the next allocation.
		 *
		 * @return: Byte count.
		 */
		const UI32 allocationSize() const noexcept { return _getAllocationSize(); }

		/* FUNCTION
		 * Check if the Array is empty.
		 */
		const B1 empty() const noexcept { return (_getSizeOfThis()) != 0; }

		/* FUNCTION
		 * Get data pointer of the Array.
		 */
		const RPTR data() const noexcept { return myBeginPtr.get(); }

		/* FUNCTION
		 * Begin iterator of the Array.
		 */
		const ITERATOR begin() const noexcept { return myBeginPtr; }

		/* FUNCTION
		 * End iterator of the Array.
		 */
		const ITERATOR end() const noexcept
		{
			if (myNextPtr.getPointerAsInteger() > myBeginPtr.getPointerAsInteger())
				return myNextPtr;

			if (myDataCount)
				return (PTR)(myBeginPtr.get() + myDataCount);

			if (myDataCount == 0)
				return myBeginPtr;

			return myEndPtr;
		}

		/* FUNCTION
		 * Find a given value in this array.
		 *
		 * @param data: Data to be searched for.
		 * @return: Array of indexes which the gived data is found.
		 */
		ARRAY<UI32> find(const TYPE& data)
		{
			ARRAY<UI32> _indexContainer;

			for (UI32 _itr = 0; _itr < size(); _itr++)
				if (this->at(_itr) == data)
					_indexContainer.pushBack(_itr);

			return _indexContainer;
		}

		/* FUNCTION
		 * Find a given value in this array.
		 *
		 * @param data: Data to be searched for.
		 * @return: Array of indexes which the gived data is found.
		 */
		ARRAY<UI32> find(TYPE&& data)
		{
			ARRAY<UI32> _indexContainer;

			for (UI32 _itr = 0; _itr < size(); _itr++)
				if (this->at(_itr) == data)
					_indexContainer.pushBack(_itr);

			return _indexContainer;
		}

		/* FUNCTION
		 * Returns a hash for the content stored in the array.
		 */
		UI64 hash()
		{
			UI64 _hashInt = 0;
			ITERATOR begin = myBeginPtr;

			while (begin != end())
			{
				_hashInt = _hashInt ^ begin.getPointerAsInteger();
				begin++;
			}

			return _hashInt;
		}

		/* TEMPLATED FUNCTION
		 * Casts the data stored in this array to the provided type.
		 */
		template<class SUB_TYPE>
		ARRAY<SUB_TYPE, DestructorCallMode> cast()
		{
			ARRAY<SUB_TYPE> _newArr;

			for (auto _elem : *this)
				_newArr.pushBack(Cast<SUB_TYPE>(_elem));

			return _newArr;
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
		const TYPE& operator[](I32 index) const
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
			if (this->_getSizeOfThis() != data.size())
				return false;
			for (UI32 _itr = 0; _itr < this->_getSizeOfThis(); _itr++)
				if (this->at(_itr) != data.at(_itr))
					return false;

			return true;
		}

		/* OPERATOR
		 * + operator overload.
		 * Concat another Array with the similar type.
		 *
		 * @param data: Array to be contacted with.
		 */
		ARRAY<TYPE>& operator+(const ARRAY<TYPE>& data)
		{
			this->insert(data);
			return *this;
		}

		/* OPERATOR
		 * = operator overload.
		 * Get data from an array and initialize it to this.
		 *
		 * @para arr: Array to be initialized to this.
		 */
		ARRAY<TYPE>& operator=(const ARRAY<TYPE>& arr)
		{
			this->set(arr.begin(), arr.end());
			return *this;
		}

		/* OPERATOR
		 * = operator overload.
		 * Get data from an array and initialize it to this.
		 *
		 * @para arr: Array to be initialized to this.
		 */
		ARRAY<TYPE>& operator=(ARRAY<TYPE>&& arr) noexcept
		{
			this->set(arr.begin(), arr.end());
			return *this;
		}

		/* OPERATOR
		 * = operator overload.
		 * Get data from a raw array and initialize it to this.
		 *
		 * @para arr: Raw array to be initialized to this.
		 */
		ARRAY<TYPE>& operator=(const PTR arr)
		{
			this->set(arr);
			return *this;
		}

		/* OPERATOR
		 * = operator overload.
		 * Get data from a initializer list and initialize it to this.
		 *
		 * @para list: Raw array to be initialized to this.
		 */
		ARRAY<TYPE>& operator=(std::initializer_list<TYPE> list)
		{
			this->set(list);
			return *this;
		}

		/* OPERATOR
		 * = operator overload.
		 * Get data from an initializer list and initialize it to this.
		 *
		 * @para arr: Initializer list to be initialized to this.
		 */
		ARRAY<TYPE>& operator=(InitializerList<TYPE> list)
		{
			this->_initializeInitializerList(list);
			return *this;
		}

		/* STATIC FUNCTION
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

		/* ARRAY MANIPULATION FUNCTIONS */
	private:
		/* PRIVATE FUNCTION
		 * Initialize this with an initializer list.
		 *
		 * @param list: Initializer list to initialize this.
		 */
		inline void _initializeInitializerList(const InitializerList<TYPE>& list)
		{
			if (list.size() > maxSize()); /* TODO: Error Flagging */

			_reAllocateBack(list.size());
			for (InitializerList<TYPE>::ITERATOR _itr = list.begin(); _itr != list.end(); ++_itr)
				pushBack(*_itr);
		}

		/* PRIVATE FUNCTION
		 * Add data to the end of the Array.
		 *
		 * @param data: Data to be emplaced.
		 */
		inline void _addDataBack(const TYPE& data)
		{
			Allocator::set(myNextPtr, (TYPE&&)data);
			myNextPtr++;
		}

		/* PRIVATE FUNCTION
		 * Add data to the end of the Array.
		 *
		 * @param data: Data to be emplaced.
		 */
		inline void _addDataFront(const TYPE& data)
		{
			myBeginPtr--;
			Allocator::set(myBeginPtr, (TYPE&&)data);
		}

		/* PRIVATE FUNCTION
		 * Allocate a fresh buffer with a given size.
		 *
		 * @param size: Size of the buffer to be allocated.
		 */
		inline PTR _allocateBuffer(UI32 size)
		{
			return Allocator::allocate(size);
		}

		/* PRIVATE FUNCTION
		 * Resize the data store and add the new space to the back.
		 *
		 * @param newSize: New size added to the pre-allocated size to be re-allocated.
		 */
		inline void _reAllocateBack(UI32 newSize)
		{
			PTR _newArr = Allocator::allocate(newSize + _getAllocatableSize(capacity()));

			try
			{
				if (myBeginPtr.isValid())
				{
					moveBytes(_newArr, myBeginPtr, myNextPtr);
					Allocator::deAllocate(myBeginPtr, myEndPtr);
				}
			}
			catch (...)
			{
				_destroyRange(_newArr, (TYPE*)(_newArr.getPointerAsInteger() + _getAllocationSize()));
				Allocator::deAllocate(_newArr, _getAllocationSize());

				_newArr.turnNull();

				throw;
			}

			_basicInitializationBack(_newArr, capacity() + _calculateCapacityInSize(newSize), _getSizeOfThis());
		}

		/* PRIVATE FUNCTION
		 * Resize the data store.
		 *
		 * @param newSize: New size added to the pre-allocated size to be re-allocated.
		 */
		inline void _reAllocateFront(UI32 newSize)
		{
			PTR _newArr = Allocator::allocate(newSize + _getAllocatableSize(capacity()));
			PTR _nxtPtr = _newArr;
			_nxtPtr += _calculateCapacityInSize(newSize);

			try
			{
				if (myBeginPtr.isValid())
				{
					moveBytes(_nxtPtr, myBeginPtr, myNextPtr);
					Allocator::deAllocate(myBeginPtr, myEndPtr);
				}
			}
			catch (...)
			{
				_destroyRange(_newArr, (TYPE*)(_newArr.getPointerAsInteger() + _getAllocationSize()));
				Allocator::deAllocate(_newArr, _getAllocationSize());

				_newArr.turnNull();

				throw;
			}

			_basicInitializationFront(_newArr, _calculateCapacityInSize(newSize), capacity() + _calculateCapacityInSize(newSize), _getSizeOfThis());
		}

		/* PRIVATE FUNCTION
		 * Re allocate memory and assign the size as the allocation size.
		 *
		 * @param size: The size to be allocated.
		 */
		inline void _reAllocateAssign(UI32 size)
		{
			PTR _newArr = Allocator::allocate(size + _getAllocatableSize(capacity()));

			try
			{
				if (myBeginPtr.isValid())
				{
					moveBytes(_newArr, myBeginPtr, myNextPtr);
					Allocator::deAllocate(myBeginPtr, myEndPtr);
				}
			}
			catch (...)
			{
				_destroyRange(_newArr, (TYPE*)(_newArr.getPointerAsInteger() + size));
				Allocator::deAllocate(_newArr, _getAllocationSize());

				_newArr.turnNull();

				throw;
			}

			_basicInitializationBack(_newArr, capacity() + _calculateCapacityInSize(size), _getSizeOfThis());
		}

		/* PRIVATE FUNCTION
		 * Create a new array and return it.
		 *
		 * @param newSize: New size added to the pre-allocated size to be re-allocated.
		 */
		inline _pointerContainer _reAllocateGetRaw(UI32 newSize)
		{
			_pointerContainer _container;
			_container._beginPtr = Allocator::allocate(newSize + _getAllocatableSize(capacity()));
			_container._endPtr = _container._beginPtr;
			_container._endPtr += (capacity() + _calculateCapacityInSize(newSize));
			_container._nextPtr = _container._beginPtr;
			_container._nextPtr += _getSizeOfThis();

			return _container;
		}

		/* PRIVATE FUNCTION
		 * Resize the data store and add a value to the end.
		 *
		 * @param newSize: New size added to the pre-allocated size to be re-allocated.
		 * @param data: Data to be inserted to the end.
		 */
		inline void _reAllocateAndPushBack(UI32 newSize, const TYPE& data)
		{
			if (_getSizeOfThis() > (capacity() * 2))
				newSize = _getNextSize();

			if ((newSize + _getAllocationSize()) > maxSize()) return; /* TODO: Error Flagging */
			_reAllocateBack(newSize);

			_addDataBack(data);
		}

		/* PRIVATE FUNCTION
		 * Resize the data store and add a value to the beginning.
		 *
		 * @param newSize: New size added to the pre-allocated size to be re-allocated.
		 * @param data: Data to be inserted to the end.
		 */
		inline void _reAllocateAndPushFront(UI32 newSize, const TYPE& data)
		{
			if (_getSizeOfThis() > (capacity() * 2))
				newSize = _getNextSize();

			if ((newSize + _getAllocationSize()) > maxSize()) return; /* TODO: Error Flagging */
			_reAllocateFront(newSize);

			_addDataFront(data);
		}

		/* PRIVATE FUNCTION
		 * Get the first value from the array and remove it.
		 * Then re allocate a new array to fit the size and move the old content to it.
		 */
		inline const TYPE& _popFrontReallocate()
		{
			if (!myBeginPtr.isValid())
				return TYPE();

			TYPE _data = front();
			setData((PTR)&myBeginPtr[0], 0, sizeof(TYPE));

			_pointerContainer _container = _reAllocateGetRaw(_getNextSize());

			PTR _localVec = &myBeginPtr[0];
			moveBytes(_localVec, (PTR)&myBeginPtr[0], _getSizeOfThis() - 1);

			try
			{
				if (_container._beginPtr.isValid())
				{
					moveBytes(_container._beginPtr, _localVec, _getSizeOfThis());
					_localVec.turnNull();
					Allocator::deAllocate(myBeginPtr, myEndPtr);
				}
			}
			catch (...)
			{
				_destroyRange(_container._beginPtr, _container._endPtr);
				Allocator::deAllocate(_container._beginPtr, _container._endPtr);

				_container._beginPtr.turnNull();
				_container._endPtr.turnNull();

				throw;
			}

			_basicInitializationBack(_container._beginPtr, _getAllocationSize(), _getSizeOfThis());
			return _data;
		}

		/* PRIVATE FUNCTION
		 * Remove the additional space after removal.
		 *
		 * @param removedIndex: Index where the data was removed.
		 */
		inline void _compactAfterRemove(UI32 removedIndex)
		{
			PTR _newArr = _allocateBuffer(_getAllocationSize());
			moveBytes(_newArr, (PTR)&myBeginPtr[removedIndex - 1], removedIndex);
			moveBytes((PTR)&_newArr[removedIndex], (PTR)&myBeginPtr[removedIndex + 1], myEndPtr);

			try
			{
				if (myBeginPtr.isValid())
					Allocator::deAllocate(myBeginPtr, myEndPtr);
			}
			catch (...)
			{
				_destroyRange(_newArr, (RPTR)(_newArr.getPointerAsInteger() + _getAllocationSize()));
				Allocator::deAllocate(_newArr, (RPTR)(_newArr.getPointerAsInteger() + _getAllocationSize()));

				_newArr.turnNull();

				throw;
			}

			_basicInitializationBack(_newArr, _getAllocationSize(), _getSizeOfThis());
		}

		/* POINTER MANIPULATION FUNCTIONS */
	private:
		/* PRIVATE FUNCTION
		 * Destroy all the data stored in a given range.
		 * Calls the destructor for all the data stored.
		 *
		 * @param first: Pointer to the first element.
		 * @param last: Pointer to the last element.
		 */
		inline void _destroyRange(PTR first, PTR last)
		{
			while (first.getPointerAsInteger() < last.getPointerAsInteger())
			{
				_singleDestruct(first);
				++first;
			}
		}

		/* PRIVATE STATIC
		 * Internal thread execution function.
		 *
		 * @param _thread: A n_internalThread instance.
		 */
		static void __internalThreadFunction(POINTER<_internalThread> _thread)
		{
			_thread->_destroy();
		}

		/* PRIVATE FUNCTION
		 * Destroy all the data stored in a given range.
		 * Calls the destructor for all the data stored giving a thread for each.
		 *
		 * Number of threads equal to the number of logical cores in the CPU - 1.
		 * Each element is assigned to a chunk which is calculated based on the thread count.
		 * Each chunk is filled with elements and passed in to the _internalThread class.
		 * The _internalThread class executes in a single thread and calls the destructor of each element.
		 * A lock guard is placed so that memory corruption will not take place
		 *
		 * @param first: Pointer to the first element.
		 * @param last: Pointer to the last element.
		 */
		inline void _destroyRangeInThreads(PTR first, PTR last)
		{
			UI32 _passes = std::thread::hardware_concurrency() - 1;	// number of threads
			UI32 _chunks = size() / std::thread::hardware_concurrency();	// number of elements thats given for each thread
			ARRAY<std::future<void>, DMKArrayDestructorCallMode::DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_ALL> _threads(_passes);		// threads
			PTR _first;
			_internalThread _localThread;

			while (--_passes)
			{
				_first = first;
				first += _chunks;

				_localThread = _internalThread(_first, first);
				//add each destructor array to threads
				_threads.pushBack(std::async(std::launch::async, __internalThreadFunction, &_localThread));
			}

			// if elements are not destroyed fully, destroy them manually (in this thread)
			while (first != last)
			{
				first.dereference().~TYPE();
				first++;
			}
		}

		/* PRIVATE FUNCTION
		 * Destroy all the pointers that contained the heap data.
		 * Calls the destructor.
		 */
		inline void _destroyPointers()
		{
			myBeginPtr.~POINTER();
			myEndPtr.~POINTER();
			myNextPtr.~POINTER();
		}

		/* PRIVATE FUNCTION
		 * Call the destructor of a given destructor.
		 *
		 * @param data: Data to call the destructor.
		 */
		inline void _singleDestruct(RPTR data)
		{
			data->~TYPE();
		}

		/* PRIVATE FUNCTION
		 * Terminate the data stored in the local pointers.
		 */
		inline void _terminate()
		{
			if (DestructorCallMode == DMKArrayDestructorCallMode::DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_ALL)
				_destroyRange(myBeginPtr, myNextPtr);
			else if (DestructorCallMode == DMKArrayDestructorCallMode::DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_ALL_THREADED)
				_destroyRangeInThreads(myBeginPtr, myNextPtr);

			Allocator::deAllocate(myBeginPtr, myEndPtr);
		}

		/* PRIVATE FUNCTION
		 * Clear all values and sets the local data to its default values.
		 */
		inline void _setLocalDataToDefault()
		{
			myBeginPtr.turnNull();
			myEndPtr = myBeginPtr;
			myNextPtr = myBeginPtr;
			myDataCount = 0;
		}

		/* UTILITY FUNCTIONS */
	private:
		/* PRIVATE
		 * Get the number of elements stored.
		 */
		inline const UI32 _getSizeOfThis() const
		{
			if (myDataCount > (capacity() * 2))
				return 0;

			return myDataCount;
		}

		/* PRIVATE
		 * Get the allocation size of this.
		 */
		inline const UI32 _getAllocationSize() const
		{
			return myEndPtr.getPointerAsInteger() - myBeginPtr.getPointerAsInteger();
		}

		/* PRIVATE
		 * Calculate the number of elements that can be stored in a size.
		 *
		 * @param bytes: Size of bytes.
		 */
		inline const UI32 _calculateCapacityInSize(UI32 bytes) const
		{
			if (bytes)
				return bytes / typeSize();

			return 0;
		}

		/* PRIVATE
		 * Calculate the allocatable size with raw size.
		 *
		 * @param rawSize: Size to be processed.
		 */
		inline const UI32 _getAllocatableSize(UI32 rawSize) const
		{
			return rawSize * typeSize();
		}

		/* PRIVATE FUNCTION
		 * Basic initializations.
		 * Set basic initializations for pushBack function.
		 *
		 * @param dataStore: Begin address of the new Array.
		 * @param capacity: Capacity of the new allocation.
		 * @param dataCount: Number of data currently stored in the new Array.
		 */
		void _basicInitializationBack(TYPE* dataStore, UI32 capacity = 0, UI32 dataCount = 0)
		{
			myBeginPtr(dataStore);

			myEndPtr(dataStore);
			myEndPtr += capacity;

			myNextPtr(dataStore);
			myNextPtr += dataCount;
		}

		/* PRIVATE FUNCTION
		 * Basic initializations.
		 * Set basic initializations for the pushFront function.
		 *
		 * @param dataStore: Begin address of the new Array.
		 * @param capacity: Capacity of the new allocation.
		 * @param dataCount: Number of data currently stored in the new Array.
		 */
		void _basicInitializationFront(TYPE* dataStore, UI32 freeCapacity = 0, UI32 capacity = 0, UI32 dataCount = 0)
		{
			myBeginPtr(dataStore);
			myBeginPtr += freeCapacity;

			myEndPtr(dataStore);
			myEndPtr += capacity;

			myNextPtr(dataStore);
			myNextPtr += dataCount;
		}

		/* PRIVATE FUNCTION
		 * Get the next allocatable size.
		 */
		inline UI32 _getNextSize()
		{
			UI32 _oldCapacity = capacity();

			if (_oldCapacity > (maxSize() - _oldCapacity / 2))
				return DMK_MEMORY_ALIGN * sizeof(TYPE);

			UI32 _nextCapacity = _oldCapacity + (_oldCapacity / 2);

			if (_nextCapacity < (DMK_MEMORY_ALIGN * sizeof(TYPE)))
				return DMK_MEMORY_ALIGN * sizeof(TYPE);

			return _nextCapacity;
		}

		/* PRIVATE FUNCTION
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

		/* PRIVATE FUNCTION
		 * Copy data from one location to another.
		 *
		 * @param source: Source address.
		 * @param destination: Destination address.
		 * @param size: Number of bytes to be copied.
		 */
		inline void _copyData(VPTR source, VPTR destination, UI32 size)
		{
			while (--size) *(TYPE*)source = *(TYPE*)destination;
		}

		/* PRIVATE FUNCTION
		 * Move data from this to another.
		 *
		 * @param newSpace: Source address.
		 * @param newSpaceSize: Number of bytes to be moved.
		 */
		inline void _moveToThis(VPTR newSpace, UI32 newSpaceSize)
		{
			moveBytes(myBeginPtr, (PTR)newSpace, newSpaceSize);
		}

		/* PRIVATE FUNCTION
		 * Move data from this to another.
		 *
		 * @param newSpace: Source address.
		 * @param newSpaceSize: Number of bytes to be moved.
		 */
		inline void _moveFromThis(VPTR newSpace, UI32 count)
		{
			moveBytes((PTR)newSpace, myBeginPtr, count);
		}

		/* PRIVATE FUNCTION
		 * Get the next pointer to data from a given index.
		 *
		 * @param index: Index of the pointer to retrieve.
		 */
		inline RPTR _getPointer(UI32 index = 0)
		{
			PTR _localPointer = myBeginPtr;
			_localPointer += index;
			return _localPointer;
		}

		/* PRIVATE FUNCTION
		 * Set a value to a number of elements.
		 *
		 * @param value: Value to be filled with.
		 * @param count: Number of elements to be filled with.
		 */
		inline void _setValue(TYPE value, UI32 count)
		{
			PTR _temp = myBeginPtr;
			while (_temp != myEndPtr)
			{
				_temp.set((TYPE&&)value);
				_temp++;
			}
		}

		/* PRIVATE FUNCTION
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

		/* PRIVATE FUNCTION
		 * Get the size of a raw Array.
		 *
		 * @param arr: Raw Array.
		 */
		inline UI32 _getSizeOfRawArray(const PTR arr)
		{
			return sizeof(arr.get()) / sizeof(TYPE);
		}

		/* PRIVATE FUNCTION
		 * Process the index so that it always produce a indexable value.
		 *
		 * @param index: Index to be processed.
		 */
		inline UI32 _getProcessedIndex(I32 index)
		{
			if (index < 0)
				index = (_getSizeOfThis() + index);

			return index;
		}

		/* PRIVATE FUNCTION
		 * Fills the allocated capacity with the constructor of the data type.
		 */
		inline void _fillWithData(UI32 capacity, TYPE&& data)
		{
			PTR _tempPtr = myBeginPtr;
			while (capacity--)
			{
				_tempPtr.set((TYPE&&)data);
				_tempPtr++;
			}
		}

		/* PRIVATE VARIABLES AND CONSTANTS */
	private:
		PTR myBeginPtr;		// begin pointer
		PTR myEndPtr = myBeginPtr;		// end pointer
		PTR myNextPtr = myBeginPtr;		// next pointer

		UI32 myDataCount = 0;	// data counter
	};
}

#endif // !_DATA_TYPES_ARRAY_H