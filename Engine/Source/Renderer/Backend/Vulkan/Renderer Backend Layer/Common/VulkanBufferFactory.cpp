#include "dmkafx.h"
#include "VulkanBufferFactory.h"

#include"../Common/VulkanUtilities.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			/* Buffer base class definitions */
			void VulkanBuffer::initialize(VulkanBufferInitInfo info)
			{
				dataCount = info.dataCount;

				VkDeviceSize bufferSize = info.byteSize;

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				VulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanUtilities::createBuffer(info.logicalDevice, info.physicalDevice, bufferInfo);

				void* data = nullptr;
				vkMapMemory(info.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, info.data, info.byteSize);
				vkUnmapMemory(info.logicalDevice, stagingBufferMemory);

				VulkanCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &buffer;
				vertBufferInfo.bufferMemory = &bufferMemory;

				VulkanUtilities::createBuffer(info.logicalDevice, info.physicalDevice, vertBufferInfo);

				VulkanUtilities::copyBuffer(info.logicalDevice, info.commandPool, info.processQueue, info.utilityQueue, stagingBuffer, buffer, bufferSize);

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
			void VulkanVertexBuffer::initialize(VulkanBufferInitInfo info)
			{
				dataCount = info.dataCount;

				VkDeviceSize bufferSize = info.byteSize;

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				VulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanUtilities::createBuffer(info.logicalDevice, info.physicalDevice, bufferInfo);

				void* data = nullptr;
				vkMapMemory(info.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, info.data, info.byteSize);
				vkUnmapMemory(info.logicalDevice, stagingBufferMemory);

				VulkanCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &buffer;
				vertBufferInfo.bufferMemory = &bufferMemory;

				VulkanUtilities::createBuffer(info.logicalDevice, info.physicalDevice, vertBufferInfo);

				VulkanUtilities::copyBuffer(info.logicalDevice, info.commandPool, info.processQueue, info.utilityQueue, stagingBuffer, buffer, bufferSize);

				vkDestroyBuffer(info.logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(info.logicalDevice, stagingBufferMemory, nullptr);
			}
			
			/* Index buffer initialize function override */
			void VulkanIndexBuffer::initialize(VulkanBufferInitInfo info)
			{
				dataCount = info.dataCount;

				VkDeviceSize bufferSize = info.byteSize;

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				VulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanUtilities::createBuffer(info.logicalDevice, info.physicalDevice, bufferInfo);

				void* data = nullptr;
				vkMapMemory(info.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, info.data, info.byteSize);
				vkUnmapMemory(info.logicalDevice, stagingBufferMemory);

				VulkanCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &buffer;
				vertBufferInfo.bufferMemory = &bufferMemory;

				VulkanUtilities::createBuffer(info.logicalDevice, info.physicalDevice, vertBufferInfo);

				VulkanUtilities::copyBuffer(info.logicalDevice, info.commandPool, info.processQueue, info.utilityQueue, stagingBuffer, buffer, bufferSize);

				vkDestroyBuffer(info.logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(info.logicalDevice, stagingBufferMemory, nullptr);
			}
			
			/* Uniform buffer initialize function override */
			void VulkanUniformBuffer::initialize(VulkanBufferInitInfo info)
			{
				VulkanCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = info.byteSize;
				bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &buffer;
				bufferInfo.bufferMemory = &bufferMemory;

				VulkanUtilities::createBuffer(info.logicalDevice, info.physicalDevice, bufferInfo);
			}
			
			/* Vulkan Buffer Factory: Create buffer function */
			VulkanBuffer& VulkanBufferFactory::createBuffer(VulkanBufferInitInfo info, VulkanBufferType type)
			{
				/* Check for the buffer type and return the initialized requested buffer */
				if (type == VulkanBufferType::ADGR_VULKAN_BUFFER_TYPE_GENERAL)
				{
					VulkanBuffer _buffer;
					_buffer.initialize(info);
					return _buffer;
				}
				else if (type == VulkanBufferType::ADGR_VULKAN_BUFFER_TYPE_VERTEX)
				{
					VulkanVertexBuffer _buffer;
					_buffer.initialize(info);
					return _buffer;
				}
				else if (type == VulkanBufferType::ADGR_VULKAN_BUFFER_TYPE_INDEX)
				{
					VulkanIndexBuffer _buffer;
					_buffer.initialize(info);
					return _buffer;
				}
				else if (type == VulkanBufferType::ADGR_VULKAN_BUFFER_TYPE_UNIFORM)
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