#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_DYNAMIC_H
#define _DYNAMIK_CENTRAL_DATA_HUB_DYNAMIC_H

#include "core/global/datatypes.h"
#include <any>

namespace Dynamik {
	/*
	 * Dynamic Data Type for the Dynamik Engine
	 * This datatype can hold any type of data.
	 */
	class DYNAMIC {
		/* DEFAULTS */
	public:
		/* CONSTRUCTOR
		 * Default constructor.
		 */
		DYNAMIC() {}

		/* TEMPLATED CONSTRUCTOR
		 * Default constructor.
		 *
		 * @param data: Data to be added to the store.
		 */
		template<class TYPE>
		DYNAMIC(const TYPE& data) : myData((VPTR)&data) {}

		/* DESTRUCTOR
		 * Default destructor.
		 */
		~DYNAMIC() {}

		/* PUBLIC FUNCTIONS */
	public:
		/* FUNCTION
		 * Return the address of the store.
		 */
		VPTR getAddress() { return myData; }

		/* TEMPLATED FUNCTION
		 * Return the value in the store with a specific type.
		 */
		template<class TYPE>
		TYPE getCast() { return *(TYPE*)myData; }

		/* FUNCTION
		 * Check if the data store is not empty.
		 * true if data is stored, false if its empty.
		 */
		B1 hasValue() { return myData != nullptr; }

		/* FUNCTION
		 * Clear and reset the data in the store to default.
		 */
		void clear() { myData = nullptr; }

		/* PUBLIC OPERATORS */
	public:
		DYNAMIC operator=(const DYNAMIC& data)
		{
			this->myData = data.myData;
			return *this;
		}

		/* PRIVATE VARIABLES AND CONSTANTS */
	private:
		VPTR myData = nullptr;	// data container
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_DYNAMIC_H
