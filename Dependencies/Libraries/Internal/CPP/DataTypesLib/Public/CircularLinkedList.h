#pragma once
#ifndef _DYNAMIK_DATA_TYPES_CIRCULAR_LINKED_LIST_H
#define _DYNAMIK_DATA_TYPES_CIRCULAR_LINKED_LIST_H

#include "Pointer.h"
#include "MemoryLib/Public/StaticAllocator.h"

namespace Dynamik {
	template<class TYPE>
	class CircularNode {
		using PTR = POINTER<CircularNode<TYPE>>;

	public:
		CircularNode() {}
		~CircularNode() {}

		TYPE myData;

		PTR myNextNode;
		UI32 myIndex = 0;
	};

	template<class TYPE, class Allocator = StaticAllocator<CircularNode<TYPE>>>
	class CircularLinkedList {
		using PTR = POINTER<CircularNode<TYPE>>;

	public:
		CircularLinkedList() {}
		~CircularLinkedList()
		{
			PTR _itr = myFirstNode;
			PTR _final;
			while (_itr.isValid())
			{
				_final = _itr;
				_itr = (_itr->myNextNode);
				_final->~CircularNode();
				Allocator::deAllocate(_final);
			}
		}

		void push(const CircularNode<TYPE>& node)
		{
			PTR _nodeAllocation = Allocator::allocate(sizeof(node));
			_nodeAllocation.set((CircularNode<TYPE>&&)node);
			_nodeAllocation->myIndex = myLinkCount;
			_nodeAllocation->myNextNode = myFirstNode;

			if (myFirstNode.isValid()) {

				myLastNode->myNextNode = _nodeAllocation;
			}
			else
				myFirstNode = _nodeAllocation;

			myLastNode = _nodeAllocation;
			myLinkCount++;
		}

		PTR get(UI32 index)
		{
			PTR _itr = myFirstNode;
			while (_itr.isValid())
			{
				if (_itr->myIndex == index)
					return _itr;

				_itr = (_itr->myNextNode);
			}

			return _itr;
		}

		CircularNode<TYPE> pop(UI32 index)
		{
			PTR _itr = myFirstNode;
			PTR _last = _itr;
			PTR _next = _itr;
			while (_itr.isValid())
			{
				if (_itr->myIndex == index)
				{
					_last->myNextNode = _next.get();
					_itr->myNextNode.turnNull();
					for (; _next.isValid(); _next = _next->myNextNode)
					{
						_next->myIndex = index;
						index++;
					}

					break;
				}

				_last = _itr;
				_itr = _itr->myNextNode;
				_next = _itr->myNextNode;
			}

			CircularNode<TYPE> _final = _itr.dereference();

			_itr->~CircularNode();
			Allocator::deAllocate(_itr);

			return _final;
		}

		UI32 size() { return myLinkCount; }

	private:
		PTR myFirstNode;
		PTR myLastNode;
		UI32 myLinkCount = 0;
	};
}

#endif // !_DATA_TYPES_CIRCULAR_LINKED_LIST_H
