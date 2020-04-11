#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_OBJECT3D_H
#define _DYNAMIK_ADGR_VULKAN_OBJECT3D_H

#include "Objects/VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanObject3D : public VulkanRenderableObject {
			public:
				VulkanObject3D(ADGRVulkanRenderableObjectInitInfo info) : VulkanRenderableObject(info) {}
				~VulkanObject3D() {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_OBJECT3D_H
