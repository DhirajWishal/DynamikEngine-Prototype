#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_SWAP_CHAIN_3D_H
#define _DYNAMIK_RENDERER_VULKAN_SWAP_CHAIN_3D_H

#include "Graphics/VulkanGraphicsSwapChain.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			class VulkanGraphicsSwapChain3D {
			public:
				VulkanGraphicsSwapChain3D() {}
				~VulkanGraphicsSwapChain3D() {}

				void basicInitialize(VkDevice device, VkPhysicalDevice physicalDevice, VulkanSurfaceContainer container);
				void initializeSwapChain(UI32 width, UI32 height);

				void terminate();

				VulkanGraphicsSwapChain swapChainContainer;

			private:
				VkPhysicalDevice myPhysicalDevice = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_SWAP_CHAIN_3D_H
