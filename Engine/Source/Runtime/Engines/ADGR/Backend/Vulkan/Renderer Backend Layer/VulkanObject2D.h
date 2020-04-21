#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_OBJECT_2D_H
#define _DYNAMIK_ADGR_VULKAN_OBJECT_2D_H

#include "Graphics/VulkanGraphicsRenderableObject.h"

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

				DMKRenderingTechnology renderTechnology = DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_INDEXED;
			};

			class VulkanObject2D : public VulkanGraphicsRenderableObject {
			public:
				VulkanObject2D(ADGRVulkanGraphicsRenderableObjectInitInfo info) : VulkanGraphicsRenderableObject(info) {}
				~VulkanObject2D() {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_2D_OBJECT_H
