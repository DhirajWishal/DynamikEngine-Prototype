#pragma once
#ifndef _DYNAMIK_ALGORITHM_SWAP_H
#define _DYNAMIK_ALGORITHM_SWAP_H

namespace Dynamik {
	/* FUNCTION
	 * Swap two given variables.
	 *
	 * @param first: First variable.
	 * @param second: Second variable.
	 */
	template<class TYPE>
	void swap(TYPE& first, TYPE& second)
	{
		if (first == second)
			return;

		TYPE _temp = first;
		first = second;
		second = _temp;
	}
}

#endif // !_DYNAMIK_ALGORITHM_SWAP_H
