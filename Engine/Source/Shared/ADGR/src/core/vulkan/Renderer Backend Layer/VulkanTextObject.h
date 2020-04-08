#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_TEXT_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_TEXT_OBJECT_H

#include "Objects/VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanTextObject : public VulkanRenderableObject {
			public:
				VulkanTextObject() {}
				~VulkanTextObject() {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_TEXT_OBJECT_H
