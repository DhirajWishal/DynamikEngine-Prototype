#include "dmkafx.h"
#include "VulkanParticleSystem.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanParticleSystem::initializeComputeBuffer(ADGRVulkanComputeObjectInitInfo info)
			{
				computeBuffer = VulkanComputeBufferObject(info);
			}
		}
	}
}