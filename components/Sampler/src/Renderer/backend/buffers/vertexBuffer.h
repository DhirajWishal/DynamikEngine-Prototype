#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "Renderer/backend/interface.h"
#include "Renderer/backend/loader/model.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class SMP_API vertexBuffer {
			public:
				vertexBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
					VkBuffer* vertexBuffer, VkDeviceMemory* vertexBufferMemory);
				~vertexBuffer() {}

				void initBuffer(VkCommandPool commandPool, VkQueue graphicsQueue,
					model myModel);
				void deleteVertexBuffer();

				VkBuffer getVertexBuffer() const { return *myVertexBuffer; }

			private:
				VkDevice* myDevice;
				VkPhysicalDevice* myPhysicalDevice;
				VkBuffer* myVertexBuffer;
				VkDeviceMemory* myVertexBufferMemory;
			};
		}
	}
}
