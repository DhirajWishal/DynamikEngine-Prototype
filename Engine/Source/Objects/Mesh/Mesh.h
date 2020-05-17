#pragma once
#ifndef _DYNAMIK_MESH_H
#define _DYNAMIK_MESH_H

#include <vector>
#include "GameObjectDescriptors.h"
#include "../Texture/Texture.h"

namespace Dynamik {
	struct VertexBoneData {
		VertexBoneData() {}
		virtual ~VertexBoneData() {}

		virtual void addBone(UI32 boneID, F32 weight) {}
	};

	struct VertexBoneData8 : public VertexBoneData {
		UI32 boneIDs[8] = { 0 };
		F32 boneWeights[8] = { 0.0f };

		void addBone(UI32 boneID, F32 weight) override;
	};

	struct VertexBoneData16 : public VertexBoneData {
		UI32 boneIDs[16] = { 0 };
		F32 boneWeights[16] = { 0.0f };

		void addBone(UI32 boneID, F32 weight) override;
	};

	struct MeshPointStore {
		VEC3 position;
		VEC3 color;
		VEC3 textureCoordinate;
		VEC3 normal;
		VEC3 space;
		CCPTR matlabLibrary;

		F32 integrity = 1.0f;

		POINTER<VertexBoneData8> boneData;

		B1 operator==(const MeshPointStore& other) const;
	};

	struct MeshVertexLimits {
		VEC2 limitX = VEC2(1.0f);	/* Min, Max */
		VEC2 limitY = VEC2(1.0f);	/* Min, Max */
		VEC2 limitZ = VEC2(1.0f);	/* Min, Max */
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
