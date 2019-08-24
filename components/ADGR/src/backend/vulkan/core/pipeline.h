#pragma once

#include "core/Pipeline.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class pipeline : public Pipeline {
			public:
				pipeline() {}
				~pipeline() {}

				void init() override;
				void initRenderPass() override;

			private:
				VkDevice* m_device = &device;
				VkPipeline* m_pipeline = &graphicsPipeline;
				VkPipelineLayout* m_pipelineLayout = &pipelineLayout;
			};
		}
	}
}
