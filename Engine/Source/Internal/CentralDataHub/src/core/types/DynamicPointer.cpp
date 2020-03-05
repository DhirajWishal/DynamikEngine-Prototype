#include "cdhafx.h"
#include "DynamicPointer.h"

namespace Dynamik {
	DynamicPointer::DynamicPointer(VPTR ptr) : myPointer(ptr)
	{
	}

	DynamicPointer::DynamicPointer(UI32 size) : myAddressSize(size)
	{
		myPointer = malloc(myAddressSize);
	}

	DynamicPointer::~DynamicPointer()
	{
		delete myPointer;
	}
}