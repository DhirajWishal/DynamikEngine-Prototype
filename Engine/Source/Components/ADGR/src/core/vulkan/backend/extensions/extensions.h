#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_ADGR_VULKAN_EXTENSIONS_H
#define _DYNAMIK_ADGR_VULKAN_EXTENSIONS_H
#if defined(DMK_USE_VULKAN)

#include "CentralDataHub.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			std::vector<const char*> getRequiredExtentions(B1);
		}
	}
}

#endif 
#endif // !_DYNAMIK_ADGR_VULKAN_EXTENSIONS_H
