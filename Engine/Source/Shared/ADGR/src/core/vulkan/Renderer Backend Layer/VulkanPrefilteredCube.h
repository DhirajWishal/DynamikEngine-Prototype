#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_PREFILTERED_CUBE_H
#define _DYNAMIK_ADGR_VULKAN_PREFILTERED_CUBE_H

#include "VulkanIrradianceCube.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanPrefilteredCube : public VulkanIrradianceCube {
			public:
				VulkanPrefilteredCube() {}
				VulkanPrefilteredCube(ADGRVulkanGraphicsSupportObjectInitInfo info) :
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
					uint32_t numSamples = 32u;
				} pushBlock;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_PREFILTERED_CUBE_H
