#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_EXTENSION_H
#define _DYNAMIK_ADGR_VULKAN_EXTENSION_H

#include "DataTypesLib/Public/Array.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanExtensions {
			public:
				VulkanExtensions() {}
				~VulkanExtensions() {}

				static ARRAY<CCPTR> getRequiredExtentions(B1);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_EXTENSION_H
