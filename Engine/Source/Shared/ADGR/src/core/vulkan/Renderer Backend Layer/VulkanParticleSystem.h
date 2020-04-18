#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_PARTICLE_SYSTEM_H
#define _DYNAMIK_ADGR_VULKAN_PARTICLE_SYSTEM_H

#include "Graphics/VulkanGraphicsRenderableObject.h"
#include "Compute/VulkanComputeBufferObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanParticleSystem :
				public VulkanGraphicsRenderableObject {
			public:
				VulkanParticleSystem() {}
				VulkanParticleSystem(ADGRVulkanGraphicsRenderableObjectInitInfo info) : VulkanGraphicsRenderableObject(info) {}
				~VulkanParticleSystem() {}

				virtual void initializeComputeBuffer(ADGRVulkanComputeObjectInitInfo info);

				VulkanComputeBufferObject computeBuffer;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_PARTICLE_SYSTEM_H
