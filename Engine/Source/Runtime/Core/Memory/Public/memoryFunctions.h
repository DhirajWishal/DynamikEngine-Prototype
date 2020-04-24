#pragma once
#ifndef _DYNAMIK_MEMORY_FUNCTIONS_H
#define _DYNAMIK_MEMORY_FUNCTIONS_H

#include "Public/Pointer.h"

namespace Dynamik {
	/* FUNCTION
	 * Move a data from one location to another.
	 *
	 * @param destination: Where to place the data.
	 * @param source: Where to move the data from.
	 * @param size: number of bytes to move.
	 */
	template<class TYPE>
	void moveBytes(POINTER<TYPE> destination, const POINTER<TYPE> source, UI32 size = 1)
	{
		UI32 _itrCount = size * source.getTypeSize();
		POINTER<BYTE> _sourceIterator = (POINTER<TYPE>)source;
		POINTER<BYTE> _destinationIterator = destination;
		while (_itrCount--) {
			_destinationIterator.dereference() = _sourceIterator.dereference();
			_destinationIterator++, _sourceIterator++;
		}
	}

	/* FUNCTION
	 * Move data from one location to another.
	 *
	 * @param destination: Where to place the data.
	 * @param first: First pointer to data.
	 * @param last: Last pointer to data.
	 */
	template<class TYPE>
	void moveBytes(POINTER<TYPE> destination, const POINTER<TYPE> first, const POINTER<TYPE> last)
	{
		POINTER<BYTE> _destinationIterator = destination;
		POINTER<BYTE> _itr = (POINTER<TYPE>) first;
		while (_itr.getPointerAsInteger() < last.getPointerAsInteger())
		{
			_destinationIterator.dereference() = _itr.dereference();
			_itr++, _destinationIterator++;
		}
	}

	/* FUNCTION
	 * Fill a given address with a given data value.
	 *
	 * @param address: Where to place the data.
	 * @param value: Value to be filled with
	 * @param size: number of bytes to fill.
	 */
	template<class TYPE>
	void setData(POINTER<TYPE> address, I32 value, UI32 size = 0, const UI32 typeSize = 1)
	{
		POINTER<UI8> _iterator = address;
		UI32 _runSize = sizeof(POINTER<TYPE>);
		if (size > 1)
			UI32 _runSize = size - 1;
		do {
			_iterator.set((UI8)value);
			_iterator++;
		} while (--_runSize);
		_iterator--;
	}

	/* FUNCTION
	 * Prepare a new pointer to store data.
	 * This allocates the pointer in heap and is not automatically deleted.
	 *
	 * @param size: Size of the pointer to be prepared.
	 */
	template<class TYPE>
	POINTER<TYPE> preparePointer(I32 size = sizeof(TYPE))
	{
		POINTER<TYPE> _ptr = StaticAllocator<TYPE>::allocate(size);
		StaticAllocator<TYPE>::set(_ptr, TYPE());
		return _ptr;
	}
}

#endif // !_DYNAMIK_MEMORY_FUNCTIONS_H
