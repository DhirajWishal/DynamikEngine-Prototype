#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_IMAGE_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_IMAGE_OBJECT_H

#include "VulkanComputeObject.h"
#include "../Graphics/VulkanGraphicsRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanComputeImageObject : public VulkanComputeObject {
			public:
				VulkanComputeImageObject() {}
				VulkanComputeImageObject(ADGRVulkanComputeObjectInitInfo info) : VulkanComputeObject(info) {}
				virtual ~VulkanComputeImageObject() {}

				virtual void initialize(ADGRVulkanComputeImageInputContainer container) override;

			protected:
				void _initializeTexture(ADGRVulkanComputeImageInputContainer container);

				virtual void _initializeDescriptorSetLayout() override;
				virtual void _initializePipelineLayout() override;
				virtual void _initializePipeline() override;
				virtual void _initializeDescriptorPool() override;
				virtual void _initializeDescriptorSets() override;

				ADGRVulkanTextureContainer myInputTextureContainer;
				ADGRVulkanTextureContainer myOutputTextureContainer;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_IMAGE_OBJECT_H
