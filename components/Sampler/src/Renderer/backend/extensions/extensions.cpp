/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "smpafx.h"
#include "extensions.h"

#include "Renderer/backend/controlHeader.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			std::vector<const char*> getRequiredExtentions(bool vaidationLayer) {
				uint32 glfwExtentionCount = 0;
				const char** glfwExtentions;
				glfwExtentions = glfwGetRequiredInstanceExtensions(&glfwExtentionCount);

				std::vector<const char*> extentions(glfwExtentions, glfwExtentions + glfwExtentionCount);

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
