#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_REFLECT_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_REFLECT_OBJECT_H

#include "Objects/VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanReflectObject : public VulkanRenderableObject {
			public:
				VulkanReflectObject(ADGRVulkanRenderableObjectInitInfo info);
				~VulkanReflectObject() {}

				ADGRVulkanRenderData initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples) override;

				void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos) override;

				void initializeUniformBuffer() override;
				void updateUniformBuffer(UBO_MVPC uniformBufferObject, UI32 currentImage);

				void initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info) override;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_REFLECT_OBJECT_H
