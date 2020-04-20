#pragma once
#ifndef _DYNAMIK_DATA_TYPES_SIMPLE_LINKED_LIST_H
#define _DYNAMIK_DATA_TYPES_SIMPLE_LINKED_LIST_H

#include "Pointer.h"
#include "Public/StaticAllocator.h"

namespace Dynamik {
	/* BASE CLASS
	 * Node structure for the linked list.
	 */
	template<class TYPE>
	class SimpleNode {
		using PTR = POINTER<SimpleNode<TYPE>>;

	public:
		SimpleNode() {}
		~SimpleNode() {}

		TYPE myData;

		PTR myNextNode;
		UI32 myIndex = 0;
	};

	/*
	 * Linked List data structure.
	 */
	template<class TYPE, class Allocator = StaticAllocator<SimpleNode<TYPE>>>
	class SimpleLinkedList {
		using PTR = POINTER<SimpleNode<TYPE>>;

	public:
		SimpleLinkedList() {}
		~SimpleLinkedList()
		{
			PTR _itr = myFirstNode;
			PTR _final;
			while (_itr.isValid())
			{
				_final = _itr;
				_itr = (_itr->myNextNode);
				_final->~SimpleNode();
				Allocator::deAllocate(_final);
			}
		}

		void push(const SimpleNode<TYPE>& node)
		{
			PTR _nodeAllocation = Allocator::allocate(sizeof(node));
			_nodeAllocation.set((SimpleNode<TYPE>&&)node);
			_nodeAllocation->myIndex = myLinkCount;

			if (myFirstNode.isValid())
				myLastNode->myNextNode = _nodeAllocation;
			else
				myFirstNode = _nodeAllocation;

			myLastNode = _nodeAllocation;
			myLinkCount++;
		}

		SimpleNode<TYPE> get(UI32 index)
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

		SimpleNode<TYPE> pop(UI32 index)
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

			SimpleNode<TYPE> _final = _itr.dereference();

			_itr->~SimpleNode();
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

#endif // !_DATA_TYPES_SIMPLE_LINKED_LIST_H
