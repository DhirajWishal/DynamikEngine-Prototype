#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_SKY_BOX_H
#define _DYNAMIK_ADGR_VULKAN_SKY_BOX_H

#include "Objects/VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanSkyBox : public VulkanRenderableObject {
			public:
				VulkanSkyBox(ADGRVulkanRenderableObjectInitInfo info);
				~VulkanSkyBox() {}

				void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos) override;

				void initializeUniformBuffer() override;
				void updateUniformBuffer(UniformBufferObject uniformBufferObject, UI32 currentImage);

				void initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info) override;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SKY_BOX_H
