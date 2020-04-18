#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_TEXT_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_TEXT_OBJECT_H

#include "Graphics/VulkanGraphicsRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanTextObject : public VulkanGraphicsRenderableObject {
			public:
				VulkanTextObject() {}
				~VulkanTextObject() {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_TEXT_OBJECT_H
