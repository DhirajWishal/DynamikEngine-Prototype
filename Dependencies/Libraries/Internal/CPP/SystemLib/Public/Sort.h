#pragma once
#ifndef _DYNAMIK_SYSTEM_SORT_H
#define _DYNAMIK_SYSTEM_SORT_H

#include "DataTypesLib/Public/datatypes.h"
#include "DataTypesLib/Public/Array.h"

namespace Dynamik {
	/* TEMPLATED FUNCTION
	 * Bubble sort for the array.
	 * Sort the Array in ascending or descending order.
	 *
	 * @param arr: The array to be sorted.
	 * @param isAsc = true: Sorting type (true = ascending, false = descending)
	 */
	template<class TYPE>
	ARRAY<TYPE>& bubbleSort(ARRAY<TYPE> arr, const bool isAsc = true)
	{
		UI32 _indexCount = 0;
		UI32 _size = arr.size();
		UI32 _index, _itr;
		while (_indexCount < (_size - 1))
		{
			_index = 0;
			_itr = 1;
			while (_itr < (_size - _indexCount))
			{
				if (isAsc)
				{
					if (arr.at(_index) > arr.at(_itr))
					{
						TYPE _temp = arr.at(_index);
						arr.update(arr.at(_itr), _index);
						arr.update(_temp, _itr);
					}
				}
				else
				{
					if (arr.at(_index) < arr.at(_itr))
					{
						TYPE _temp = arr.at(_index);
						arr.update(arr.at(_itr), _index);
						arr.update(_temp, _itr);
					}
				}
				_index++;
				_itr++;
			}
			_indexCount++;
		}

		return arr;
	}
}

#endif // !_DYNAMIK_SYSTEM_SORT_H
