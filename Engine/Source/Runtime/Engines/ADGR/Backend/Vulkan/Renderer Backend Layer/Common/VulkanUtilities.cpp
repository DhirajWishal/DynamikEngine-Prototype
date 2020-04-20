#include "dmkafx.h"
#include "VulkanUtilities.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void _displayVertexData(const Vertex& vertex)
			{
				printf("Position (X, Y, Z): %f %f %f\n", vertex.Position.x, vertex.Position.y, vertex.Position.z);
				//printf("Color (R, G, B, A): %f %f %f %f\n", vertex.Color.r, vertex.Color.g, vertex.Color.b, vertex.Color.)
			}

			void VulkanUtilities::displayVertexBufferContent(VkDevice logicalDevice, VkBuffer buffer, VkDeviceMemory bufferMemory)
			{
				UI32 _baseSize = sizeof(Vertex);

				VkMemoryRequirements _memoryReq;
				vkGetBufferMemoryRequirements(logicalDevice, buffer, &_memoryReq);

				UI32 _vertexCount = _memoryReq.size / _baseSize;

				void* data = nullptr;
				if (vkMapMemory(logicalDevice, bufferMemory, 0, _memoryReq.size, 0, &data) != VK_SUCCESS)
					DMK_CORE_FATAL("Unable to read buffer memory!");

				for (UI32 _itr = 0; _itr < _vertexCount; _itr++)
				{
					Vertex* _vertexData = nullptr;
					memcpy(_vertexData, (void*)((UI64)data + (_baseSize * _itr)), _baseSize);
					_displayVertexData(*_vertexData);
				}

				vkUnmapMemory(logicalDevice, bufferMemory);
			}
		}
	}
}