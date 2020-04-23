#include "dmkafx.h"
#include "VulkanSkeletalAnimation.h"

#include "../Common/VulkanUtilities.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void ADGRVulkanSkeletalBoneData::add(UI32 boneID, F32 weight)
			{
				for (UI32 _itr = 0; _itr < MAX_BONES_PER_VERTEX; _itr++)
				{
					if (weights[_itr] == 0.0f)
					{
						IDs[_itr] = boneID;
						weights[_itr] = weight;
						return;
					}
				}
			}

			ADGRVulkanSkeletalBoneInfo::ADGRVulkanSkeletalBoneInfo()
			{
				offset = aiMatrix4x4();
				finalTransformation = aiMatrix4x4();
			}

			VulkanSkeletalAnimation::VulkanSkeletalAnimation(ADGRVulkanGraphicsRenderableObjectInitInfo info) : VulkanGraphicsRenderableObject(info)
			{
				myRenderData.type = DMKObjectType::DMK_OBJECT_TYPE_SKELETAL_ANIMATION;
			}

			ADGRVulkanRenderData VulkanSkeletalAnimation::initializeObject(POINTER<InternalFormat> _object, VkSampleCountFlagBits msaaSamples)
			{
				ARRAY<VkDescriptorSetLayoutBinding> bindings;

				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				bindings.push_back(uboLayoutBinding);

				VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
				samplerLayoutBinding.binding = 1; // info.bindIndex;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				bindings.push_back(samplerLayoutBinding);

				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
				layoutInitInfo.bindings = bindings;
				myRenderData.descriptors.initializeLayout(logicalDevice, layoutInitInfo);

				ADGRVulkanGraphicsPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.layouts = { myRenderData.descriptors.layout };
				initializePipelineLayout(pipelineLayoutInitInfo);

				ARRAY<VulkanGraphicsShader> _shaders = VulkanUtilities::getGraphicsShaders(logicalDevice, _object);

				// initialize pipeline
				ADGRVulkanGraphicsPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = _shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				pipelineInitInfo.vertexBindingDescription = VulkanUtilities::getBindingDescription(descriptor.vertexBufferObjectDescription.attributes, 1);
				pipelineInitInfo.vertexAttributeDescription = VulkanUtilities::getAttributeDescriptions(descriptor.vertexBufferObjectDescription.attributes, 1);
				pipelineInitInfo.isTexturesAvailable = _object->texturePaths.size();
				pipelineInitInfo.rasterizerFrontFace = VK_FRONT_FACE_CLOCKWISE;
				initializePipeline(pipelineInitInfo);

				for (VulkanGraphicsShader _shader : _shaders)
					_shader.terminate(logicalDevice);

				// initialize textures
				initializeObjectResources();

				// initialize uniform buffers
				initializeUniformBuffer();

				// initialize descriptor pool
				initializeDescriptorPool();

				// initialize descriptor sets
				initializeDescriptorSets();

				return myRenderData;
			}

			void VulkanSkeletalAnimation::setAnimation(UI32 animationIndex)
			{
				if (animationIndex >= myAnimationData.scene->mNumAnimations)
					DMK_CORE_FATAL("Invalid animation index!");

				myAnimationData.pAnimation = myAnimationData.scene->mAnimations[animationIndex];
			}

			void VulkanSkeletalAnimation::loadBones(const aiMesh* pMesh, UI32 vertexOffset, ARRAY<ADGRVulkanSkeletalBoneData>& bones)
			{
				for (UI32 _itr = 0; _itr < pMesh->mNumBones; _itr++)
				{
					UI32 index = 0;

					if (pMesh->mNumBones > MAX_BONES)
						DMK_CORE_FATAL("Invalid number of bones!");

					std::string name(pMesh->mBones[_itr]->mName.data);

					if (myAnimationData.boneMapping.find(name) == myAnimationData.boneMapping.end())
					{
						// Bone not present, add new one
						index = myAnimationData.numBones;
						myAnimationData.numBones++;
						ADGRVulkanSkeletalBoneInfo bone;
						myAnimationData.boneInfo.push_back(bone);
						myAnimationData.boneInfo[index].offset = pMesh->mBones[_itr]->mOffsetMatrix;
						myAnimationData.boneMapping[name] = index;
					}
					else
					{
						index = myAnimationData.boneMapping[name];
					}

					for (UI32 j = 0; j < pMesh->mBones[_itr]->mNumWeights; j++)
					{
						UI32 vertexID = vertexOffset + pMesh->mBones[_itr]->mWeights[j].mVertexId;
						bones[vertexID].add(index, pMesh->mBones[_itr]->mWeights[j].mWeight);
					}
				}
				myAnimationData.boneTransforms.resize(myAnimationData.numBones);
			}

			void VulkanSkeletalAnimation::update(F32 time)
			{
				float TicksPerSecond = (float)(myAnimationData.scene->mAnimations[0]->mTicksPerSecond != 0 ? myAnimationData.scene->mAnimations[0]->mTicksPerSecond : 25.0f);
				float TimeInTicks = time * TicksPerSecond;
				float AnimationTime = fmod(TimeInTicks, (float)myAnimationData.scene->mAnimations[0]->mDuration);

				aiMatrix4x4 identity = aiMatrix4x4();
				readNodeHierarchy(AnimationTime, myAnimationData.scene->mRootNode, identity);

				for (UI32 i = 0; i < myAnimationData.boneTransforms.size(); i++)
					myAnimationData.boneTransforms[i] = myAnimationData.boneInfo[i].finalTransformation;
			}

			void VulkanSkeletalAnimation::draw(F32 time)
			{
			}

			void VulkanSkeletalAnimation::initializeUniformBuffer()
			{
				myRenderData.uniformBufferContainers.pushBack(
					VulkanUtilities::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UBO_SKELETAL),
						myRenderData.swapChainPointer->swapChainImages.size()
						)
					);
			}

			void VulkanSkeletalAnimation::updateUniformBuffer(UBO_SKELETAL uniformBufferObject, UI32 currentImage, F32 time)
			{
				update(time);

				for (UI32 _itr = 0; _itr < myAnimationData.boneTransforms.size(); _itr++)
					uniformBufferObject.bones[_itr] = glm::transpose(glm::make_mat4(&myAnimationData.boneTransforms[_itr].a1));

				for (ADGRVulkanUnformBufferContainer _container : myRenderData.uniformBufferContainers)
				{
					void* data = nullptr;
					vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uniformBufferObject), 0, &data);
					memcpy(data, &uniformBufferObject, sizeof(uniformBufferObject));
					vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);
				}
			}

			void VulkanSkeletalAnimation::initializeDescriptorSets()
			{
				ARRAY<VkWriteDescriptorSet> descriptorWrites = {};
				ARRAY<VkDescriptorBufferInfo> bufferInfos;

				for (UI32 itr = 0; itr < myRenderData.uniformBufferContainers.size(); itr++)
				{
					for (UI32 i = 0; i < myRenderData.uniformBufferContainers[itr].buffers.size(); i++)
					{
						VkDescriptorBufferInfo bufferInfo = {};
						bufferInfo.buffer = myRenderData.uniformBufferContainers[itr].buffers[i];
						bufferInfo.offset = 0;
						bufferInfo.range = sizeof(UBO_SKELETAL);
						bufferInfos.pushBack(bufferInfo);
					}
				}

				VkWriteDescriptorSet _writes1;
				_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes1.dstBinding = 0;
				_writes1.dstArrayElement = 0;
				_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_writes1.descriptorCount = 1;
				_writes1.pBufferInfo = bufferInfos.data();
				_writes1.pNext = VK_NULL_HANDLE;
				_writes1.pImageInfo = VK_NULL_HANDLE;
				_writes1.pTexelBufferView = VK_NULL_HANDLE;
				descriptorWrites.push_back(_writes1);

				if (myRenderData.textures.size())
				{
					for (UI32 _texIndex = 0; _texIndex < myRenderData.textures.size(); _texIndex++)
					{
						VkDescriptorImageInfo imageInfo = {};
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
						imageInfo.imageView = myRenderData.textures[_texIndex].imageView;
						imageInfo.sampler = myRenderData.textures[_texIndex].imageSampler;

						VkWriteDescriptorSet _writes2;
						_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes2.dstBinding = 1;
						_writes2.dstArrayElement = 0;
						_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						_writes2.descriptorCount = 1;
						_writes2.pImageInfo = &imageInfo;
						_writes2.pNext = VK_NULL_HANDLE;
						_writes2.pTexelBufferView = VK_NULL_HANDLE;
						_writes2.pBufferInfo = VK_NULL_HANDLE;
						descriptorWrites.push_back(_writes2);
					}
				}

				ADGRVulkanDescriptorSetsInitInfo initInfo;
				initInfo.descriptorWrites = descriptorWrites;
				myRenderData.descriptors.initializeSets(logicalDevice, initInfo);
			}

			const aiNodeAnim* VulkanSkeletalAnimation::findNodeAnim(const aiAnimation* animation, const std::string nodeName)
			{
				for (UI32 i = 0; i < animation->mNumChannels; i++)
				{
					const aiNodeAnim* nodeAnim = animation->mChannels[i];
					if (std::string(nodeAnim->mNodeName.data) == nodeName)
						return nodeAnim;
				}
				return nullptr;
			}

			aiMatrix4x4 VulkanSkeletalAnimation::interpolateTranslation(float time, const aiNodeAnim* pNodeAnim)
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

			aiMatrix4x4 VulkanSkeletalAnimation::interpolateRotation(float time, const aiNodeAnim* pNodeAnim)
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

			aiMatrix4x4 VulkanSkeletalAnimation::interpolateScale(float time, const aiNodeAnim* pNodeAnim)
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

			void VulkanSkeletalAnimation::readNodeHierarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform)
			{
				std::string NodeName(pNode->mName.data);

				aiMatrix4x4 NodeTransformation(pNode->mTransformation);

				const aiNodeAnim* pNodeAnim = findNodeAnim(myAnimationData.pAnimation, NodeName);

				if (pNodeAnim)
				{
					// Get interpolated matrices between current and next frame
					aiMatrix4x4 matScale = interpolateScale(AnimationTime, pNodeAnim);
					aiMatrix4x4 matRotation = interpolateRotation(AnimationTime, pNodeAnim);
					aiMatrix4x4 matTranslation = interpolateTranslation(AnimationTime, pNodeAnim);

					NodeTransformation = matTranslation * matRotation * matScale;
				}

				aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;

				if (myAnimationData.boneMapping.find(NodeName) != myAnimationData.boneMapping.end())
				{
					UI32 BoneIndex = myAnimationData.boneMapping[NodeName];
					myAnimationData.boneInfo[BoneIndex].finalTransformation = myAnimationData.globalInverseTransform * GlobalTransformation * myAnimationData.boneInfo[BoneIndex].offset;
				}

				for (UI32 i = 0; i < pNode->mNumChildren; i++)
					readNodeHierarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
			}
		}
	}
}