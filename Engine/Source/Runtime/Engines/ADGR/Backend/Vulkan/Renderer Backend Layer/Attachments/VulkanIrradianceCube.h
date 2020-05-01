#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_IRRADIANCE_CUBE_H
#define _DYNAMIK_ADGR_VULKAN_IRRADIANCE_CUBE_H

#include "../Graphics/VulkanGraphicsSupportObject.h"

#define M_PI       3.14159265358979323846

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanIrradianceCube : public VulkanGraphicsSupportObject {
			public:
				VulkanIrradianceCube() {}
				VulkanIrradianceCube(ADGRVulkanGraphicsSupportObjectInitInfo info) :
					VulkanGraphicsSupportObject(info) {}
				virtual ~VulkanIrradianceCube() {}

				UI32 dimentions = 64;
				ADGRVulkanRenderData skyboxRenderData;

			protected:
				void _initializeTexture() override;
				void _initializeRenderPass() override;
				void _initializeOffscreenFrameBuffer() override;
				void _initializeFrameBuffer() override;
				void _initializeDescriptorSetLayout() override;
				void _initializeDescriptorPool() override;
				void _initializeDescriptorSets() override;
				void _initializePipelineLayout() override;
				void _initializePipeline() override;
				void _initializeCommandBuffers() override;

				ADGRVulkanGraphicsOffscreenFrameBufferContainer offscreenContainer;

				struct PushBlock {
					glm::mat4 mvp;
					// Sampling deltas
					float deltaPhi = (2.0f * float(M_PI)) / 180.0f;
					float deltaTheta = (0.5f * float(M_PI)) / 64.0f;
				} pushBlock;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_IRRADIANCE_CUBE_H
