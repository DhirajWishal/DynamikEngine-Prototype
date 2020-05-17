#include "dmkafx.h"
#include "AnimatedMesh.h"

#define MAX_BONES	64

namespace Dynamik {
	DMKSkeletalBoneInfo::DMKSkeletalBoneInfo()
	{
		offset = aiMatrix4x4();
		finalTransformation = aiMatrix4x4();
	}

	DMKSkeletalBoneInfo::~DMKSkeletalBoneInfo()
	{
	}

	void AnimatedMesh::setAnimation(UI32 index)
	{
		if (index >= scene->mNumAnimations)
			DMK_CORE_FATAL("Invalid animation index!");

		pAnimation = scene->mAnimations[index];
	}

	void AnimatedMesh::loadBones(const aiMesh* pMesh, UI32 vertexOffset, std::vector<VertexBoneData8> boneData)
	{
		for (UI32 _itr = 0; _itr < pMesh->mNumBones; _itr++)
		{
			UI32 index = 0;

			if (pMesh->mNumBones > MAX_BONES)
				DMK_CORE_FATAL("Invalid number of bones!");

			std::string name(pMesh->mBones[_itr]->mName.data);


			if (boneMapping.find(name) == boneMapping.end())
			{
				index = numBones;
				numBones++;
				DMKSkeletalBoneInfo bone;
				boneInfo.push_back(bone);
				boneInfo[index].offset = pMesh->mBones[_itr]->mOffsetMatrix;
				boneMapping[name] = index;
			}
			else
			{
				index = boneMapping[name];
			}

			for (UI32 j = 0; j < pMesh->mBones[_itr]->mNumWeights; j++)
			{
				UI32 vertexID = vertexOffset + pMesh->mBones[_itr]->mWeights[j].mVertexId;
				bones[vertexID].addBone(index, pMesh->mBones[_itr]->mWeights[j].mWeight);
			}
		}
		boneTransforms.resize(numBones);
	}

	void AnimatedMesh::update(F32 time)
	{
		time *= animationSpeed;

		float TicksPerSecond = (float)(scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f);
		float TimeInTicks = time * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, (float)scene->mAnimations[0]->mDuration);

		aiMatrix4x4 identity = aiMatrix4x4();
		readNodeHierarchy(AnimationTime, scene->mRootNode, identity);

		for (UI32 i = 0; i < boneTransforms.size(); i++)
			boneTransforms[i] = boneInfo[i].finalTransformation;
	}

	void AnimatedMesh::readNodeHierarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform)
	{
		std::string NodeName(pNode->mName.data);

		aiMatrix4x4 NodeTransformation(pNode->mTransformation);

		const aiNodeAnim* pNodeAnim = findNodeAnim(pAnimation, NodeName);

		if (pNodeAnim)
		{
			// Get interpolated matrices between current and next frame
			aiMatrix4x4 matScale = interpolateScale(AnimationTime, pNodeAnim);
			aiMatrix4x4 matRotation = interpolateRotation(AnimationTime, pNodeAnim);
			aiMatrix4x4 matTranslation = interpolateTranslation(AnimationTime, pNodeAnim);

			NodeTransformation = matTranslation * matRotation * matScale;
		}

		aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;

		if (boneMapping.find(NodeName) != boneMapping.end())
		{
			UI32 BoneIndex = boneMapping[NodeName];
			boneInfo[BoneIndex].finalTransformation = globalInverseTransform * GlobalTransformation * boneInfo[BoneIndex].offset;
		}

		for (UI32 i = 0; i < pNode->mNumChildren; i++)
			readNodeHierarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}

	const aiNodeAnim* AnimatedMesh::findNodeAnim(const aiAnimation* animation, const std::string nodeName)
	{
		for (UI32 i = 0; i < animation->mNumChannels; i++)
		{
			const aiNodeAnim* nodeAnim = animation->mChannels[i];
			if (std::string(nodeAnim->mNodeName.data) == nodeName)
				return nodeAnim;
		}

		return nullptr;
	}

	aiMatrix4x4 AnimatedMesh::interpolateScale(float time, const aiNodeAnim* pNodeAnim)
	{
		aiVector3D scale;

		if (pNodeAnim->mNumScalingKeys == 1)
		{
			scale = pNodeAnim->mScalingKeys[0].mValue;
		}
		else
		{
			UI32 frameIndex = 0;
			for (UI32 i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
			{
				if (time < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
				{
					frameIndex = i;
					break;
				}
			}

			aiVectorKey currentFrame = pNodeAnim->mScalingKeys[frameIndex];
			aiVectorKey nextFrame = pNodeAnim->mScalingKeys[(frameIndex + 1) % pNodeAnim->mNumScalingKeys];

			float delta = (time - (float)currentFrame.mTime) / (float)(nextFrame.mTime - currentFrame.mTime);

			const aiVector3D& start = currentFrame.mValue;
			const aiVector3D& end = nextFrame.mValue;

			scale = (start + delta * (end - start));
		}

		aiMatrix4x4 mat;
		aiMatrix4x4::Scaling(scale, mat);
		return mat;
	}

	aiMatrix4x4 AnimatedMesh::interpolateRotation(float time, const aiNodeAnim* pNodeAnim)
	{
		aiQuaternion rotation;

		if (pNodeAnim->mNumRotationKeys == 1)
		{
			rotation = pNodeAnim->mRotationKeys[0].mValue;
		}
		else
		{
			UI32 frameIndex = 0;
			for (UI32 i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
			{
				if (time < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
				{
					frameIndex = i;
					break;
				}
			}

			aiQuatKey currentFrame = pNodeAnim->mRotationKeys[frameIndex];
			aiQuatKey nextFrame = pNodeAnim->mRotationKeys[(frameIndex + 1) % pNodeAnim->mNumRotationKeys];

			float delta = (time - (float)currentFrame.mTime) / (float)(nextFrame.mTime - currentFrame.mTime);

			const aiQuaternion& start = currentFrame.mValue;
			const aiQuaternion& end = nextFrame.mValue;

			aiQuaternion::Interpolate(rotation, start, end, delta);
			rotation.Normalize();
		}

		aiMatrix4x4 mat(rotation.GetMatrix());
		return mat;
	}

	aiMatrix4x4 AnimatedMesh::interpolateTranslation(float time, const aiNodeAnim* pNodeAnim)
	{
		aiVector3D translation;

		if (pNodeAnim->mNumPositionKeys == 1)
		{
			translation = pNodeAnim->mPositionKeys[0].mValue;
		}
		else
		{
			UI32 frameIndex = 0;
			for (UI32 i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
			{
				if (time < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
				{
					frameIndex = i;
					break;
				}
			}

			aiVectorKey currentFrame = pNodeAnim->mPositionKeys[frameIndex];
			aiVectorKey nextFrame = pNodeAnim->mPositionKeys[(frameIndex + 1) % pNodeAnim->mNumPositionKeys];

			float delta = (time - (float)currentFrame.mTime) / (float)(nextFrame.mTime - currentFrame.mTime);

			const aiVector3D& start = currentFrame.mValue;
			const aiVector3D& end = nextFrame.mValue;

			translation = (start + delta * (end - start));
		}

		aiMatrix4x4 mat;
		aiMatrix4x4::Translation(translation, mat);
		return mat;
	}
}