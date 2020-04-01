#include "adgrafx.h"
#include "VulkanUniformBufferManager.h"

#include "VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanUniformBufferManager::initialize(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object)
			{
				VkDeviceSize bufferSize = sizeof(UniformBufferObject);

				object->uniformBuffers.resize(core->swapChainImages.size());
				object->uniformBufferMemories.resize(core->swapChainImages.size());

				for (size_t i = 0; i < core->swapChainImages.size(); i++) {
					ADGRCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = bufferSize;
					bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &object->uniformBuffers[i];
					bufferInfo.bufferMemory = &object->uniformBufferMemories[i];

					VulkanFunctions::createBuffer(core, bufferInfo);
				}
			}

			void VulkanUniformBufferManager::update(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object, UniformBufferObject uniformBuferObject, UI32 currentImage)
			{
				void* data = nullptr;
				vkMapMemory(core->logicalDevice, object->uniformBufferMemories[currentImage], 0, sizeof(uniformBuferObject), 0, &data);
				memcpy(data, &uniformBuferObject, sizeof(uniformBuferObject));
				vkUnmapMemory(core->logicalDevice, object->uniformBufferMemories[currentImage]);
			}

			void VulkanUniformBufferManager::terminate(POINTER<VulkanCoreObject> core, VulkanRenderObject object)
			{
				for (I32 x = 0; x < object.uniformBuffers.size(); x++) {
					vkDestroyBuffer(core->logicalDevice, object.uniformBuffers[x], nullptr);
					vkFreeMemory(core->logicalDevice, object.uniformBufferMemories[x], nullptr);
				}
			}
		}
	}
}