#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_SHADER_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_SHADER_H

#include "VulkanCore.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanComputeShader {
			public:
				VulkanComputeShader() {}
				virtual ~VulkanComputeShader() {}

				void setResources(VulkanCore coreObject);

				void initialize();
				void compute();
				void terminate();

				VulkanCore myCoreObject;
				VulkanCommandBuffer myCommandBuffer;
				VulkanPipeline myPipeline;
				VulkanRenderableObject myRenderableObject;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_SHADER_H
