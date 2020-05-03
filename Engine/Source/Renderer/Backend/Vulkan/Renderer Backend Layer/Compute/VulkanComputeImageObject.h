#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_COMPUTE_IMAGE_OBJECT_H
#define _DYNAMIK_RENDERER_VULKAN_COMPUTE_IMAGE_OBJECT_H

#include "VulkanComputeObject.h"
#include "../Graphics/VulkanGraphicsContainers.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			class VulkanComputeImageObject : public VulkanComputeObject {
			public:
				VulkanComputeImageObject() {}
				VulkanComputeImageObject(VulkanComputeObjectInitInfo info) : VulkanComputeObject(info) {}
				virtual ~VulkanComputeImageObject() {}

				virtual void initialize(VulkanComputeImageInputContainer container) override;

			protected:
				void _initializeTexture(VulkanComputeImageInputContainer container);

				virtual void _initializeDescriptorSetLayout() override;
				virtual void _initializePipelineLayout() override;
				virtual void _initializePipeline() override;
				virtual void _initializeDescriptorPool() override;
				virtual void _initializeDescriptorSets() override;

				VulkanTextureContainer myInputTextureContainer;
				VulkanTextureContainer myOutputTextureContainer;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_COMPUTE_IMAGE_OBJECT_H
