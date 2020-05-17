#pragma once
#ifndef _DYNAMIK_ANIMATED_MESH_H
#define _DYNAMIK_ANIMATED_MESH_H

#include "Mesh.h"
#include <assimp/matrix4x4.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <map>

namespace Dynamik {
	struct DMKSkeletalBoneInfo {
		DMKSkeletalBoneInfo();
		~DMKSkeletalBoneInfo();

		aiMatrix4x4 offset;
		aiMatrix4x4 finalTransformation;
	};

	class AnimatedMesh : public Mesh {
	public:
		AnimatedMesh() {}
		~AnimatedMesh() {}

		void setAnimation(UI32 index);

		void loadBones(const aiMesh* pMesh, UI32 vertexOffset, std::vector<VertexBoneData8> boneData);

		// Bone related stuff
		// Maps bone name with index
		std::map<std::string, UI32> boneMapping;
		// Bone details
		std::vector<DMKSkeletalBoneInfo> boneInfo;
		// Number of bones present
		UI32 numBones = 0;

		// Root inverese transform matrix
		aiMatrix4x4 globalInverseTransform;
		// Per-vertex bone info
		std::vector<VertexBoneData8> bones;
		// Bone transformations
		std::vector<aiMatrix4x4> boneTransforms;

		// Modifier for the animation
		float animationSpeed = 0.75f;
		// Currently active animation
		aiAnimation* pAnimation;

		// Store reference to the ASSIMP scene for accessing properties of it during animation
		Assimp::Importer Importer;
		const aiScene* scene;

		void update(F32 time);
		void readNodeHierarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);
		const aiNodeAnim* findNodeAnim(const aiAnimation* animation, const std::string nodeName);
		aiMatrix4x4 interpolateScale(float time, const aiNodeAnim* pNodeAnim);
		aiMatrix4x4 interpolateRotation(float time, const aiNodeAnim* pNodeAnim);
		aiMatrix4x4 interpolateTranslation(float time, const aiNodeAnim* pNodeAnim);
	};
}

#endif // !_DYNAMIK_ANIMATED_MESH_H
