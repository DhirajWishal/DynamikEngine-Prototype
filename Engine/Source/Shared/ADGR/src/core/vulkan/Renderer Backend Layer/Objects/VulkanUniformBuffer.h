#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H

#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "UniformBufferObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanUniformBuffer {
			public:
				VulkanUniformBuffer() {}
				~VulkanUniformBuffer() {}

				void initialize(VulkanDevice device, VulkanSwapChain swapChain);
				void update(VulkanDevice device, UniformBufferObject uniformBuferObject, UI32 currentImage);
				void terminate(VulkanDevice device);

				ARRAY<VkBuffer> buffers;
				ARRAY<VkDeviceMemory> bufferMemories;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H
