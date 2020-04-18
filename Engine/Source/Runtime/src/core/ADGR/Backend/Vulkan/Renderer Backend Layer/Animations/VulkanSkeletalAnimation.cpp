#include "dmkafx.h"
#include "VulkanSkeletalAnimation.h"

#include "../Graphics/VulkanGraphicsFunctions.h"
#include "../Graphics/Attachments/VulkanGraphicsUniformBufferAttachment.h"

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

			ADGRVulkanRenderData VulkanSkeletalAnimation::initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples)
			{
				/* Resource Initialization */
				/* Attachment Initialization */
				ARRAY<ADGRVulkanTextureInitInfo> textureInitInfos;

				// initialize textures
				for (UI32 _itr = 0; _itr < _object.texturePaths.size(); _itr++)
				{
					ADGRVulkanTextureInitInfo initInfo;
					initInfo.path = _object.texturePaths[_itr];
					initInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
					initInfo.mipLevels = 1;
					initInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					initInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					initInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					initInfo.magFilter = VK_FILTER_LINEAR;
					initInfo.minFilter = VK_FILTER_LINEAR;
					initInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					textureInitInfos.pushBack(initInfo);
				}

				initializeTextures(textureInitInfos);

				// initialize uniform buffers
				initializeUniformBuffer();

				/* Pipeline Initialization */
				ADGRVulkanGraphicsPipelineLayoutInitInfo pipelineLayoutInitInfo;
				for (UI32 aItr = 0; aItr < myRenderData.attachments.size(); aItr++)
					pipelineLayoutInitInfo.layouts.pushBack(myRenderData.attachments[aItr].descriptor.layout);

				initializePipelineLayout(pipelineLayoutInitInfo);

				ARRAY<VulkanGraphicsShader> _shaders;

				if (_object.vertexShaderPath.size() && _object.vertexShaderPath != "NONE")
				{
					ADGRVulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = _object.vertexShaderPath;
					_initInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.tessellationShaderPath.size() && _object.tessellationShaderPath != "NONE")
				{
					ADGRVulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = _object.tessellationShaderPath;
					_initInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.geometryShaderPath.size() && _object.geometryShaderPath != "NONE")
				{
					ADGRVulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = _object.geometryShaderPath;
					_initInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.fragmentShaderPath.size() && _object.fragmentShaderPath != "NONE")
				{
					ADGRVulkanGraphicsShaderInitInfo _initInfo;
					_initInfo.path = _object.fragmentShaderPath;
					_initInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

					VulkanGraphicsShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}

				// initialize pipeline
				ADGRVulkanGraphicsPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = _shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
				pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
				pipelineInitInfo.isTexturesAvailable = _object.texturePaths.size();
				initializePipeline(pipelineInitInfo);

				for (VulkanGraphicsShader _shader : _shaders)
					_shader.terminate(logicalDevice);

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

			void VulkanSkeletalAnimation::initializeVertexBuffer(ARRAY<SkeletalVertex>* vertexBufferObject)
			{
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.vertexCount = vertexBufferObject->size();
				VkDeviceSize bufferSize = myRenderData.vertexCount * vertexBufferObject->typeSize();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRVulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);

				void* data = nullptr;
				vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, vertexBufferObject->data(), (size_t)bufferSize);
				vkUnmapMemory(logicalDevice, stagingBufferMemory);

				ADGRVulkanCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &_buffer;
				vertBufferInfo.bufferMemory = &_bufferMemory;

				VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, vertBufferInfo);

				VulkanGraphicsFunctions::copyBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue, stagingBuffer, _buffer, bufferSize);

				vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

				myRenderData.vertexBuffers.pushBack(_buffer);
				myRenderData.vertexBufferMemories.pushBack(_bufferMemory);
			}

			void VulkanSkeletalAnimation::initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos)
			{
				ADGRVulkanAttachmentInitInfo initInfo;
				initInfo.logicalDevice = logicalDevice;
				initInfo.physicalDevice = physicalDevice;
				initInfo.commandPool = commandPool;
				initInfo.processQueue = graphicsQueue;
				initInfo.utilityQueue = presentQueue;
				VulkanGraphicsTextureAttachmentCube _texture(initInfo);
				_texture.initializeTextures(infos);
				_texture.initializeDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);

				myRenderData.attachments.pushBack(_texture);
			}

			void VulkanSkeletalAnimation::initializeUniformBuffer()
			{
				ADGRVulkanAttachmentInitInfo initInfo;
				initInfo.logicalDevice = logicalDevice;
				initInfo.physicalDevice = physicalDevice;
				initInfo.commandPool = commandPool;
				initInfo.processQueue = graphicsQueue;
				initInfo.utilityQueue = presentQueue;
				VulkanGraphicsUniformBufferAttachment uboAttachment(initInfo);
				uboAttachment.initializeUniformBuffer(sizeof(UBO_SKELETAL), myRenderData.swapChainPointer->swapChainImages.size(), 0);
				uboAttachment.initializeDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);

				myRenderData.attachments.pushBack(uboAttachment);
				uniformBuffers.pushBack(uboAttachment.bufferContainer);
			}

			void VulkanSkeletalAnimation::updateUniformBuffer(UBO_SKELETAL uniformBufferObject, UI32 currentImage, F32 time)
			{
				update(time);

				for (UI32 _itr = 0; _itr < myAnimationData.boneTransforms.size(); _itr++)
					uniformBufferObject.bones[_itr] = glm::transpose(glm::make_mat4(&myAnimationData.boneTransforms[_itr].a1));

				for (ADGRVulkanUnformBufferContainer _container : uniformBuffers)
				{
					void* data = nullptr;
					vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uniformBufferObject), 0, &data);
					memcpy(data, &uniformBufferObject, sizeof(uniformBufferObject));
					vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);
				}
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