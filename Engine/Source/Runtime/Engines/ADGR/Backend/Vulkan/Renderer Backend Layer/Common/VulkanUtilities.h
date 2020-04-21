#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_UTILITIES_H
#define _DYNAMIK_ADGR_VULKAN_UTILITIES_H

#include <vulkan/vulkan.h>

#include "Public/Array.h"
#include "GameObjectDescriptors.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanUtilities {
				VulkanUtilities(const VulkanUtilities&) = delete;
				VulkanUtilities(VulkanUtilities&&) = delete;
			public:
				~VulkanUtilities() {}

				static ARRAY<VkVertexInputBindingDescription> getBindingDescription(ARRAY<DMKVertexAttribute> attributes, UI32 bindCount = 1);
				static ARRAY<VkVertexInputAttributeDescription> getAttributeDescriptions(ARRAY<DMKVertexAttribute> attributes, UI32 binding = 1);

				static void updateUniformBuffer(VkDevice device, ARRAY<MAT4> uniformData, VkDeviceMemory uniformBufferMemory);

				static VkFormat vertexAttributeTypeToVkFormat(DMKDataType type);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_UTILITIES_H
