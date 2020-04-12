#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_PBR_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_PBR_OBJECT_H

#include "Objects/VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanPBRObject : public VulkanRenderableObject {
			public:
				VulkanPBRObject(ADGRVulkanRenderableObjectInitInfo initInfo);
				virtual ~VulkanPBRObject() {}

				virtual ADGRVulkanRenderData initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples) override;

				void initializeUniformBuffer() override;
				void updateUniformBuffer(UBO_MVPC uniformBufferObject, UI32 currentImage);

				void initializeDescriptorPool(ADGRVulkanDescriptorPoolInitInfo info) override;
				void initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info) override;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_PBR_OBJECT_H
