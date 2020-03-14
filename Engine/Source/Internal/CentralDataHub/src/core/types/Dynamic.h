#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_DYNAMIC_H
#define _DYNAMIK_CENTRAL_DATA_HUB_DYNAMIC_H

#include "core/global/datatypes.h"

namespace Dynamik {
	/*
	 * Dynamic Data Type for the Dynamik Engine
	 * This datatype can hold any type of data.
	 */
	class DYNAMIC {
	public:
		DYNAMIC() {}
		template<class TYPE>
		DYNAMIC(const TYPE& data) : myData((VPTR)&data) {}
		~DYNAMIC() {}

	public:
		template<class TYPE>
		TYPE getCast() { return (TYPE)myData; }

	private:
		VPTR myData;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_DYNAMIC_H
