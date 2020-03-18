#pragma once
#ifndef _DYNAMIK_MEMORY_FUNCTIONS_H
#define _DYNAMIK_MEMORY_FUNCTIONS_H

#include "DataTypesLib/Public/Pointer.h"

namespace Dynamik {
	/* FUNCTION
	 * Move a data from one location to another.
	 *
	 * @param destination: Where to place the data.
	 * @param source: Where to move the data from.
	 * @param size: number of bytes to move.
	 */
	template<class SUB_TYPE>
	void moveBytes(POINTER<SUB_TYPE> destination, const POINTER<SUB_TYPE> source, UI32 size = 1)
	{
		UI32 _itrCount = size * source.getTypeSize();
		POINTER<BYTE> _sourceIterator = (POINTER<SUB_TYPE>)source;
		POINTER<BYTE> _destinationIterator = destination;
		while (_itrCount--) {
			_destinationIterator.dereference() = _sourceIterator.dereference();
			_destinationIterator++, _sourceIterator++;
		}
	}

	/* FUNCTION
	 * Fill a given address with a given data value.
	 *
	 * @param address: Where to place the data.
	 * @param value: Value to be filled with
	 * @param size: number of bytes to fill.
	 */
	template<class SUB_TYPE>
	void setData(POINTER<SUB_TYPE> address, I32 value, UI32 size = 0, const UI32 typeSize = 1)
	{
		POINTER<UI8> _iterator = address;
		UI32 _runSize = sizeof(POINTER<SUB_TYPE>);
		if (size > 1)
			UI32 _runSize = size - 1;
		do {
			_iterator.set((UI8)value);
			_iterator++;
		} while (--_runSize);
		_iterator--;
	}
}

#endif // !_DYNAMIK_MEMORY_FUNCTIONS_H
