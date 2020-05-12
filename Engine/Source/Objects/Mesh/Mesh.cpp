#include "dmkafx.h"
#include "Mesh.h"

namespace Dynamik {
	UI32 Mesh::allocatableSize(std::vector<DMKVertexAttribute> attributes)
	{
		return vertexDataStore.size() * DMKVertexBufferObjectDescriptor::vertexByteSize(attributes);
	}

	void Mesh::packData(std::vector<DMKVertexAttribute> attributes, VPTR ptr)
	{
		POINTER<F32> nextPtr = ptr;

		for (MeshPointStore _store : vertexDataStore)
		{
			for (DMKVertexAttribute attribute : attributes)
			{
				switch (attribute.name)
				{
				case DMKVertexData::DMK_VERTEX_DATA_POSITION:
					memcpy(nextPtr.get(), &_store.position, (UI32)attribute.dataType);
					break;

				case DMKVertexData::DMK_VERTEX_DATA_COLOR:
					memcpy(nextPtr.get(), &_store.color, (UI32)attribute.dataType);
					break;

				case DMKVertexData::DMK_VERTEX_DATA_TEXTURE_COORDINATES:
					memcpy(nextPtr.get(), &_store.textureCoordinate, (UI32)attribute.dataType);
					break;

				case DMKVertexData::DMK_VERTEX_DATA_NORMAL_VECTORS:
					memcpy(nextPtr.get(), &_store.normal, (UI32)attribute.dataType);
					break;

				case DMKVertexData::DMK_VERTEX_DATA_SPACE_VECTORS:
					memcpy(nextPtr.get(), &_store.space, (UI32)attribute.dataType);
					break;

				case DMKVertexData::DMK_VERTEX_DATA_INTEGRITY:
					memcpy(nextPtr.get(), &_store.integrity, (UI32)attribute.dataType);
					break;
				}

				nextPtr += _getNextPointerAddress(attribute);
			}
		}
	}

	std::vector<F32> Mesh::_getAttributeData(DMKDataType type, VEC3 data)
	{
		std::vector<F32> _attributeData;

		UI32 _dataCount = 0;
		UI32 _typeSize = (UI32)type;

		if (_typeSize % sizeof(F32) != 0)
			DMK_CORE_FATAL("Unsupported Vartex Data Format!");

		if ((type == DMKDataType::DMK_DATA_TYPE_VEC3) || (type == DMKDataType::DMK_DATA_TYPE_BVEC3) || (type == DMKDataType::DMK_DATA_TYPE_UVEC3))
			_dataCount = 3;
		else
			_dataCount = _typeSize / sizeof(F32);

		if (_dataCount >= 4 && _typeSize != 16)
			DMK_CORE_FATAL("Data count is out of bound!");

		for (UI32 _itr = 0; _itr < _dataCount; _itr++)
			_attributeData.push_back(data[_itr]);

		return _attributeData;
	}

	UI32 Mesh::_getNextPointerAddress(DMKVertexAttribute attribute)
	{
		return (UI32)attribute.dataType / sizeof(F32);
	}

	B1 MeshPointStore::operator==(const MeshPointStore& other) const
	{
		return (
			position == other.position &&
			color == other.color &&
			textureCoordinate == other.textureCoordinate &&
			normal == other.normal &&
			space == other.space &&
			integrity == other.integrity);
	}
}
