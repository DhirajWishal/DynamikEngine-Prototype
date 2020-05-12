#pragma once
#ifndef _DYNAMIK_MESH_H
#define _DYNAMIK_MESH_H

#include <vector>
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
		virtual ~Mesh() {}

		CCPTR name = "";
		std::vector<MeshPointStore> vertexDataStore;
		std::vector<UI32> indexes;
		std::vector<Texture> textureDatas;

		UI32 allocatableSize(std::vector<DMKVertexAttribute> attributes);
		void packData(std::vector<DMKVertexAttribute> attributes, VPTR data);

	protected:
		std::vector<F32> _getAttributeData(DMKDataType type, VEC3 data);
		UI32 _getNextPointerAddress(DMKVertexAttribute attribute);
	};
}

#endif // !_DYNAMIK_MESH_H
