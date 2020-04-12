#include "adgrafx.h"
#include "VulkanPBRObject.h"

#include "Objects/VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanPBRObject::VulkanPBRObject(ADGRVulkanRenderableObjectInitInfo initInfo) : VulkanRenderableObject(initInfo)
			{
				myRenderData.enableMaterials = true;
			}

			ADGRVulkanRenderData VulkanPBRObject::initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples)
			{
				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
				layoutInitInfo.overrideBindings = true;
				
				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.additionalBindings.pushBack(uboLayoutBinding);
				
				uboLayoutBinding.binding = 1; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				layoutInitInfo.additionalBindings.pushBack(uboLayoutBinding);
				myRenderData.descriptors.layout = VulkanRenderLayout::createDescriptorSetLayout(logicalDevice, layoutInitInfo);

				ADGRVulkanPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.layouts = { myRenderData.descriptors.layout };

				VkPushConstantRange pushConstantRange;
				pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				pushConstantRange.size = sizeof(glm::vec3);
				pushConstantRange.offset = 0;
				pipelineLayoutInitInfo.pushConstantRanges.pushBack(pushConstantRange);
				
				pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				pushConstantRange.size = sizeof(ADGRVulkanMaterialDescriptor::PushBlock);
				pushConstantRange.offset = sizeof(glm::vec3);
				pipelineLayoutInitInfo.pushConstantRanges.pushBack(pushConstantRange);
				myRenderData.pipelineLayout = VulkanRenderLayout::createPipelineLayout(logicalDevice, pipelineLayoutInitInfo);

				ARRAY<VulkanShader> _shaders;

				if (_object.vertexShaderPath.size() && _object.vertexShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.vertexShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;

					VulkanShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.tessellationShaderPath.size() && _object.tessellationShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.tessellationShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION;

					VulkanShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.geometryShaderPath.size() && _object.geometryShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.geometryShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY;

					VulkanShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}
				if (_object.fragmentShaderPath.size() && _object.fragmentShaderPath != "NONE")
				{
					ADGRVulkanShaderInitInfo _initInfo;
					_initInfo.path = _object.fragmentShaderPath;
					_initInfo.type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;

					VulkanShader _shader;
					_shader.initialize(logicalDevice, _initInfo);
					_shaders.pushBack(_shader);
				}

				// initialize pipeline
				ADGRVulkanPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = _shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				pipelineInitInfo.vertexBindingDescription = Vertex::getBindingDescription(1);
				pipelineInitInfo.vertexAttributeDescription = Vertex::getAttributeDescriptions();
				pipelineInitInfo.isTexturesAvailable = _object.texturePaths.size();
				pipelineInitInfo.depthStencilTestEnable = VK_TRUE;
				pipelineInitInfo.depthStencilWriteEnable = VK_TRUE;
				pipelineInitInfo.rasterizerCullMode = VK_CULL_MODE_FRONT_BIT;
				initializePipeline(pipelineInitInfo);

				for (VulkanShader _shader : _shaders)
					_shader.terminate(logicalDevice);

				ARRAY<ADGRVulkanTextureInitInfo> textureInitInfos;

				// initialize textures
				//for (UI32 _itr = 0; _itr < _object.texturePaths.size(); _itr++)
				//{
				//	ADGRVulkanTextureInitInfo initInfo;
				//	initInfo.path = _object.texturePaths[_itr];
				//	initInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
				//	initInfo.mipLevels = 1;
				//	initInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				//	initInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				//	initInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				//	initInfo.magFilter = VK_FILTER_LINEAR;
				//	initInfo.minFilter = VK_FILTER_LINEAR;
				//	initInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				//	textureInitInfos.pushBack(initInfo);
				//}
				//
				//initializeTextures(textureInitInfos);

				// initialize vertex buffers
				for (UI32 _itr = 0; _itr < _object.vertexBufferObjects->size(); _itr++)
					initializeVertexBuffer(&_object.vertexBufferObjects->at(_itr));

				// initialize index buffers
				for (UI32 _itr = 0; _itr < _object.indexBufferObjects->size(); _itr++)
					initializeIndexBufferUI32(&_object.indexBufferObjects->at(_itr));

				// initialize uniform buffers
				initializeUniformBuffer();

				// initialize descriptor pool
				ADGRVulkanDescriptorPoolInitInfo descriptorPoolInitInfo;
				initializeDescriptorPool(descriptorPoolInitInfo);

				// initialize descriptor sets
				ADGRVulkanDescriptorSetsInitInfo descriptorSetsInitInfo;
				initializeDescriptorSets(descriptorSetsInitInfo);

				return myRenderData;
			}

			void VulkanPBRObject::initializeUniformBuffer()
			{
				myRenderData.uniformBufferContainers.pushBack(
					VulkanFunctions::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UBO_MVPC),
						myRenderData.swapChainPointer->getSwapChainImages().size()
						)
					);

				myRenderData.uniformBufferContainers.pushBack(
					VulkanFunctions::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UBO_L4),
						myRenderData.swapChainPointer->getSwapChainImages().size()
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

			void VulkanPBRObject::initializeDescriptorPool(ADGRVulkanDescriptorPoolInitInfo info)
			{
				ARRAY<VkDescriptorPoolSize> poolSizes = {};

				VkDescriptorPoolSize _poolSize1;
				_poolSize1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_poolSize1.descriptorCount = myRenderData.uniformBufferContainers.size();
				poolSizes.push_back(_poolSize1);

				VkDescriptorPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				poolInfo.poolSizeCount = poolSizes.size();
				poolInfo.pPoolSizes = poolSizes.data();
				poolInfo.maxSets = 1;

				VkDescriptorPool _localDescriptorPool = VK_NULL_HANDLE;
				if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &_localDescriptorPool) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor pool!");

				myRenderData.descriptors.pool = _localDescriptorPool;
			}

			void VulkanPBRObject::initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info)
			{
				VkDescriptorSetLayout _layout = myRenderData.descriptors.layout;

				VkDescriptorSetAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = myRenderData.descriptors.pool;
				allocInfo.descriptorSetCount = 1;
				allocInfo.pSetLayouts = &_layout;

				if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, &myRenderData.descriptors.descriptorSet) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate descriptor sets!");

				ARRAY<VkWriteDescriptorSet> descriptorWrites = {};
				ARRAY<VkDescriptorBufferInfo> bufferInfos1;
				ARRAY<VkDescriptorBufferInfo> bufferInfos2;

				ADGRVulkanUnformBufferContainer _container1 = myRenderData.uniformBufferContainers[0];

				for (UI32 itr = 0; itr < _container1.buffers.size(); itr++)
				{
					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = _container1.buffers[itr];
					bufferInfo.offset = 0;
					bufferInfo.range = sizeof(UBO_MVPC);
					bufferInfos1.pushBack(bufferInfo);
				}

				ADGRVulkanUnformBufferContainer _container2 = myRenderData.uniformBufferContainers[1];

				for (UI32 itr = 0; itr < _container2.buffers.size(); itr++)
				{
					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = _container2.buffers[itr];
					bufferInfo.offset = 0;
					bufferInfo.range = sizeof(UBO_L4);
					bufferInfos2.pushBack(bufferInfo);
				}

				VkWriteDescriptorSet _writes1;
				_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes1.dstSet = myRenderData.descriptors.descriptorSet;
				_writes1.dstBinding = 0;
				_writes1.dstArrayElement = 0;
				_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_writes1.descriptorCount = 1;
				_writes1.pBufferInfo = bufferInfos1.data();
				_writes1.pNext = VK_NULL_HANDLE;
				_writes1.pImageInfo = VK_NULL_HANDLE;
				_writes1.pTexelBufferView = VK_NULL_HANDLE;
				descriptorWrites.push_back(_writes1);

				_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				_writes1.dstSet = myRenderData.descriptors.descriptorSet;
				_writes1.dstBinding = 1;
				_writes1.dstArrayElement = 0;
				_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				_writes1.descriptorCount = 1;
				_writes1.pBufferInfo = bufferInfos2.data();
				_writes1.pNext = VK_NULL_HANDLE;
				_writes1.pImageInfo = VK_NULL_HANDLE;
				_writes1.pTexelBufferView = VK_NULL_HANDLE;
				descriptorWrites.push_back(_writes1);

				for (VkWriteDescriptorSet _write : info.additionalWrites)
					descriptorWrites.push_back(_write);

				vkUpdateDescriptorSets(logicalDevice, static_cast<UI32>(descriptorWrites.size()),
					descriptorWrites.data(), 0, nullptr);
			}
		}
	}
}