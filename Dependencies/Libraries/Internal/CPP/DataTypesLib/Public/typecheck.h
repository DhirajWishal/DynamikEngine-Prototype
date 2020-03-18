#pragma once
#ifndef _DYNAMIK_DATA_TYPES_TYPE_CHECK_H
#define _DYNAMIK_DATA_TYPES_TYPE_CHECK_H

#include "datatypes.h"

namespace Dynamik {
	/* TEMPLATED
	 * Type check.
	 * isValue is set 'false' if both template arguments are different.
	 */
	template<class, class>
	inline constexpr B1 isValue = false;

	/* TEMPLATED
	 * Type check.
	 * isValue is set 'true' if both template arguments are the same.
	 */
	template<class TYPE_1>
	inline constexpr B1 isValue<TYPE_1, TYPE_1> = true;

	/* TEMPLATED
	 * Store the final result in a static variable.
	 * Castable to fit any type.
	 *
	 * @usage: Constant<TYPE, value>::result
	 */
	template<class TYPE, TYPE value>
	struct Constant
	{
		static constexpr TYPE result = value;
	};

	/* TEMPLATED
	 * BASE: Constant
	 * Type check for the Dynamik Engine.
	 * Check if a type matched another type.
	 *
	 * @usage: isType<firstType, secondType>::result
	 * @result: 'true' (1) if both the types are equal. 'false' (0) if both types are unequal.
	 */
	template<class TYPE_1, class TYPE_2>
	struct isType : Constant<B1, isValue<TYPE_1, TYPE_2>> {};

	/* TYPE CHECK ASSERTION MACRO
	 * Compile time evaluation.
	 *
	 * @param TYPE_1: First type.
	 * @param TYPE_2: Second type.
	 * @param MSG: Message to be printed if the assertion occurs.
	 */
#define STATIC_TYPE_CHECK(TYPE_1, TYPE_2, MSG)	static_assert(!::Dynamik::isType<TYPE_1, TYPE_2>::result, MSG)

	template<class TYPE>
	constexpr TYPE&& forward(TYPE&& args)
	{
		return static_cast<TYPE&&>(args);
	}
}

#endif // !_DATA_TYPES_TYPE_CHECK_H
