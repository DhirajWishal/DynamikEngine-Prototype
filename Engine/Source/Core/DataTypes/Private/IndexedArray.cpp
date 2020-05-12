#include "dmkafx.h"
#include "Public/IndexedArray.h"

namespace Dynamik {
	IndexedArray::IndexedArray() {}

	IndexedArray::~IndexedArray()
	{
		std::vector<POINTER<VPTR>> _myArr(myIndexArray.size());
		for (auto _elem : myIndexArray)
		{
			POINTER<VPTR> _ptr = _elem.myAddress;
			_myArr.push_back(_ptr);
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