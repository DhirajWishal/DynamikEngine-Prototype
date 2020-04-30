#include "dmkafx.h"
#include "Public/AutomatedHeap.h"

namespace Dynamik {
	AutomatedHeap AutomatedHeap::instance;
	
	AutomatedHeap::~AutomatedHeap()
	{
		for (auto _container : instance.addressStore)
			StaticAllocator<VPTR>::deAllocate(_container.address, _container.byteSize);
	}

	void AutomatedHeap::deallocate(VPTR address)
	{
		for (UI32 _itr = 0; _itr < instance.addressStore.size(); _itr++)
		{
			if (instance.addressStore[_itr].address == address)
			{
				StaticAllocator<VPTR>::deAllocate(address, instance.addressStore[_itr].byteSize);

				instance.addressStore.remove(_itr);
				return;
			}
		}
	}
}