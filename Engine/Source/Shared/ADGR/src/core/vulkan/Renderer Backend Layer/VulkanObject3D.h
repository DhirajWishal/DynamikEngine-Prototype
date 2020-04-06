#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_OBJECT3D_H
#define _DYNAMIK_ADGR_VULKAN_OBJECT3D_H

#include "Objects/VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkan3DObjectData {
				DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT;

				POINTER<ARRAY<ARRAY<Vertex>>> vertexBufferObjects;
				POINTER<ARRAY<ARRAY<UI32>>> indexBufferObjects;

				std::string vertexShaderPath = "";
				std::string tessellationShaderPath = "";
				std::string geometryShaderPath = "";
				std::string fragmentShaderPath = "";

				ARRAY<std::string> texturePaths;

				DMK_ADGR_RENDERING_TECHNOLOGY renderTechnology = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;
			};

			class VulkanObject3D : public VulkanRenderableObject {
			public:
				VulkanObject3D(ADGRVulkanRenderableObjectInitInfo info) : VulkanRenderableObject(info) {}
				~VulkanObject3D() {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_OBJECT3D_H
