#pragma once

#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class Pipeline : public Core {
			public:
				Pipeline() {}
				virtual ~Pipeline() {}

				virtual void initRenderPass(ADGRVulkanDataContainer* container) {}
			};
		}
	}
}