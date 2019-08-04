/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "extensions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			std::vector<const char*> getRequiredExtentions(bool vaidationLayer) {
				uint32 glfwExtentionCount = 0;
				const char** glfwExtentions;
				glfwExtentions = glfwGetRequiredInstanceExtensions(&glfwExtentionCount);

				std::vector<const char*> extentions(glfwExtentions, glfwExtentions + glfwExtentionCount);

				if (vaidationLayer)
					extentions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

				return extentions;
			}
		}
	}
}
