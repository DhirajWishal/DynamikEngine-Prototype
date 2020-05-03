#include "dmkafx.h"
#include "VulkanParticleSystem.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			void VulkanParticleSystem::initializeComputeBuffer(VulkanComputeObjectInitInfo info)
			{
				computeBuffer = VulkanComputeBufferObject(info);
			}
		}
	}
}