#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_ADGR_VULKAN_VALIDATORS_H
#define _DYNAMIK_ADGR_VULKAN_VALIDATORS_H
#ifdef DMK_USE_VULKAN

#include "debugger/debugger.h"
#include "CentralDataHub.h"
#include "Interface.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			const ARRAY<const char*> validationLayer = {
				"VK_LAYER_KHRONOS_validation",
				//"VK_LAYER_LUNARG_api_dump",
				//"VK_LAYER_LUNARG_screenshot"
			};

#if defined(DMK_DEBUG)
			const B1 enableValidationLayers = true;

			// ----------
#else
			const B1 enableValidationLayers = false;

			// ----------
#endif

			B1 checkValidationLayerSupport();
			void destroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks*);
		}
	}
}

#endif // DMK_USE_VULKAN

#endif	// !_DYNAMIK_ADGR_VULKAN_VALIDATORS_H
