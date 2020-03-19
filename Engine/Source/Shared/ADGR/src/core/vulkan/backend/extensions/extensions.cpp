/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#if defined(DMK_USE_VULKAN)

#include "extensions.h"

#include "core/vulkan/backend/controlHeader.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			ARRAY<CCPTR> getRequiredExtentions(B1 vaidationLayer) {
				UI32 glfwExtentionCount = 0;
				CCPTR* glfwExtentions;
				glfwExtentions = glfwGetRequiredInstanceExtensions(&glfwExtentionCount);

				std::vector<CCPTR> extentions(glfwExtentions, glfwExtentions + glfwExtentionCount);

				if (pushDescriptorsSupported)
					extentions.push_back(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME);

				if (checkpointsSupported)
					extentions.push_back(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);

				if (meshShadingSupported)
					extentions.push_back(VK_NV_MESH_SHADER_EXTENSION_NAME);

				if (vaidationLayer)
					extentions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

				return extentions;
			}
		}
	}
}

#endif