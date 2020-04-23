#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_BIDIRECTIONAL_REFLECTANCE_DESTRIBUTION_FUNCTION_H
#define _DYNAMIK_ADGR_VULKAN_BIDIRECTIONAL_REFLECTANCE_DESTRIBUTION_FUNCTION_H

#include "../Graphics/VulkanGraphicsSupportObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanBRDF : public VulkanGraphicsSupportObject {
			public:
				VulkanBRDF() {}
				VulkanBRDF(ADGRVulkanGraphicsSupportObjectInitInfo info) :
					VulkanGraphicsSupportObject(info) {}
				 ~VulkanBRDF() {}

				 void initialize() override;
				 void terminate() override;

				UI32 dimentions = 512;

			protected:
				void _initializeTexture() override;
				void _initializeRenderPass() override;
				void _initializeFrameBuffer() override;
				void _initializeDescriptorSetLayout() override;
				void _initializeDescriptorPool() override;
				void _initializeDescriptorSets() override;
				void _initializePipelineLayout() override;
				void _initializePipeline() override;
				void _initializeCommandBuffers() override;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_BIDIRECTIONAL_REFLECTANCE_DESTRIBUTION_FUNCTION_H
