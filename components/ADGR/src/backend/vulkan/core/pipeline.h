#pragma once

#ifndef _DMK_ADGR_VULKAN_PIPELINE_H
#define _DMK_ADGR_VULKAN_PIPELINE_H

#include "core/Pipeline.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			// TODO: Pipeline cashe

			class pipeline : public Pipeline {
			public:
				pipeline() {}
				~pipeline() {}

				void init(ADGRVulkanDataContainer* container, DMKPipelineInitInfo info);
				void initRenderPass(ADGRVulkanDataContainer* container) override;

			private:
			};
		}
	}
}

#endif
