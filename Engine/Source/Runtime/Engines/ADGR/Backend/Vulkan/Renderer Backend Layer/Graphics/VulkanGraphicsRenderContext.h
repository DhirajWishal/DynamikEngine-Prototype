#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_RENDER_CONTEXT_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_RENDER_CONTEXT_H

#include "VulkanGraphicsSwapChain.h"
#include "VulkanGraphicsFrameBuffer.h"
#include "VulkanGraphicsCommandBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanGraphicsRenderContext {
			public:
				VulkanGraphicsRenderContext() {}
				~VulkanGraphicsRenderContext() {}

				VkSurfaceKHR surface = VK_NULL_HANDLE;
				VulkanGraphicsSwapChain swapChain;
				VulkanGraphicsFrameBuffer frameBuffer;

				VulkanGraphicsCommandBuffer inFlightCommandBuffer;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDER_CONTEXT_H
