#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "buffer.h"
#include "vertexBuffer.h"
#include "core/utils/DMK_DataTypes.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class DMK_API indexBuffer : public Buffer {
			public:
				indexBuffer(VkBuffer* indexBuffer, VkDeviceMemory* indexBufferMemory);
				~indexBuffer() {}

				void initBuffer(VkQueue graphicsQueue) override;
				void deleteBuffer();

				void setVertexBuffer(vertexBuffer* vertBuff);
				inline VkBuffer getBuffer() { return *myIndexBuffer; }

			private:
				vertexBuffer* myVertexBuffer;
			};
		}
		const std::vector<Dynamik::utils::uint16> indices = {
			0, 1, 2, 2, 3, 0
		};
	}
}


