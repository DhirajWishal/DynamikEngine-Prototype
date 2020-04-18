#include "adgrafx.h"
#include "VulkanGraphicsRenderableObject.h"

#include "../Common/VulkanValidator.h"
#include "../Common/VulkanExtensionsManager.h"
#include "VulkanGraphicsFunctions.h"
#include "VulkanGraphicsOneTimeCommandBuffer.h"
#include "defines.h"

#include "Attachments/VulkanGraphicsUniformBufferAttachment.h"
#include "Attachments/VulkanGraphicsTextureAttachment2D.h"

#include "CentralDataHub.h"

/*
 OBJECT INITIALIZING PROCEDURE:
 1. Resource Initialization.
 2. Attachment Initializations.
 3. Pipeline Initialization.
 4. Additional Initializations.
*/

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			using namespace core;

			void VulkanGraphicsRenderableObject::initializeResources(ADGRVulkanGraphicsRenderableObjectInitInfo info)
			{
				logicalDevice = info.logicalDevice;
				physicalDevice = info.physicalDevice;
				graphicsQueue = info.graphicsQueue;
				presentQueue = info.presentQueue;
				commandPool = info.commandPool;
			}

			ADGRVulkanRenderData VulkanGraphicsRenderableObject::initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples)
			{
				/* Resource Initialization */
				// initialize vertex buffers
				for (UI32 _itr = 0; _itr < _object.vertexBufferObjects->size(); _itr++)
					initializeVertexBuffer(&_object.vertexBufferObjects->at(_itr));

				// initialize index buffers
				for (UI32 _itr = 0; _itr < _object.indexBufferObjects->size(); _itr++)
					initializeIndexBufferUI32(&_object.indexBufferObjects->at(_itr));

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

			void VulkanGraphicsRenderableObject::initializePipelineLayout(ADGRVulkanGraphicsPipelineLayoutInitInfo info)
			{
				VulkanGraphicsPipeline _pipeline;
				_pipeline.initializePipelineLayout(logicalDevice, info);
				myRenderData.pipelineContainers.pushBack(_pipeline);
			}

			void VulkanGraphicsRenderableObject::initializePipeline(ADGRVulkanGraphicsPipelineInitInfo info)
			{
				if (!myRenderData.pipelineContainers.size())
					DMK_CORE_FATAL("Pipeline layout was not initialized!");

				info.renderPass = myRenderData.frameBufferPointer->renderPass;
				info.swapChainExtent = myRenderData.swapChainPointer->swapChainExtent;

				UI32 _index = myRenderData.pipelineContainers.size() - 1;
				myRenderData.pipelineContainers[_index].initializePipeline(logicalDevice, info);
			}

			void VulkanGraphicsRenderableObject::terminatePipeline()
			{
				for (VulkanGraphicsPipeline _container : myRenderData.pipelineContainers)
					_container.terminate(logicalDevice);
			}

			void VulkanGraphicsRenderableObject::initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos)
			{
				ADGRVulkanAttachmentInitInfo initInfo;
				initInfo.logicalDevice = logicalDevice;
				initInfo.physicalDevice = physicalDevice;
				initInfo.commandPool = commandPool;
				initInfo.processQueue = graphicsQueue;
				initInfo.utilityQueue = presentQueue;
				VulkanGraphicsTextureAttachment2D _texture(initInfo);
				_texture.initializeTextures(infos);
				_texture.initializeDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);

				myRenderData.attachments.pushBack(_texture);
			}

			void VulkanGraphicsRenderableObject::terminateTextures()
			{

			}

			void VulkanGraphicsRenderableObject::initializeVertexBuffer(ARRAY<Vertex>* vertexes)
			{
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.vertexCount = vertexes->size();
				VkDeviceSize bufferSize = myRenderData.vertexCount * vertexes->typeSize();

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
				memcpy(data, vertexes->data(), (size_t)bufferSize);
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

			void VulkanGraphicsRenderableObject::initializeVertex2DBuffer(ARRAY<vertex2D>* vertexes)
			{
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.vertexCount = vertexes->size();
				VkDeviceSize bufferSize = myRenderData.vertexCount * vertexes->typeSize();

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
				memcpy(data, vertexes->data(), (size_t)bufferSize);
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

			void VulkanGraphicsRenderableObject::initializeVertexBufferP(ARRAY<VertexP>* vertexes)
			{
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.vertexCount = vertexes->size();
				VkDeviceSize bufferSize = myRenderData.vertexCount * vertexes->typeSize();

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
				memcpy(data, vertexes->data(), (size_t)bufferSize);
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

			void VulkanGraphicsRenderableObject::initializeVertexBufferPN(ARRAY<VertexPN>* vertexes)
			{
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.vertexCount = vertexes->size();
				VkDeviceSize bufferSize = myRenderData.vertexCount * vertexes->typeSize();

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
				memcpy(data, vertexes->data(), (size_t)bufferSize);
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

			void VulkanGraphicsRenderableObject::terminateVertexBuffer()
			{
				for (UI32 _itr = 0; _itr < myRenderData.vertexBuffers.size(); _itr++)
				{
					vkDestroyBuffer(logicalDevice, myRenderData.vertexBuffers[_itr], nullptr);
					vkFreeMemory(logicalDevice, myRenderData.vertexBufferMemories[_itr], nullptr);
				}
			}

			void VulkanGraphicsRenderableObject::initializeIndexBufferUI8(ARRAY<UI8>* indexes)
			{
				myRenderData.indexbufferObjectTypeSize = sizeof(UI8);
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.indexCount = indexes->size();
				VkDeviceSize bufferSize = myRenderData.indexCount * indexes->typeSize();

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
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(logicalDevice, stagingBufferMemory);

				ADGRVulkanCreateBufferInfo indexBufferInfo;
				indexBufferInfo.bufferSize = bufferSize;
				indexBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				indexBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				indexBufferInfo.buffer = &_buffer;
				indexBufferInfo.bufferMemory = &_bufferMemory;

				VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, indexBufferInfo);

				VulkanGraphicsFunctions::copyBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue, stagingBuffer, _buffer, bufferSize);

				vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

				myRenderData.indexBuffers.pushBack(_buffer);
				myRenderData.indexBufferMemories.pushBack(_bufferMemory);
			}

			void VulkanGraphicsRenderableObject::initializeIndexBufferUI16(ARRAY<UI16>* indexes)
			{
				myRenderData.indexbufferObjectTypeSize = sizeof(UI16);
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.indexCount = indexes->size();
				VkDeviceSize bufferSize = myRenderData.indexCount * indexes->typeSize();

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
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(logicalDevice, stagingBufferMemory);

				ADGRVulkanCreateBufferInfo indexBufferInfo;
				indexBufferInfo.bufferSize = bufferSize;
				indexBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				indexBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				indexBufferInfo.buffer = &_buffer;
				indexBufferInfo.bufferMemory = &_bufferMemory;

				VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, indexBufferInfo);

				VulkanGraphicsFunctions::copyBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue, stagingBuffer, _buffer, bufferSize);

				vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

				myRenderData.indexBuffers.pushBack(_buffer);
				myRenderData.indexBufferMemories.pushBack(_bufferMemory);
			}

			void VulkanGraphicsRenderableObject::initializeIndexBufferUI32(ARRAY<UI32>* indexes)
			{
				myRenderData.indexbufferObjectTypeSize = sizeof(UI32);
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.indexCount = indexes->size();
				VkDeviceSize bufferSize = myRenderData.indexCount * indexes->typeSize();

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
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(logicalDevice, stagingBufferMemory);

				ADGRVulkanCreateBufferInfo indexBufferInfo;
				indexBufferInfo.bufferSize = bufferSize;
				indexBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				indexBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				indexBufferInfo.buffer = &_buffer;
				indexBufferInfo.bufferMemory = &_bufferMemory;

				VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, indexBufferInfo);

				VulkanGraphicsFunctions::copyBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue, stagingBuffer, _buffer, bufferSize);

				vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

				myRenderData.indexBuffers.pushBack(_buffer);
				myRenderData.indexBufferMemories.pushBack(_bufferMemory);
			}

			void VulkanGraphicsRenderableObject::initializeIndexBufferUI64(ARRAY<UI64>* indexes)
			{
				myRenderData.indexbufferObjectTypeSize = sizeof(UI64);
				VkBuffer _buffer = VK_NULL_HANDLE;
				VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

				myRenderData.indexCount = indexes->size();
				VkDeviceSize bufferSize = myRenderData.indexCount * indexes->typeSize();

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
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(logicalDevice, stagingBufferMemory);

				ADGRVulkanCreateBufferInfo indexBufferInfo;
				indexBufferInfo.bufferSize = bufferSize;
				indexBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				indexBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				indexBufferInfo.buffer = &_buffer;
				indexBufferInfo.bufferMemory = &_bufferMemory;

				VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, indexBufferInfo);

				VulkanGraphicsFunctions::copyBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue, stagingBuffer, _buffer, bufferSize);

				vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

				myRenderData.indexBuffers.pushBack(_buffer);
				myRenderData.indexBufferMemories.pushBack(_bufferMemory);
			}

			void VulkanGraphicsRenderableObject::terminateIndexBuffer()
			{
				for (UI32 _itr = 0; _itr < myRenderData.indexBuffers.size(); _itr++)
				{
					vkDestroyBuffer(logicalDevice, myRenderData.indexBuffers[_itr], nullptr);
					vkFreeMemory(logicalDevice, myRenderData.indexBufferMemories[_itr], nullptr);
				}
			}

			void VulkanGraphicsRenderableObject::initializeUniformBuffer()
			{
				ADGRVulkanAttachmentInitInfo initInfo;
				initInfo.logicalDevice = logicalDevice;
				initInfo.physicalDevice = physicalDevice;
				initInfo.commandPool = commandPool;
				initInfo.processQueue = graphicsQueue;
				initInfo.utilityQueue = presentQueue;
				VulkanGraphicsUniformBufferAttachment uboAttachment(initInfo);
				uboAttachment.initializeUniformBuffer(sizeof(UniformBufferObject), myRenderData.swapChainPointer->swapChainImages.size(), 0);
				uboAttachment.initializeDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);

				myRenderData.attachments.pushBack(uboAttachment);
				uniformBuffers.pushBack(uboAttachment.bufferContainer);
			}

			void VulkanGraphicsRenderableObject::updateUniformBuffer(UniformBufferObject uniformBuferObject, UI32 currentImage)
			{
				for (ADGRVulkanUnformBufferContainer _container : uniformBuffers)
				{
					void* data = nullptr;
					vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uniformBuferObject), 0, &data);
					memcpy(data, &uniformBuferObject, sizeof(uniformBuferObject));
					vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);
				}
			}

			void VulkanGraphicsRenderableObject::terminateUniformBuffer()
			{
				for (ADGRVulkanUnformBufferContainer _container : uniformBuffers)
				{
					for (I32 x = 0; x < _container.buffers.size(); x++) {
						vkDestroyBuffer(logicalDevice, _container.buffers[x], nullptr);
						vkFreeMemory(logicalDevice, _container.bufferMemories[x], nullptr);
					}
				}
			}

			void VulkanGraphicsRenderableObject::initializeNoTextureDescriptorSetLayout()
			{
				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

				ARRAY<VkDescriptorSetLayoutBinding> bindings;
				bindings.push_back(uboLayoutBinding);

				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = static_cast<UI32>(bindings.size());
				layoutInfo.pBindings = bindings.data();

				if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &noTextureDescriptorSetLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor set layout!");
			}

			void VulkanGraphicsRenderableObject::initializeNoTexturePipelineLayout()
			{
				VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
				pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipelineLayoutInfo.setLayoutCount = 1;
				pipelineLayoutInfo.pSetLayouts = &noTextureDescriptorSetLayout;

				/*
				if (false) {
					ARRAY<VkPushConstantRange> pushConstantInfos;
					pushConstants.resize(info.pushConstantCount);

					// initialize push constants
					for (I32 i = 0; i <= info.pushConstantCount; i++) {
						VkPushConstantRange pushConsInfo = {};
						pushConsInfo.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
						pushConsInfo.size = pushConstants.typeSize() * info.pushConstantCount;
						pushConsInfo.offset = info.pushConstantOffset;
						//pushConsInfo.offset = pushConstants.typeSize() * i;

						pushConstantInfos.push_back(pushConsInfo);
					}
					pipelineLayoutInfo.pushConstantRangeCount = info.pushConstantCount;	// make support for multiple
					pipelineLayoutInfo.pPushConstantRanges = pushConstantInfos.data();
				}*/

				pipelineLayoutInfo.pushConstantRangeCount = 0;	// make support for multiple
				pipelineLayoutInfo.pPushConstantRanges = nullptr;

				// create the pipeline layout
				VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
				if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create pipeline layout!");
			}
		}
	}
}