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
	public:
		StaticArray() {}
		~StaticArray() {}

	private:
		TYPE myData[Size];
		POINTER<TYPE> myNextPointer = myData;
	};
}

#endif // !_DATA_TYPES_STATIC_ARRAY_H
