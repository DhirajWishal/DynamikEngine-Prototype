#include "adgrafx.h"
#include "VulkanUniformBuffer.h"

#include "VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanUniformBuffer::initialize(VulkanDevice device, VulkanSwapChain swapChain)
			{
				VkDeviceSize bufferSize = sizeof(UniformBufferObject);

				buffers.resize(swapChain.mySwapChainImages.size());
				bufferMemories.resize(swapChain.mySwapChainImages.size());

				for (size_t i = 0; i < swapChain.mySwapChainImages.size(); i++) {
					ADGRCreateBufferInfo bufferInfo;
					bufferInfo.device = device;
					bufferInfo.bufferSize = bufferSize;
					bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &buffers[i];
					bufferInfo.bufferMemory = &bufferMemories[i];

					VulkanFunctions::createBuffer(bufferInfo);
				}
			}

			void VulkanUniformBuffer::update(VulkanDevice device, UniformBufferObject uniformBuferObject, UI32 currentImage)
			{
				void* data = nullptr;
				vkMapMemory(device.logicalDevice, bufferMemories[currentImage], 0, sizeof(uniformBuferObject), 0, &data);
				memcpy(data, &uniformBuferObject, sizeof(uniformBuferObject));
				vkUnmapMemory(device.logicalDevice, bufferMemories[currentImage]);
			}

			void VulkanUniformBuffer::terminate(VulkanDevice device)
			{
				for (I32 x = 0; x < buffers.size(); x++) {
					vkDestroyBuffer(device.logicalDevice, buffers[x], nullptr);
					vkFreeMemory(device.logicalDevice, bufferMemories[x], nullptr);
				}
			}
		}
	}
}