#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_REFLECT_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_REFLECT_OBJECT_H

#include "Objects/VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanReflectObject : public VulkanRenderableObject {
			public:
				VulkanReflectObject(ADGRVulkanRenderableObjectInitInfo info)
					: VulkanRenderableObject(info) {}
				~VulkanReflectObject() {}

				void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos) override;

				//void initializeUniformBuffer() override;
				//void updateUniformBuffer(UBO_MPL uniformBufferObject, UI32 currentImage);

				void initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info) override;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_REFLECT_OBJECT_H
