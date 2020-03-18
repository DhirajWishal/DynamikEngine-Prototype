#include "DataTypesLib/Public/IndexedArray.h"

namespace Dynamik {
	IndexedArray::IndexedArray() {}

	IndexedArray::~IndexedArray()
	{
		ARRAY<POINTER<VPTR>> _myArr(myIndexArray.size());
		for (auto _elem : myIndexArray)
		{
			POINTER<VPTR> _ptr = _elem.myAddress;
			_myArr.pushBack(_ptr);
		}

		for (auto _elem : _myArr)
		{
			_elem.~POINTER();
			StaticAllocator<VPTR>::deAllocate(_elem);
		}
	}

	IndexedArray::ITERATOR IndexedArray::begin()
	{
		return myIndexArray.begin();
	}

	IndexedArray::ITERATOR IndexedArray::end()
	{
		return myIndexArray.end();
	}
}