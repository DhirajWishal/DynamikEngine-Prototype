#pragma once
#ifndef _DYNAMIK_MESH_H
#define _DYNAMIK_MESH_H

#include "Public/Array.h"
#include "GameObjectDescriptors.h"
#include "../Texture/Texture.h"

namespace Dynamik {
	struct MeshPointStore {
		VEC3 position;
		VEC3 color;
		VEC3 textureCoordinate;
		VEC3 normal;
		VEC3 space;
		CCPTR matlabLibrary;

		F32 integrity = 1.0f;

		B1 operator==(const MeshPointStore& other) const;
	};

	class Mesh {
	public:
		Mesh() {}
		~Mesh() {}

		CCPTR name = "";
		ARRAY<MeshPointStore> vertexDataStore;
		ARRAY<UI32> indexes;
		Texture textureData;

		UI32 allocatableSize(ARRAY<DMKVertexAttribute> attributes);
		void packData(ARRAY<DMKVertexAttribute> attributes, VPTR data);

	private:
		ARRAY<F32> _getAttributeData(DMKDataType type, VEC3 data);
		UI32 _getNextPointerAddress(DMKVertexAttribute attribute);
	};
}

#endif // !_DYNAMIK_MESH_H
