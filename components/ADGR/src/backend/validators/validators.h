#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "debugger/debugger.h"
#include "core/utils/DMK_DataTypes.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			const std::vector<const char*> validationLayer = {
				"VK_LAYER_LUNARG_standard_validation",
				"VK_LAYER_LUNARG_core_validation",
				"VK_LAYER_KHRONOS_validation"
			};

#ifdef DMK_DEBUG
			const bool enableValidationLayers = true;

			// ----------
#else
			const bool enableValidationLayers = false;

			// ----------
#endif

			bool checkValidationLayerSupport();
			void destroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks*);
		}
	}
}
