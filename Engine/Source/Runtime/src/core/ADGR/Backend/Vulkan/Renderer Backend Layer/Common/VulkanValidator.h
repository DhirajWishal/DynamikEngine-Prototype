#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_VALIDATOR_H
#define _DYNAMIK_ADGR_VULKAN_VALIDATOR_H

#include <vulkan/vulkan.h>
#include "DataTypesLib/DataTypesLib.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanValidator {
			public:
				VulkanValidator() {}
				~VulkanValidator() {}

				static ARRAY<const char*> validationLayer;

#if defined(DMK_DEBUG)
				static const B1 enableValidationLayers = true;

				// ----------
#else
				static const B1 enableValidationLayers = false;

				// ----------
#endif

				static B1 checkValidationLayerSupport();
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_VALIDATOR_H
