#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_PIPELINE_H
#define _DYNAMIK_ADGR_VULKAN_PIPELINE_H

#include "core/Pipeline.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			// TODO: Pipeline cashe

			class pipelineManager : public Pipeline {
			public:
				pipelineManager() {}
				~pipelineManager() {}

				std::pair<VkPipeline, VkPipelineLayout> init(ADGRVulkanDataContainer* container, DMKPipelineInitInfo info);
				void initRenderPass(ADGRVulkanDataContainer* container) override;

			private:
			};
		}
	}
}

#endif	// !_DYNAMIK_ADGR_VULKAN_PIPELINE_H
