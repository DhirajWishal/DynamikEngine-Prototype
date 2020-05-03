#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_COMPUTE_DESCRIPTOR_H
#define _DYNAMIK_RENDERER_VULKAN_COMPUTE_DESCRIPTOR_H

#include "../Common/VulkanDescriptor.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			class VulkanComputeDescriptor : public VulkanDescriptor {
			public:
				VulkanComputeDescriptor() {}
				virtual ~VulkanComputeDescriptor() {}
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_COMPUTE_DESCRIPTOR_H
