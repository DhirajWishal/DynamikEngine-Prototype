#pragma once
#ifndef _DYNAMIK_DATA_TYPES_INITIALIZER_LIST_H
#define _DYNAMIK_DATA_TYPES_INITIALIZER_LIST_H

#include "Pointer.h"

namespace Dynamik {
	/*
	 * Initializer list data structure for the Dynamik Engine.
	 */
	template<class TYPE>
	class InitializerList {
		/* DEFAULTS */
	public:
		/* PUBLIC DATATYPE
		 * Iterator datatype for the the Initializer List.
		 */
		typedef POINTER<TYPE> ITERATOR;

	public:
		/* CONSTRUCTOR
		 * Default constructor.
		 */
		constexpr InitializerList() noexcept : myFirst(nullptr), myLast(nullptr) {}

		/* CONSTRUCTOR
		 * Default constructor.
		 *
		 * @param first: First address of the data.
		 * @param last: Last address of the data.
		 */
		constexpr InitializerList(const TYPE* first, const TYPE* last)
			noexcept : myFirst(first), myLast(last) {}

		/* CONSTRUCTOR
		 * Default constructor.
		 *
		 * @param first: First address of the data.
		 * @param last: Last address of the data.
		 */
		constexpr InitializerList(const InitializerList<TYPE>& list)
			noexcept : myFirst(list.begin()), myLast(list.end()) {}

		/* CONSTRUCTOR
		 * Default constructor.
		 *
		 * @param first: First address of the data.
		 * @param last: Last address of the data.
		 */
		constexpr InitializerList(InitializerList<TYPE>&&)
			noexcept : myFirst(list.begin()), myLast(list.end()) {}

		/* DESTRUCTOR
		 * Default destructor.
		 */
		 //~InitializerList() {}

		 /* PUBLIC FUNCTION */
	public:
		/* PUBLIC FUNCTION
		 * Return the first address.
		 */
		constexpr const ITERATOR begin() const noexcept
		{
			return myFirst;
		}

		/* PUBLIC FUNCTION
		 * Return the last address.
		 */
		constexpr const ITERATOR end() const noexcept
		{
			return myLast;
		}

		/* PUBLIC FUNCTION
		 * Return the size of the list.
		 * Returns the data count.
		 */
		constexpr UI32 size() const noexcept
		{
			return (myLast.getAddressAsInteger() - myFirst.getAddressAsInteger());
		}

		/* PRIVATE VARIABLES AND CONSTANTS */
	private:
		const POINTER<TYPE> myFirst;	// first address
		const POINTER<TYPE> myLast;	// last address
	};
}

#endif // !_DATA_TYPES_INITIALIZER_LIST_H
