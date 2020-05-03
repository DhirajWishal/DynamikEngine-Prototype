#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_PREFILTERED_CUBE_H
#define _DYNAMIK_RENDERER_VULKAN_PREFILTERED_CUBE_H

#include "VulkanIrradianceCube.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			class VulkanPrefilteredCube : public VulkanIrradianceCube {
			public:
				VulkanPrefilteredCube() {}
				VulkanPrefilteredCube(VulkanGraphicsSupportObjectInitInfo info) :
					VulkanIrradianceCube(info) {}
				~VulkanPrefilteredCube() {}

				void initialize() override;
				void _initializeTexture() override;
				void _initializePipelineLayout() override;
				void _initializePipeline() override;
				void _initializeCommandBuffers() override;

				struct PushBlock {
					glm::mat4 mvp;
					float roughness;
					UI32 numSamples = 32u;
				} pushBlock;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_PREFILTERED_CUBE_H
