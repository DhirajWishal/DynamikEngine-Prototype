#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_MANAGER_H

#include "VulkanComputeCore.h"
#include "VulkanComputePipeline.h"
#include "VulkanComputeDescriptor.h"
#include "VulkanComputeObject.h"
#include "VulkanComputeCommandBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanComputeManager {
			public:
				VulkanComputeManager() {}
				virtual ~VulkanComputeManager() {}

				void initialize();
				void compute();
				void terminate();

			private:
				VulkanComputeCore myComputeCore;
				VulkanComputeCommandBuffer myComputeCommandBuffer;
				ARRAY<ADGRVulkanComputeData> myComputeDatas;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_MANAGER_H
