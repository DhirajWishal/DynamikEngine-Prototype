#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_PIPELINE_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_PIPELINE_H

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

#endif // !_DYNAMIK_ADGR_VULKAN_RENDERER_PIPELINE_H
