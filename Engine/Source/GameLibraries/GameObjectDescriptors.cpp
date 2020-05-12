#include "dmkafx.h"
#include "GameObjectDescriptors.h"

namespace Dynamik {
	UI32 DMKVertexBufferObjectDescriptor::vertexByteSize(ARRAY<DMKVertexAttribute> attributes)
	{
		UI32 _size = 0;

		for (auto attribute : attributes)
			_size += (UI32)attribute.dataType;

		return _size;
	}

	UI32 DMKUniformBufferObjectDescriptor::uniformByteSize(ARRAY<DMKUniformAttribute> attributes)
	{
		UI32 _size = 0;

		for (auto attribute : attributes)
			_size += (UI32)attribute.dataType;

		return _size;
	}

	/* Uniform Data Store object definition */
	DMKUniformBufferData::DMKUniformBufferData()
	{
		store = nullptr;
		nextPtr = store;
	}

	DMKUniformBufferData::~DMKUniformBufferData()
	{
	}

	void DMKUniformBufferData::initialize(ARRAY<DMKUniformAttribute> attributes)
	{
		_attributes = attributes;

		allocationSize = DMKUniformBufferObjectDescriptor::uniformByteSize(attributes);
		store = StaticAllocator<VPTR>::allocate(allocationSize);
		nextPtr = store;

		memset(store, 0, allocationSize);
	}

	void DMKUniformBufferData::clear()
	{
		memset(store, 0, allocationSize);
		nextPtr = store;
	}

	void DMKUniformBufferData::terminate()
	{
		StaticAllocator<VPTR>::deAllocate(store, allocationSize);
		isTerminated = true;

		store = nullptr;
		nextPtr = store;
	}

	void DMKUniformBufferData::addData(VPTR data, UI32 dataSize, UI32 dataLocation)
	{
		if (dataLocation < _attributes.size())
		{
			if ((UI32)_attributes[dataLocation].dataType != dataSize)
				DMK_CORE_FATAL("Invalid data size bound! Possibly because your binding the wront data.");

			if (dataLocation)
			{
				_incrementPtr((UI32)_attributes[dataLocation - 1].dataType);
				memcpy(nextPtr, data, dataSize);
			}
			else
				memcpy(store, data, dataSize);
		}
		else
			DMK_CORE_FATAL("Invalid data location bound!");
	}

	void DMKUniformBufferData::_incrementPtr(UI32 byteCount)
	{
		nextPtr = (VPTR)(((UI64)nextPtr) + byteCount);
	}
}
