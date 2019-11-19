#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_SWAPCHAIN_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_SWAPCHAIN_H

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

#endif // !_DYNAMIK_ADGR_VULKAN_RENDERER_SWAPCHAIN_H
