#pragma once
#ifndef _DYNAMIK_AUTOMATED_HEAP_H
#define _DYNAMIK_AUTOMATED_HEAP_H

#include "Public/Array.h"

namespace Dynamik {
	/* Automated Heap allocator for the Dynamik Engine.
	 * This automatically destroys/ deallocates all the allocated heap memory pools upon shutdown.
	 */
	class AutomatedHeap {
		AutomatedHeap() {}
		~AutomatedHeap();

		static AutomatedHeap instance;

		struct _internalPointerContainer {
			_internalPointerContainer() {}
			~_internalPointerContainer() {}

			VPTR address = nullptr;
			UI32 byteSize = 0;
		};

	public:
		template<class TYPE>
		static POINTER<TYPE> allocate(UI32 byteSize = sizeof(TYPE))
		{
			POINTER<TYPE> ptr = StaticAllocator<TYPE>::allocate(byteSize);

			_internalPointerContainer _container;
			_container.address = ptr.get();
			_container.byteSize = byteSize;
			instance.addressStore.pushBack(_container);

			return ptr;
		}

		static void deallocate(VPTR address);

		template<class TYPE>
		static void setValue(POINTER<TYPE> address, TYPE&& data)
		{
			StaticAllocator<TYPE>::set(address, data);
		}

	private:
		ARRAY<_internalPointerContainer> addressStore;
	};
}

#endif // !_DYNAMIK_AUTOMATED_HEAP_H
