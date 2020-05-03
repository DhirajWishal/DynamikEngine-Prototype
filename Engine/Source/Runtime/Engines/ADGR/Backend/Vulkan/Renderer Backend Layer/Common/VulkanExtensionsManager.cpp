#include "dmkafx.h"
#include "VulkanExtensionsManager.h"
#include "VulkanControlHeader.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			ARRAY<CCPTR> VulkanExtensionsManager::getRequiredExtentions(B1 validationLayer)
			{
				UI32 glfwExtentionCount = 0;
				const char** glfwExtentions;
				glfwExtentions = glfwGetRequiredInstanceExtensions(&glfwExtentionCount);

				std::vector<CCPTR> extentions(glfwExtentions, glfwExtentions + glfwExtentionCount);

				if (pushDescriptorsSupported)
					extentions.push_back(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME);

				if (checkpointsSupported)
					extentions.push_back(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);

				if (meshShadingSupported)
					extentions.push_back(VK_NV_MESH_SHADER_EXTENSION_NAME);

				if (validationLayer)
					extentions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

				return extentions;
			}
		}
	}
}