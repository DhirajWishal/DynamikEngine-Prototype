#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_PIPELINE_H
#define _DYNAMIK_ADGR_VULKAN_PIPELINE_H

#include "backend/vulkan/core/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			// TODO: Pipeline cashe

			class pipelineManager {
			public:
				pipelineManager() {}
				~pipelineManager() {}

				std::pair<VkPipeline, VkPipelineLayout> init(ADGRVulkanDataContainer* container, DMKPipelineInitInfo info);
				void initRenderPass(ADGRVulkanDataContainer* container) ;
			};
		}
	}
}

#endif	// !_DYNAMIK_ADGR_VULKAN_PIPELINE_H
