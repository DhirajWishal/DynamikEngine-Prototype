#pragma once
#ifndef _DYNAMIK_MANAGERS_MEMORY_FUNCTIONS_H
#define _DYNAMIK_MANAGERS_MEMORY_FUNCTIONS_H

#include "core/types/Pointer.h"

namespace Dynamik {
	/* FUNCTION
	 * Move a data from one location to another.
	 *
	 * @param destination: Where to place the data.
	 * @param source: Where to move the data from.
	 * @param size: number of bytes to move.
	 */
	template<class SUB_TYPE>
	void moveBytes(POINTER<SUB_TYPE> destination, const POINTER<SUB_TYPE> source, UI32 size = 0)
	{
		POINTER<SUB_TYPE> _sourceIterator = (POINTER<SUB_TYPE>)source;
		POINTER<SUB_TYPE> _destinationIterator = destination;
		while (size--) {
			*_destinationIterator = *_sourceIterator;
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
	void setData(POINTER<SUB_TYPE> address, const SUB_TYPE value, UI32 size = 0, const UI32 typeSize = 1)
	{
		POINTER<SUB_TYPE> _iterator = address;
		UI32 _runSize = size / typeSize;
		do {
			_iterator = value;
			_iterator++;
		} while (--_runSize);
		_iterator--;
	}
}

#endif // !_DYNAMIK_MANAGERS_MEMORY_FUNCTIONS_H
