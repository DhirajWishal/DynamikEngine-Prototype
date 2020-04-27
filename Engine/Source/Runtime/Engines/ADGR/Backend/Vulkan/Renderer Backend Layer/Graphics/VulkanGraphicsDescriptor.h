#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_DESCRIPTOR_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_DESCRIPTOR_H

#include "../Common/VulkanDescriptor.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanGraphicsDescriptor : public VulkanDescriptor {
			public:
				VulkanGraphicsDescriptor() {}
				virtual ~VulkanGraphicsDescriptor() {}

				VkDescriptorSetLayout layout = VK_NULL_HANDLE;
				ARRAY<VkDescriptorPool> pools;
				ARRAY<VkDescriptorSet> sets;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_GRAPHICS_DESCRIPTOR_H
