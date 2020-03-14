#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_MATRIX_H
#define _DYNAMIK_CENTRAL_DATA_HUB_MATRIX_H

#include "Array.h"
#include <vector>

namespace Dynamik {
	/* TEMPLATED
	 * Matrix class for the Dynamik Engine.
	 */
	template<class TYPE>
	class MATRIX {
	public:
		MATRIX(const UI32& row, const UI32 column) 
			: myRowCount(row), myColumnCount(column)
		{
			myMatrix.setSizeAndValue(myRowCount, ARRAY<TYPE>(myColumnCount, 0));
		}
		~MATRIX() {}

	public:
		void set(ARRAY<ARRAY<TYPE>> matrix)
		{
			if ((matrix.size() != myRowCount) || (matrix[0].size() != myColumnCount))
				return;

			for (UI32 row = 0; row < myRowCount; row++)
				for (UI32 col = 0; col < myColumnCount; col++)
					myMatrix[row][col] = matrix[col][row];
		}

	private:
		ARRAY<ARRAY<TYPE>> myMatrix;
		UI32 myRowCount = 0;
		UI32 myColumnCount = 0;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_MATRIX_H
