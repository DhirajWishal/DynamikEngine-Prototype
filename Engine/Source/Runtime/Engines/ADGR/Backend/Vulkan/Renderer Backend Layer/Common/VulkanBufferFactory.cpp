#include "dmkafx.h"
#include "VulkanBufferFactory.h"

#include "../Graphics/VulkanGraphicsFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			/* Buffer base class definitions */
			void VulkanBuffer::initialize(ADGRVulkanBufferInitInfo info)
			{
				dataCount = info.dataCount;

				VkDeviceSize bufferSize = info.byteSize;

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRVulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanGraphicsFunctions::createBuffer(info.logicalDevice, info.physicalDevice, bufferInfo);

				void* data = nullptr;
				vkMapMemory(info.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, info.data, info.byteSize);
				vkUnmapMemory(info.logicalDevice, stagingBufferMemory);

				ADGRVulkanCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &buffer;
				vertBufferInfo.bufferMemory = &bufferMemory;

				VulkanGraphicsFunctions::createBuffer(info.logicalDevice, info.physicalDevice, vertBufferInfo);

				VulkanGraphicsFunctions::copyBuffer(info.logicalDevice, info.commandPool, info.processQueue, info.utilityQueue, stagingBuffer, buffer, bufferSize);

				vkDestroyBuffer(info.logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(info.logicalDevice, stagingBufferMemory, nullptr);
			}
			
			void VulkanBuffer::update(VkDevice logicalDevice, VPTR data)
			{
				VPTR _mappedMemory = nullptr;
				vkMapMemory(logicalDevice, bufferMemory, 0, byteSize, 0, &_mappedMemory);
				memcpy(_mappedMemory, data, byteSize);
				vkUnmapMemory(logicalDevice, bufferMemory);
			}
			
			void VulkanBuffer::terminate(VkDevice logicalDevice)
			{
				vkDestroyBuffer(logicalDevice, buffer, nullptr);
				vkFreeMemory(logicalDevice, bufferMemory, nullptr);
			}
			
			/* Vertex buffer initialize function override */
			void VulkanVertexBuffer::initialize(ADGRVulkanBufferInitInfo info)
			{
				dataCount = info.dataCount;

				VkDeviceSize bufferSize = info.byteSize;

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRVulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanGraphicsFunctions::createBuffer(info.logicalDevice, info.physicalDevice, bufferInfo);

				void* data = nullptr;
				vkMapMemory(info.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, info.data, info.byteSize);
				vkUnmapMemory(info.logicalDevice, stagingBufferMemory);

				ADGRVulkanCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &buffer;
				vertBufferInfo.bufferMemory = &bufferMemory;

				VulkanGraphicsFunctions::createBuffer(info.logicalDevice, info.physicalDevice, vertBufferInfo);

				VulkanGraphicsFunctions::copyBuffer(info.logicalDevice, info.commandPool, info.processQueue, info.utilityQueue, stagingBuffer, buffer, bufferSize);

				vkDestroyBuffer(info.logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(info.logicalDevice, stagingBufferMemory, nullptr);
			}
			
			/* Index buffer initialize function override */
			void VulkanIndexBuffer::initialize(ADGRVulkanBufferInitInfo info)
			{
				dataCount = info.dataCount;

				VkDeviceSize bufferSize = info.byteSize;

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRVulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanGraphicsFunctions::createBuffer(info.logicalDevice, info.physicalDevice, bufferInfo);

				void* data = nullptr;
				vkMapMemory(info.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, info.data, info.byteSize);
				vkUnmapMemory(info.logicalDevice, stagingBufferMemory);

				ADGRVulkanCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &buffer;
				vertBufferInfo.bufferMemory = &bufferMemory;

				VulkanGraphicsFunctions::createBuffer(info.logicalDevice, info.physicalDevice, vertBufferInfo);

				VulkanGraphicsFunctions::copyBuffer(info.logicalDevice, info.commandPool, info.processQueue, info.utilityQueue, stagingBuffer, buffer, bufferSize);

				vkDestroyBuffer(info.logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(info.logicalDevice, stagingBufferMemory, nullptr);
			}
			
			/* Uniform buffer initialize function override */
			void VulkanUniformBuffer::initialize(ADGRVulkanBufferInitInfo info)
			{
				ADGRVulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = info.byteSize;
				bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &buffer;
				bufferInfo.bufferMemory = &bufferMemory;

				VulkanGraphicsFunctions::createBuffer(info.logicalDevice, info.physicalDevice, bufferInfo);
			}
			
			/* Vulkan Buffer Factory: Create buffer function */
			VulkanBuffer& VulkanBufferFactory::createBuffer(ADGRVulkanBufferInitInfo info, ADGRVulkanBufferType type)
			{
				/* Check for the buffer type and return the initialized requested buffer */
				if (type == ADGRVulkanBufferType::ADGR_VULKAN_BUFFER_TYPE_GENERAL)
				{
					VulkanBuffer _buffer;
					_buffer.initialize(info);
					return _buffer;
				}
				else if (type == ADGRVulkanBufferType::ADGR_VULKAN_BUFFER_TYPE_VERTEX)
				{
					VulkanVertexBuffer _buffer;
					_buffer.initialize(info);
					return _buffer;
				}
				else if (type == ADGRVulkanBufferType::ADGR_VULKAN_BUFFER_TYPE_INDEX)
				{
					VulkanIndexBuffer _buffer;
					_buffer.initialize(info);
					return _buffer;
				}
				else if (type == ADGRVulkanBufferType::ADGR_VULKAN_BUFFER_TYPE_UNIFORM)
				{
					VulkanUniformBuffer _buffer;
					_buffer.initialize(info);
					return _buffer;
				}
				else
				{
					DMK_CORE_FATAL("Invalid buffer type!");
				}

				return VulkanBuffer();
			}
		}
	}
}