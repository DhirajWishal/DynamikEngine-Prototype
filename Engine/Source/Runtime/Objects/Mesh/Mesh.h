#pragma once
#ifndef _DYNAMIK_MESH_H
#define _DYNAMIK_MESH_H

#include "Public/Array.h"

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
		ARRAY<MeshPointStore> data;
		ARRAY<UI32> indexes;
	};
}

#endif // !_DYNAMIK_MESH_H
