#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_REFLECT_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_REFLECT_OBJECT_H

#include "Graphics/VulkanGraphicsRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanReflectObject : public VulkanGraphicsRenderableObject {
			public:
				VulkanReflectObject(ADGRVulkanGraphicsRenderableObjectInitInfo info);
				~VulkanReflectObject() {}

				ADGRVulkanRenderData initializeObject(POINTER<InternalFormat> _object, VkSampleCountFlagBits msaaSamples) override;

				void initializeUniformBuffer() override;
				void updateUniformBuffer(UBO_MVPC uniformBufferObject, UI32 currentImage);

				void initializeDescriptorSets() override;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_REFLECT_OBJECT_H
