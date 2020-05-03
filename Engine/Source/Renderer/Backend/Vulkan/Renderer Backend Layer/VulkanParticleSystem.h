#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_PARTICLE_SYSTEM_H
#define _DYNAMIK_RENDERER_VULKAN_PARTICLE_SYSTEM_H

#include "Graphics/VulkanGraphicsContainers.h"
#include "Compute/VulkanComputeBufferObject.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			class VulkanParticleSystem {
			public:
				VulkanParticleSystem() {}
				VulkanParticleSystem(VulkanGraphicsRenderableObjectInitInfo info) {}
				~VulkanParticleSystem() {}

				virtual void initializeComputeBuffer(VulkanComputeObjectInitInfo info);

				VulkanComputeBufferObject computeBuffer;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_PARTICLE_SYSTEM_H
