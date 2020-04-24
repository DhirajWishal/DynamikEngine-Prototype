#include "dmkafx.h"
#include "VulkanReflectObject.h"

#include "Common/VulkanUtilities.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanReflectObject::VulkanReflectObject(ADGRVulkanGraphicsRenderableObjectInitInfo info)
				: VulkanGraphicsRenderableObject(info)
			{
				myRenderData.type = DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT;
			}

			ADGRVulkanRenderData VulkanReflectObject::initializeObject(POINTER<InternalFormat> _object, VkSampleCountFlagBits msaaSamples)
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
				pipelineInitInfo.vertexBindingDescription = VulkanUtilities::getBindingDescription(myInternalFormat->descriptor.vertexBufferObjectDescription.attributes, 1);
				pipelineInitInfo.vertexAttributeDescription = VulkanUtilities::getAttributeDescriptions(myInternalFormat->descriptor.vertexBufferObjectDescription.attributes, 1);
				pipelineInitInfo.isTexturesAvailable = _object->texturePaths.size();
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

			void VulkanReflectObject::initializeUniformBuffer()
			{
				myRenderData.uniformBufferContainers.pushBack(
					VulkanUtilities::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UBO_MVPC),
						myRenderData.swapChainPointer->swapChainImages.size()
						)
					);
			}

			void VulkanReflectObject::updateUniformBuffer(UBO_MVPC uniformBufferObject, UI32 currentImage)
			{
				for (ADGRVulkanUnformBufferContainer _container : myRenderData.uniformBufferContainers)
				{
					void* data = nullptr;
					vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uniformBufferObject), 0, &data);
					memcpy(data, &uniformBufferObject, sizeof(uniformBufferObject));
					vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);
				}
			}

			void VulkanReflectObject::initializeDescriptorSets()
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
						bufferInfo.range = sizeof(UBO_MVPC);
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
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
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
		}
	}
}