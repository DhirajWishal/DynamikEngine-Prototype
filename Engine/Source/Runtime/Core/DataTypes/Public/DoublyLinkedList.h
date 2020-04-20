#pragma once
#ifndef _DYNAMIK_DATA_TYPES_DOUBLY_LINKED_LIST_H
#define _DYNAMIK_DATA_TYPES_DOUBLY_LINKED_LIST_H

#include "Pointer.h"
#include "Public/StaticAllocator.h"

namespace Dynamik {
	template<class TYPE>
	class DoublyNode {
		using PTR = POINTER<DoublyNode<TYPE>>;

	public:
		DoublyNode() {}
		~DoublyNode() {}

		TYPE myData;

		PTR myNextNode;
		PTR myPreviousNode;
		UI32 myIndex = 0;
	};

	template<class TYPE, class Allocator = StaticAllocator<DoublyNode<TYPE>>>
	class DoublyLinkedList {
		using PTR = POINTER<DoublyNode<TYPE>>;

	public:
		DoublyLinkedList() {}
		~DoublyLinkedList()
		{
			PTR _itr = myFirstNode;
			PTR _final;
			while (_itr.isValid())
			{
				_final = _itr;
				_itr = (_itr->myNextNode);
				_final->~DoublyNode();
				Allocator::deAllocate(_final);
			}
		}

		void push(const DoublyNode<TYPE>& node)
		{
			PTR _nodeAllocation = Allocator::allocate(sizeof(node));
			_nodeAllocation.set((DoublyNode<TYPE>&&)node);
			_nodeAllocation->myIndex = myLinkCount;
			_nodeAllocation->myPreviousNode = myLastNode;

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

		DoublyNode<TYPE> pop(UI32 index)
		{
			PTR _itr = myFirstNode;
			PTR _last = _itr;
			PTR _next = _itr;
			while (_itr.isValid())
			{
				if (_itr->myIndex == index)
				{
					_last->myNextNode = _next.get();
					_next->myPreviousNode = _last.get();
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

			DoublyNode<TYPE> _final = _itr.dereference();

			_itr->~DoublyNode();
			Allocator::deAllocate(_itr);

			return _final;
		}

		UI32 size() { return myLinkCount; }

	private:
		PTR myFirstNode;
		PTR myLastNode = myFirstNode;
		UI32 myLinkCount = 0;
	};
}

#endif // !_DATA_TYPES_DOUBLY_LINKED_LIST_H
