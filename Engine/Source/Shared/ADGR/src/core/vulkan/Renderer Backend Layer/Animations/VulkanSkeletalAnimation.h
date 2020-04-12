#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_SKELETAL_ANIMATION_H
#define _DYNAMIK_ADGR_VULKAN_SKELETAL_ANIMATION_H

#include "../Objects/VulkanRenderableObject.h"

#include <assimp/assimp/Importer.hpp>
#include <assimp/assimp/scene.h>
#include <assimp/assimp/postprocess.h>
#include <assimp/assimp/cimport.h>

#include <map>
#include <string>

// Maximum number of bones per vertex
#define MAX_BONES_PER_VERTEX 4

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanSkeletalBoneData {
				std::array<UI32, MAX_BONES_PER_VERTEX> IDs;
				std::array<F32, MAX_BONES_PER_VERTEX> weights;

				void add(UI32 boneID, F32 weight);
			};

			struct ADGRVulkanSkeletalBoneInfo {
				ADGRVulkanSkeletalBoneInfo();
				~ADGRVulkanSkeletalBoneInfo() {}

				aiMatrix4x4 offset;
				aiMatrix4x4 finalTransformation;
			};

			struct ADGRVulkanSkeletalAnimationData {
				// Bone related stuff
				// Maps bone name with index
				std::map<std::string, UI32> boneMapping;
				// Bone details
				ARRAY<ADGRVulkanSkeletalBoneInfo> boneInfo;
				// Number of bones present
				UI32 numBones = 0;
				// Root inverese transform matrix
				aiMatrix4x4 globalInverseTransform;
				// Per-vertex bone info
				ARRAY<ADGRVulkanSkeletalBoneData> bones;
				// Bone transformations
				ARRAY<aiMatrix4x4> boneTransforms;

				// Modifier for the animation
				float animationSpeed = 0.75f;
				// Currently active animation
				aiAnimation* pAnimation;

				// Store reference to the ASSIMP scene for accessing properties of it during animation
				Assimp::Importer Importer;
				const aiScene* scene;
			};

			class VulkanSkeletalAnimation : public VulkanRenderableObject {
			public:
				VulkanSkeletalAnimation(ADGRVulkanRenderableObjectInitInfo info);
				virtual ~VulkanSkeletalAnimation() {}

				ADGRVulkanRenderData initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples) override;

				void setAnimation(UI32 animationIndex);
				void loadBones(const aiMesh* pMesh, UI32 vertexOffset, ARRAY<ADGRVulkanSkeletalBoneData>& bones);
				void update(F32 time);

				void draw(F32 time);

				void initializeVertexBuffer(ARRAY<SkeletalVertex>* vertexBufferObject);
				void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos) override;

				void initializeUniformBuffer() override;
				void updateUniformBuffer(UBO_SKELETAL uniformBufferObject, UI32 currentImage, F32 time);

				void initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info) override;

			public:
				ADGRVulkanSkeletalAnimationData myAnimationData;

			private:
				const aiNodeAnim* findNodeAnim(const aiAnimation* animation, const std::string nodeName);
				aiMatrix4x4 interpolateTranslation(float time, const aiNodeAnim* pNodeAnim);
				aiMatrix4x4 interpolateRotation(float time, const aiNodeAnim* pNodeAnim);
				aiMatrix4x4 interpolateScale(float time, const aiNodeAnim* pNodeAnim);
				void readNodeHierarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SKELETAL_ANIMATION_H
