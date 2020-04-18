#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_OBJECT3D_H
#define _DYNAMIK_ADGR_VULKAN_OBJECT3D_H

#include "Graphics/VulkanGraphicsRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanObject3D : public VulkanGraphicsRenderableObject {
			public:
				VulkanObject3D(ADGRVulkanGraphicsRenderableObjectInitInfo info) : VulkanGraphicsRenderableObject(info) {}
				~VulkanObject3D() {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_OBJECT3D_H
