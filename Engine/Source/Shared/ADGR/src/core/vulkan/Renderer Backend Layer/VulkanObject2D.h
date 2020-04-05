#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_OBJECT_2D_H
#define _DYNAMIK_ADGR_VULKAN_OBJECT_2D_H

#include "Objects/VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkan2DObjectData {
				POINTER<ARRAY<ARRAY<Vertex>>> vertexBufferObjects;
				POINTER<ARRAY<ARRAY<UI32>>> indexBufferObjects;

				std::string vertexShaderPath = "";
				std::string tessellationShaderPath = "";
				std::string geometryShaderPath = "";
				std::string fragmentShaderPath = "";

				ARRAY<std::string> texturePaths;

				DMK_ADGR_RENDERING_TECHNOLOGY renderTechnology = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;
			};

			class VulkanObject2D : public VulkanRenderableObject {
			public:
				VulkanObject2D(
					VkDevice logicalDevice,
					VkPhysicalDevice physicalDevice,
					VkCommandPool commandPool,
					VkQueue graphicsQueue,
					VkQueue presentQueue)
					: VulkanRenderableObject(
						logicalDevice,
						physicalDevice, commandPool,
						graphicsQueue,
						presentQueue) {}
				~VulkanObject2D() {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_2D_OBJECT_H