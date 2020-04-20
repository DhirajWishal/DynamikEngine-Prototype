#pragma once
#ifndef _DYNAMIK_DATA_TYPES_INDEXED_ARRAY_H
#define _DYNAMIK_DATA_TYPES_INDEXED_ARRAY_H

#include "Array.h"
#include "Dynamic.h"

namespace Dynamik {
	/*
	 * Allocation Data container for the Indexed Array data structure.
	 */
	struct AllocationData {
		AllocationData() {}
		~AllocationData()
		{
			myTypeName = "";
			myAddress = nullptr;
		}

		FSTR myTypeName = "";
		VPTR myAddress = nullptr;	// 32 bit size address

		void setAddress(VPTR addr)
		{
			myAddress = addr;
		}

		template<class T>
		void setTypeName()
		{
			myTypeName = typeid(T).name();
		}
	};

	/*
	 * Index array data structure.
	 * This array contains an array of heap allocated pointers.
	 * When indexed, it refers the index array and returns the pointer to the data.
	 * This way the data does not need to be contiguous and can have varyinig sizes.
	 */
	class IndexedArray {
	public:
		IndexedArray();
		~IndexedArray();

		typedef ARRAY<AllocationData>::ITERATOR ITERATOR;

		template<class SUB_TYPE>
		void push(SUB_TYPE data, UI32 size = 0)
		{
			AllocationData _myDataContainer;
			if (size)
				_myDataContainer.setAddress(StaticAllocator<VPTR>::allocate(size));
			else
				_myDataContainer.setAddress(StaticAllocator<VPTR>::allocate(sizeof(data)));
			_myDataContainer.setTypeName<SUB_TYPE>();

			StaticAllocator<SUB_TYPE>::set(_myDataContainer.myAddress, (SUB_TYPE&&)data);
			myIndexArray.pushBack((const AllocationData)_myDataContainer);
		}

		FSTR getTypeNameAt(I32 index = 0)
		{
			return myIndexArray.at(index).myTypeName;
		}

		template<class TYPE>
		TYPE& at(I32 index = 0)
		{
			return *(TYPE*)myIndexArray.at(index).myAddress;
		}

		template<class TYPE>
		TYPE& getFirstTypedData()
		{
			FSTR _typeName = typeid(TYPE).name();
			for (auto _allocData : myIndexArray)
				if (_allocData.myTypeName == _typeName)
					return *(TYPE*)_allocData.myAddress;
		}

		template<class TYPE>
		TYPE& getLastTypedData()
		{
			FSTR _typeName = typeid(TYPE).name();
			for (ARRAY<AllocationData>::ITERATOR _itr = myIndexArray.end(); _itr != myIndexArray.begin(); _itr--)
				if (_itr->myTypeName == _typeName)
					return *(TYPE*)_itr->myAddress;
		}

		template<class TYPE>
		TYPE& getTypedData(I32 firstIndex, UI32 lastIndex)
		{
			FSTR _typeName = typeid(TYPE).name();
			if (firstIndex == lastIndex);
			else if (firstIndex > lastIndex)
			{
				for (UI32 _index = lastIndex; _index <= firstIndex; _index++)
					if (myIndexArray[_index].myTypeName == _typeName)
						return *(TYPE*)myIndexArray[_index].myAddress;
			}
			else if (lastIndex > firstIndex)
			{
				for (UI32 _index = firstIndex; _index <= lastIndex; _index++)
					if (myIndexArray[_index].myTypeName == _typeName)
						return *(TYPE*)myIndexArray[_index].myAddress;
			}
		}

		ITERATOR begin();
		ITERATOR end();

	private:
		ARRAY<AllocationData> myIndexArray;
	};
}

#endif // !_DATA_TYPES_INDEXED_ARRAY_H
