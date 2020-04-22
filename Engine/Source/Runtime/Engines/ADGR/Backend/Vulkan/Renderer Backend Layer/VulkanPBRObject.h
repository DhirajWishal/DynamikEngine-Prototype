#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_PBR_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_PBR_OBJECT_H

#include "Graphics/VulkanGraphicsRenderableObject.h"
#include "Graphics/VulkanGraphicsPushConstant.h"

#include "VulkanIrradianceCube.h"
#include "VulkanPrefilteredCube.h"
#include "VulkanBRDF.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanPBRObject : public VulkanGraphicsRenderableObject {
			public:
				VulkanPBRObject(ADGRVulkanGraphicsRenderableObjectInitInfo initInfo);
				virtual ~VulkanPBRObject() {}

				virtual ADGRVulkanRenderData initializeObject(VkDevice logicalDevice, POINTER<InternalFormat> _object, VkSampleCountFlagBits msaaSamples) override;

				void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos) override;

				void initializeUniformBuffer() override;
				void updateUniformBuffer(UBO_MVPC uniformBufferObject, UI32 currentImage);

				void initializeDescriptorPool() override;
				void initializeDescriptorSets() override;

				VulkanBRDF myBRDF;
				VulkanIrradianceCube myIrradianceCube;
				VulkanPrefilteredCube myPreFilteredCube;

			private:
				class _VertexPushConstant : public VulkanGraphicsPushConstant {
				public:
					_VertexPushConstant();
					~_VertexPushConstant() {}
				} vertPushConstant;

				class _FragmentPushConstant : public VulkanGraphicsPushConstant {
				public:
					_FragmentPushConstant();
					~_FragmentPushConstant() {}
				} fragPushConstant;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_PBR_OBJECT_H
