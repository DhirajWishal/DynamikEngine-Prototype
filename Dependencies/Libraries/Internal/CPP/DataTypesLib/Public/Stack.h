#pragma once
#ifndef _DYNAMIK_DATA_TYPES_STACK_H
#define _DYNAMIK_DATA_TYPES_STACK_H

#include "Array.h"

namespace Dynamik {
	template<class TYPE>
	class STACK {
	public:
		STACK() {}
		~STACK() {}

		void push(TYPE& data)
		{
			myDataStore.pushBack(data);
		}

		TYPE front()
		{
			return myDataStore.front();
		}

		TYPE popFront()
		{
			return myDataStore.popFront();
		}

		UI32 size()
		{
			return myDataStore.size();
		}

	private:
		ARRAY<TYPE> myDataStore;
	};
}

#endif // !_DATA_TYPES_STACK_H
