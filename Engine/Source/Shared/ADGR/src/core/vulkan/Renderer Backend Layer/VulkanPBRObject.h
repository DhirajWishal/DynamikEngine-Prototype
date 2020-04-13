#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_PBR_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_PBR_OBJECT_H

#include "Objects/VulkanRenderableObject.h"
#include "Objects/VulkanPushConstant.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanPBRObject : public VulkanRenderableObject {
			public:
				VulkanPBRObject(ADGRVulkanRenderableObjectInitInfo initInfo);
				virtual ~VulkanPBRObject() {}

				virtual ADGRVulkanRenderData initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples) override;

				void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos) override;

				void initializeUniformBuffer() override;
				void updateUniformBuffer(UBO_MVPC uniformBufferObject, UI32 currentImage);

				void initializeDescriptorPool(ADGRVulkanDescriptorPoolInitInfo info) override;
				void initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info) override;

			private:
				class _VertexPushConstant : public VulkanPushConstant {
				public:
					_VertexPushConstant();
					~_VertexPushConstant() {}
				} vertPushConstant;

				class _FragmentPushConstant : public VulkanPushConstant {
				public:
					_FragmentPushConstant();
					~_FragmentPushConstant() {}
				} fragPushConstant;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_PBR_OBJECT_H
