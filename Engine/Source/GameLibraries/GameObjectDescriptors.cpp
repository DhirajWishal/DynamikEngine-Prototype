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
}
