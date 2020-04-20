#pragma once
#ifndef _DYNAMIK_DATA_TYPES_STATIC_ARRAY_H
#define _DYNAMIK_DATA_TYPES_STATIC_ARRAY_H

#include "Array.h"

namespace Dynamik {
	/* TEMPLATED
	 * Static Array Datastructure for the Dynamik Engine.
	 */
	template<class TYPE, UI32 Size, DMKArrayDestructorCallMode DestructorCallMode = DMKArrayDestructorCallMode::DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_NONE>
	class StaticArray {
		using PTR = POINTER<TYPE>;
		using RPTR = TYPE*;

	public:
		StaticArray() {}
		~StaticArray() {}

		PTR data() { return myData; }

	private:
		TYPE myData[Size] = { TYPE() };
		POINTER<TYPE> myNextPointer = myData;
	};
}

#endif // !_DATA_TYPES_STATIC_ARRAY_H
