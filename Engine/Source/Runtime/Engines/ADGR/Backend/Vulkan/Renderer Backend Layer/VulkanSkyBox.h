#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_SKY_BOX_H
#define _DYNAMIK_ADGR_VULKAN_SKY_BOX_H

#include "Graphics/VulkanGraphicsRenderableObject.h"

#include "VulkanBRDF.h"
#include "VulkanIrradianceCube.h"
#include "VulkanPrefilteredCube.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanSkyBox : public VulkanGraphicsRenderableObject {
			public:
				VulkanSkyBox(ADGRVulkanGraphicsRenderableObjectInitInfo info);
				~VulkanSkyBox() {}

				ADGRVulkanRenderData initializeObject(VkDevice logicalDevice, POINTER<InternalFormat> _object, VkSampleCountFlagBits msaaSamples) override;

				void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos) override;

				void initializeUniformBuffer() override;
				void updateUniformBuffer(UniformBufferObject uniformBufferObject, UI32 currentImage);

				void initializeDescriptorSets() override;

				VulkanBRDF myBRDF;
				VulkanIrradianceCube myIrradianceCube;
				VulkanPrefilteredCube myPreFilteredCube;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SKY_BOX_H
