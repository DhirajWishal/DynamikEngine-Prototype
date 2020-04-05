#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_SWAP_CHAIN_3D_H
#define _DYNAMIK_ADGR_VULKAN_SWAP_CHAIN_3D_H

#include "Objects/VulkanSwapChain.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanSwapChain3D {
			public:
				VulkanSwapChain3D() {}
				~VulkanSwapChain3D() {}

				void basicInitialize(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR surfaceCapabilities);
				void initializeSwapChain(UI32 width, UI32 height);
				void initializeRenderPass(VkSampleCountFlagBits msaaSamples);
				void initializeFrameBuffers(ADGRVulkanFrameBufferInitInfo frameBufferInitInfo);
				void initializeDescriptorSetLayout(ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo);
				void initializePipelineLayout(ADGRVulkanPipelineLayoutInitInfo pipelineLayoutInitInfo);

				void terminate();

				VulkanSwapChain swapChainContainer;

			private:
				VkPhysicalDevice myPhysicalDevice = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SWAP_CHAIN_3D_H
