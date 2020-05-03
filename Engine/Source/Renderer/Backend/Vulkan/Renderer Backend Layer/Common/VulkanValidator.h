#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_VALIDATOR_H
#define _DYNAMIK_RENDERER_VULKAN_VALIDATOR_H

#include <vulkan/vulkan.h>
#include "DataTypesLib.h"

namespace Dynamik {
	namespace Renderer {
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

#endif // !_DYNAMIK_RENDERER_VULKAN_VALIDATOR_H
