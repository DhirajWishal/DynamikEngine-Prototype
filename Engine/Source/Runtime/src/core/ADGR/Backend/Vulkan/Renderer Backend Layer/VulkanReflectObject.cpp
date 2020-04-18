#include "dmkafx.h"
#include "VulkanReflectObject.h"

#include "Graphics/VulkanGraphicsFunctions.h"

#include "Graphics/Attachments/VulkanGraphicsUniformBufferAttachment.h"
#include "Graphics/Attachments/VulkanGraphicsTextureAttachmentCube.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanReflectObject::VulkanReflectObject(ADGRVulkanGraphicsRenderableObjectInitInfo info)
				: VulkanGraphicsRenderableObject(info)
			{
				myRenderData.type = DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT;
			}

			ADGRVulkanRenderData VulkanReflectObject::initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples)
			{
				// initialize vertex buffers
				for (UI32 _itr = 0; _itr < _object.vertexBufferObjects->size(); _itr++)
					initializeVertexBuffer(&_object.vertexBufferObjects->at(_itr));

				// initialize index buffers
				for (UI32 _itr = 0; _itr < _object.indexBufferObjects->size(); _itr++)
					initializeIndexBufferUI32(&_object.indexBufferObjects->at(_itr));

				// initialize uniform buffers
				initializeUniformBuffer();

				ARRAY<ADGRVulkanTextureInitInfo> textureInitInfos;

				// initialize textures
				if (_object.texturePaths.size())
				{
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
				}

				// initialize uniform buffers
				initializeUniformBuffer();

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

			void VulkanReflectObject::initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos)
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

			void VulkanReflectObject::initializeUniformBuffer()
			{
				ADGRVulkanAttachmentInitInfo initInfo;
				initInfo.logicalDevice = logicalDevice;
				initInfo.physicalDevice = physicalDevice;
				initInfo.commandPool = commandPool;
				initInfo.processQueue = graphicsQueue;
				initInfo.utilityQueue = presentQueue;
				VulkanGraphicsUniformBufferAttachment uboAttachment(initInfo);
				uboAttachment.initializeUniformBuffer(sizeof(UBO_MVPC), myRenderData.swapChainPointer->swapChainImages.size(), 0);
				uboAttachment.initializeDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);

				myRenderData.attachments.pushBack(uboAttachment);
				uniformBuffers.pushBack(uboAttachment.bufferContainer);
			}

			void VulkanReflectObject::updateUniformBuffer(UBO_MVPC uniformBufferObject, UI32 currentImage)
			{
				for (ADGRVulkanUnformBufferContainer _container : uniformBuffers)
				{
					void* data = nullptr;
					vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uniformBufferObject), 0, &data);
					memcpy(data, &uniformBufferObject, sizeof(uniformBufferObject));
					vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);
				}
			}
		}
	}
}