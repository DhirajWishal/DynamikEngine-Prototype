#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "core/utils/DMK_DataTypes.h"
#include "Renderer/backend/interface.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			void createBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size,
				VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
				VkDeviceMemory& bufferMemory);

			uint32 findMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties,
				VkPhysicalDevice physicalDevice);

			void copyBuffer(VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
				VkCommandPool commandPool, VkQueue graphicsQueue);
		}
	}
}