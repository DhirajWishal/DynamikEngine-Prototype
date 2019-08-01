#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "src/backend/interface.h"
#include "vertexBuffer.h"
#include "core/utils/DMK_DataTypes.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class ADGR_API indexBuffer {
			public:
				indexBuffer(VkDevice* device, VkBuffer* buffer, VkDeviceMemory* bufferMemory);
				~indexBuffer() {}

				void initBuffer(vertexBuffer* vertexBuffer, VkCommandPool commandPool,
					VkQueue graphicsQueue);
				void deleteIndexBuffer();

				VkBuffer getIndexBuffer() { return *myIndexBuffer; }

			private:
				VkDevice* myDevice;
				VkBuffer* myIndexBuffer;
				VkDeviceMemory* myIndexBufferMemory;
			};

			const std::vector<uint16_t> indices = {
				0, 1, 2, 2, 3, 0
			};
		}
	}
}
