#include "dmkafx.h"
#include "VulkanPBRObject.h"

#include "Common/VulkanUtilities.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanPBRObject::VulkanPBRObject(ADGRVulkanGraphicsRenderableObjectInitInfo initInfo) : VulkanGraphicsRenderableObject(initInfo)
			{
				myRenderData.enableMaterials = true;
			}

			ADGRVulkanRenderData VulkanPBRObject::initializeObject(POINTER<InternalFormat> _object, VkSampleCountFlagBits msaaSamples)
			{
				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;

				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 1; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 2; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 3; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 4; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 5; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 6; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 7; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 8; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);

				uboLayoutBinding.binding = 9; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.bindings.pushBack(uboLayoutBinding);
				myRenderData.descriptors.initializeLayout(logicalDevice, layoutInitInfo);

				ADGRVulkanGraphicsPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.layouts = { myRenderData.descriptors.layout };

				//VkPushConstantRange pushConstantRange;
				//pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				//pushConstantRange.size = sizeof(glm::vec3);
				//pushConstantRange.offset = 0;
				//pipelineLayoutInitInfo.pushConstantRanges.pushBack(pushConstantRange);
				//
				//pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				//pushConstantRange.size = sizeof(ADGRVulkanMaterialDescriptor::PushBlock);
				//pushConstantRange.offset = sizeof(glm::vec3);
				//pipelineLayoutInitInfo.pushConstantRanges.pushBack(pushConstantRange);
				initializePipelineLayout(pipelineLayoutInitInfo);
				//
				//myRenderData.pushConstants.pushBack(vertPushConstant);
				//myRenderData.pushConstants.pushBack(fragPushConstant);

				ARRAY<VulkanGraphicsShader> _shaders = VulkanUtilities::getGraphicsShaders(logicalDevice, _object);

				// initialize pipeline
				ADGRVulkanGraphicsPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = _shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				pipelineInitInfo.vertexBindingDescription = VulkanUtilities::getBindingDescription(myInternalFormat->descriptor.vertexBufferObjectDescription.attributes, 1);
				pipelineInitInfo.vertexAttributeDescription = VulkanUtilities::getAttributeDescriptions(myInternalFormat->descriptor.vertexBufferObjectDescription.attributes, 1);
				pipelineInitInfo.isTexturesAvailable = _object->texturePaths.size();
				pipelineInitInfo.depthStencilTestEnable = VK_TRUE;
				pipelineInitInfo.depthStencilWriteEnable = VK_TRUE;
				pipelineInitInfo.rasterizerCullMode = VK_CULL_MODE_FRONT_BIT;
				initializePipeline(pipelineInitInfo);

				for (VulkanGraphicsShader _shader : _shaders)
					_shader.terminate(logicalDevice);

				initializeObjectResources();

				// initialize uniform buffers
				initializeUniformBuffer();

				// initialize descriptor pool
				initializeDescriptorPool();

				// initialize descriptor sets
				initializeDescriptorSets();

				return myRenderData;
			}

			void VulkanPBRObject::initializeUniformBuffer()
			{
				myRenderData.uniformBufferContainers.pushBack(
					VulkanUtilities::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UBO_MVPC),
						myRenderData.swapChainPointer->swapChainImages.size()
						)
					);

				myRenderData.uniformBufferContainers.pushBack(
					VulkanUtilities::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UBO_L4),
						myRenderData.swapChainPointer->swapChainImages.size()
						)
					);
			}

			void VulkanPBRObject::updateUniformBuffer(UBO_MVPC uniformBufferObject, UI32 currentImage)
			{
				ADGRVulkanUnformBufferContainer _container = myRenderData.uniformBufferContainers[0];
				void* data = nullptr;
				vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uniformBufferObject), 0, &data);
				memcpy(data, &uniformBufferObject, sizeof(uniformBufferObject));
				vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);

				const float p = 15.0f;
				UBO_L4 uboParams;
				uboParams.lights[0] = glm::vec4(-p, -p * 0.5f, -p, 1.0f);
				uboParams.lights[1] = glm::vec4(-p, -p * 0.5f, p, 1.0f);
				uboParams.lights[2] = glm::vec4(p, -p * 0.5f, p, 1.0f);
				uboParams.lights[3] = glm::vec4(p, -p * 0.5f, -p, 1.0f);

				_container = myRenderData.uniformBufferContainers[1];
				data = nullptr;
				vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uboParams), 0, &data);
				memcpy(data, &uboParams, sizeof(uboParams));
				vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);
			}

			void VulkanPBRObject::initializeDescriptorPool()
			{
				ARRAY<VkDescriptorPoolSize> poolSizes = {};

				VkDescriptorPoolSize _poolSize1;
				_poolSize1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_poolSize1.descriptorCount = 1;
				poolSizes.push_back(_poolSize1);

				_poolSize1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_poolSize1.descriptorCount = 1;
				poolSizes.push_back(_poolSize1);

				if (myRenderData.textures.size())
				{
					VkDescriptorPoolSize _poolSize2;
					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);

					_poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_poolSize2.descriptorCount = 1;
					poolSizes.push_back(_poolSize2);
				}

				ADGRVulkanDescriptorPoolInitInfo initInfo;
				initInfo.poolSizes = poolSizes;
				initInfo.maxDescriptorSets = 1;
				myRenderData.descriptors.initializePool(logicalDevice, initInfo);
			}

			void VulkanPBRObject::initializeDescriptorSets()
			{
				ARRAY<VkWriteDescriptorSet> descriptorWrites = {};
				ARRAY< VkDescriptorBufferInfo> bufferInfos1;
				ARRAY< VkDescriptorBufferInfo> bufferInfos2;

				for (UI32 index = 0; index < myRenderData.uniformBufferContainers[0].buffers.size(); index++)
				{
					VkDescriptorBufferInfo bufferInfo1 = {};
					bufferInfo1.buffer = myRenderData.uniformBufferContainers[0].buffers[index];
					bufferInfo1.offset = 0;
					bufferInfo1.range = sizeof(UBO_MVPC);
					bufferInfos1.pushBack(bufferInfo1);

					VkDescriptorBufferInfo bufferInfo2 = {};
					bufferInfo2.buffer = myRenderData.uniformBufferContainers[1].buffers[index];
					bufferInfo2.offset = 0;
					bufferInfo2.range = sizeof(UBO_L4);
					bufferInfos2.pushBack(bufferInfo2);
				}

				{
					VkWriteDescriptorSet _writes1;
					_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes1.dstBinding = 0;
					_writes1.dstArrayElement = 0;
					_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					_writes1.descriptorCount = 1;
					_writes1.pBufferInfo = bufferInfos1.data();
					_writes1.pNext = VK_NULL_HANDLE;
					_writes1.pImageInfo = VK_NULL_HANDLE;
					_writes1.pTexelBufferView = VK_NULL_HANDLE;
					descriptorWrites.push_back(_writes1);
				}

				{
					VkWriteDescriptorSet _writes1;
					_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes1.dstBinding = 1;
					_writes1.dstArrayElement = 0;
					_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					_writes1.descriptorCount = 1;
					_writes1.pBufferInfo = bufferInfos2.data();
					_writes1.pNext = VK_NULL_HANDLE;
					_writes1.pImageInfo = VK_NULL_HANDLE;
					_writes1.pTexelBufferView = VK_NULL_HANDLE;
					descriptorWrites.push_back(_writes1);
				}

				{
					VkDescriptorImageInfo imageInfo = {};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = myIrradianceCube.myTextureContainer.imageView;
					imageInfo.sampler = myIrradianceCube.myTextureContainer.imageSampler;

					VkWriteDescriptorSet _writes2;
					_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes2.dstBinding = 2;
					_writes2.dstArrayElement = 0;
					_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_writes2.descriptorCount = 1;
					_writes2.pImageInfo = &imageInfo;
					_writes2.pNext = VK_NULL_HANDLE;
					_writes2.pTexelBufferView = VK_NULL_HANDLE;
					_writes2.pBufferInfo = VK_NULL_HANDLE;
					descriptorWrites.push_back(_writes2);
				}

				{
					VkDescriptorImageInfo imageInfo = {};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = myBRDF.myTextureContainer.imageView;
					imageInfo.sampler = myBRDF.myTextureContainer.imageSampler;

					VkWriteDescriptorSet _writes2;
					_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes2.dstBinding = 3;
					_writes2.dstArrayElement = 0;
					_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_writes2.descriptorCount = 1;
					_writes2.pImageInfo = &imageInfo;
					_writes2.pNext = VK_NULL_HANDLE;
					_writes2.pTexelBufferView = VK_NULL_HANDLE;
					_writes2.pBufferInfo = VK_NULL_HANDLE;
					descriptorWrites.push_back(_writes2);
				}

				{
					VkDescriptorImageInfo imageInfo = {};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = myPreFilteredCube.myTextureContainer.imageView;
					imageInfo.sampler = myPreFilteredCube.myTextureContainer.imageSampler;

					VkWriteDescriptorSet _writes2;
					_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes2.dstBinding = 4;
					_writes2.dstArrayElement = 0;
					_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_writes2.descriptorCount = 1;
					_writes2.pImageInfo = &imageInfo;
					_writes2.pNext = VK_NULL_HANDLE;
					_writes2.pTexelBufferView = VK_NULL_HANDLE;
					_writes2.pBufferInfo = VK_NULL_HANDLE;
					descriptorWrites.push_back(_writes2);
				}

				for (UI32 _texIndex = 0; _texIndex < myRenderData.textures.size(); _texIndex++)
				{
					VkDescriptorImageInfo imageInfo = {};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = myRenderData.textures[_texIndex].imageView;
					imageInfo.sampler = myRenderData.textures[_texIndex].imageSampler;

					VkWriteDescriptorSet _writes2;
					_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes2.dstBinding = 5 + _texIndex;
					_writes2.dstArrayElement = 0;
					_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					_writes2.descriptorCount = 1;
					_writes2.pImageInfo = &imageInfo;
					_writes2.pNext = VK_NULL_HANDLE;
					_writes2.pTexelBufferView = VK_NULL_HANDLE;
					_writes2.pBufferInfo = VK_NULL_HANDLE;
					descriptorWrites.push_back(_writes2);
				}

				ADGRVulkanDescriptorSetsInitInfo info;
				info.descriptorWrites = descriptorWrites;
				myRenderData.descriptors.initializeSets(logicalDevice, info);
			}

			VulkanPBRObject::_VertexPushConstant::_VertexPushConstant()
			{
				stageFlag = VK_SHADER_STAGE_VERTEX_BIT;
				offset = 0;
				byteSize = sizeof(glm::vec3);
			}

			VulkanPBRObject::_FragmentPushConstant::_FragmentPushConstant()
			{
				stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT;
				offset = sizeof(glm::vec3);
				byteSize = sizeof(ADGRVulkanMaterialDescriptor::PushBlock);
			}
		}
	}
}