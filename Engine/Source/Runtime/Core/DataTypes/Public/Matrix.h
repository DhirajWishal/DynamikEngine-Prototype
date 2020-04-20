#pragma once
#ifndef _DYNAMIK_DATA_TYPES_MATRIX_H
#define _DYNAMIK_DATA_TYPES_MATRIX_H

#include "Array.h"

namespace Dynamik {
	/* TEMPLATED
	 * Matrix class for the Dynamik Engine.
	 *
	 * @template param TYPE: Type of the data to be stored.
	 * @template param RowCount: Number of rows in the matrix.
	 * @template param ColumnCount: Number of columns in the matrix.
	 */
	template<class TYPE, UI32 RowCount = 1, UI32 ColumnCount = 1>
	class MATRIX {
	public:
		MATRIX() {}
		MATRIX(const UI32& row, const UI32& column)
		{
			if (myRowCount <= row || myColumnCount <= column)
				myRowCount = row, myColumnCount = column;

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
		UI32 myRowCount = RowCount;
		UI32 myColumnCount = ColumnCount;
	};
}
#endif // !_DATA_TYPES_MATRIX_H
