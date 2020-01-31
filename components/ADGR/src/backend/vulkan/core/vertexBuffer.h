#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H

#include "backend/vulkan/core/data structures/vulkan.h"
#include "vulkanFormat.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		namespace core {
			class vertexBufferManager {
			public:
				vertexBufferManager() {}
				~vertexBufferManager() {}

				void createVertexBuffer(ADGRVulkanDataContainer* container, vulkanFormat* format);
				void clear(ADGRVulkanDataContainer* container, vulkanFormat* format);
			};
		}
	}
}
#endif // DMK_USE_VULKAN

#endif	// !_DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H
