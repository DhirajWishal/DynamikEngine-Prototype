#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "Renderer/backend/interface.h"
#include "vertexBuffer.h"
#include "core/utils/DMK_DataTypes.h"
#include "Renderer/backend/loader/model.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class SMP_API indexBuffer {
			public:
				indexBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkBuffer* buffer,
					VkDeviceMemory* bufferMemory);
				~indexBuffer() {}

				void initBuffer(vertexBuffer* vertexBuffer, model myModel, VkCommandPool commandPool,
					VkQueue graphicsQueue);
				void deleteIndexBuffer();

				VkBuffer getIndexBuffer() { return *myIndexBuffer; }

			private:
				VkDevice* myDevice;
				VkPhysicalDevice* myPhysicalDevice;

				VkBuffer* myIndexBuffer;
				VkDeviceMemory* myIndexBufferMemory;
			};
		}
	}
}
