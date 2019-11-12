#pragma once

#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct swapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities = {};
				std::vector<VkSurfaceFormatKHR> formats = {};
				std::vector<VkPresentModeKHR> presentModes = {};
			};

			class SwapChain : public Core {
			public:
				SwapChain() {}
				virtual ~SwapChain() {}

				virtual void initImageViews(ADGRVulkanDataContainer* container) {}

			protected:
			};
		}
	}
}
