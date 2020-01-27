#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_ADGR_VULKAN_VALIDATORS_H
#define _DYNAMIK_ADGR_VULKAN_VALIDATORS_H
#ifdef DMK_USE_VULKAN

#include "debugger/debugger.h"
#include "CentralDataHub.h"
#include "backend/Interface.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			const std::vector<const char*> validationLayer = {
				"VK_LAYER_KHRONOS_validation",
				//"VK_LAYER_LUNARG_standard_validation",
				//"VK_LAYER_LUNARG_core_validation",
			};

#if defined(DMK_DEBUG)
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

#endif // DMK_USE_VULKAN

#endif	// !_DYNAMIK_ADGR_VULKAN_VALIDATORS_H
