#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_DESCRIPTOR_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_DESCRIPTOR_H

#include "../Common/VulkanDescriptor.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanComputeDescriptor : public VulkanDescriptor {
			public:
				VulkanComputeDescriptor() {}
				virtual ~VulkanComputeDescriptor() {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_DESCRIPTOR_H
