#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_PIPELINE_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_PIPELINE_MANAGER_H
#ifdef DMK_USE_VULKAN

#include "vulkan/backend/data structures/vulkan.h"
#include "vulkanFormat.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			// TODO: Pipeline cashe

			class pipelineManager {
			public:
				pipelineManager() {}
				~pipelineManager() {}

				std::pair<VkPipeline, VkPipelineLayout> init(ADGRVulkanDataContainer* container, DMKPipelineInitInfo info);
				void initRenderPass(ADGRVulkanDataContainer* container);

				void clear(ADGRVulkanDataContainer* container, vulkanFormat* format);
				void destroyRenderPass(ADGRVulkanDataContainer* container);
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif	// !_DYNAMIK_ADGR_VULKAN_PIPELINE_H
