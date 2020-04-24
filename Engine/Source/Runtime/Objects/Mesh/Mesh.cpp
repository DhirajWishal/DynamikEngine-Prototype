#include "dmkafx.h"
#include "Mesh.h"

namespace Dynamik {
	UI32 Mesh::allocatableSize(ARRAY<DMKVertexAttribute> attributes)
	{
		return vertexDataStore.size() * DMKVertexBufferObjectDescriptor::vertexByteSize(attributes);
	}

	void Mesh::packData(ARRAY<DMKVertexAttribute> attributes, VPTR ptr)
	{
		POINTER<F32> _pool = StaticAllocator<F32>::allocate(allocatableSize(attributes));
		POINTER<F32> nextPtr = _pool;
		UI32 _baseSize = sizeof(F32);

		for (MeshPointStore _store : vertexDataStore)
		{
			for (DMKVertexAttribute attribute : attributes)
			{
				ARRAY<F32> _tempArray;

				switch (attribute.name)
				{
				case DMKVertexData::DMK_VERTEX_DATA_POSITION:
					_tempArray = _getAttributeData(attribute.dataType, _store.position);
					moveBytes(nextPtr, _tempArray.begin(), _tempArray.end());
					nextPtr += _getNextPointerAddress(attribute);
					break;

				case DMKVertexData::DMK_VERTEX_DATA_COLOR:
					_tempArray = _getAttributeData(attribute.dataType, _store.color);
					moveBytes(nextPtr, _tempArray.begin(), _tempArray.end());
					nextPtr += _getNextPointerAddress(attribute);
					break;

				case DMKVertexData::DMK_VERTEX_DATA_TEXTURE_COORDINATES:
					_tempArray = _getAttributeData(attribute.dataType, _store.textureCoordinate);
					moveBytes(nextPtr, _tempArray.begin(), _tempArray.end());
					nextPtr += _getNextPointerAddress(attribute);
					break;

				case DMKVertexData::DMK_VERTEX_DATA_NORMAL_VECTORS:
					_tempArray = _getAttributeData(attribute.dataType, _store.normal);
					moveBytes(nextPtr, _tempArray.begin(), _tempArray.end());
					nextPtr += _getNextPointerAddress(attribute);
					break;

				case DMKVertexData::DMK_VERTEX_DATA_SPACE_VECTORS:
					_tempArray = _getAttributeData(attribute.dataType, _store.space);
					moveBytes(nextPtr, _tempArray.begin(), _tempArray.end());
					nextPtr += _getNextPointerAddress(attribute);
					break;

				case DMKVertexData::DMK_VERTEX_DATA_INTEGRITY:
					_tempArray = _getAttributeData(attribute.dataType, _store.integrity);
					moveBytes(nextPtr, _tempArray.begin(), _tempArray.end());
					nextPtr += _getNextPointerAddress(attribute);
					break;

				default:
					break;
				}
			}

		}

		moveBytes((POINTER<F32>)ptr, _pool, nextPtr);
	}

	ARRAY<F32> Mesh::_getAttributeData(DMKDataType type, ARRAY<F32> data)
	{
		ARRAY<F32> _attributeData;

		UI32 _dataCount = 0;
		UI32 _typeSize = (UI32)type;

		if (_typeSize % sizeof(F32) != 0)
			DMK_CORE_FATAL("Unsupported Vartex Data Format!");

		_dataCount = sizeof(F32) / _typeSize;

		if (_dataCount >= data.size())
			DMK_CORE_FATAL("Data count is out of bound!");

		for (UI32 _itr = 0; _itr < _dataCount; _itr++)
			_attributeData.pushBack(data[_itr]);

		return _attributeData;
	}

	UI32 Mesh::_getNextPointerAddress(DMKVertexAttribute attribute)
	{
		return sizeof(F32) / (UI32)attribute.dataType;
	}
}
