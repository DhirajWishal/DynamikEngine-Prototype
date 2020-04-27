#include "dmkafx.h"
#include "GraphicsObject.h"

namespace Dynamik {
	namespace ADGR {
		void GraphicsObject::initialize()
		{
			vulkanRenderer::initializeVertexBuffers(myInternalFormat);
			vulkanRenderer::initializeIndexBuffers(myInternalFormat);
			vulkanRenderer::initializeTextures(myInternalFormat);
			vulkanRenderer::initializeUniformBuffers(myInternalFormat);
			vulkanRenderer::initializePipeline(myInternalFormat);
		}
		
		void GraphicsObject::terminate()
		{
		}
	}
}