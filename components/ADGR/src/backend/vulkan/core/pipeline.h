#pragma once

#include "core/Pipeline.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKPipelineInitInfo {
				std::vector<VkDescriptorSetLayout_T*> layouts;
			};

			class ADGR_API pipeline : public Pipeline {
			public:
				pipeline() {}
				~pipeline() {}

				void init(DMKPipelineInitInfo info);
				void initRenderPass() override;

			private:
				VkDevice* m_device = &device;
				VkPipeline* m_pipeline = &graphicsPipeline;
				VkPipelineLayout* m_pipelineLayout = &pipelineLayout;
			};
		}
	}
}
