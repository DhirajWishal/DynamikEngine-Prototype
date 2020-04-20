#pragma once
#ifndef _DYNAMIK_MESH_H
#define _DYNAMIK_MESH_H

#include "Public/Array.h"
#include "GameObjectDescriptors.h"

namespace Dynamik {
	struct MeshPointStore {
		ARRAY<F32> position;
		ARRAY<F32> color;
		ARRAY<F32> textureCoordinate;
		ARRAY<F32> normal;
		ARRAY<F32> space;
		CCPTR matlabLibrary;

		F32 integrity = 1.0f;
	};

	class Mesh {
	public:
		Mesh() {}
		~Mesh() {}

		CCPTR name = "";
		ARRAY<MeshPointStore> vertexDataStore;
		ARRAY<UI32> indexes;

		UI32 allocatableSize(ARRAY<DMKVertexAttribute> attributes);
		void packData(ARRAY<DMKVertexAttribute> attributes, VPTR data);

	private:
		ARRAY<F32> _getAttributeData(DMKDataType type, ARRAY<F32> data);
		UI32 _getNextPointerAddress(DMKVertexAttribute attribute);
	};
}

#endif // !_DYNAMIK_MESH_H
