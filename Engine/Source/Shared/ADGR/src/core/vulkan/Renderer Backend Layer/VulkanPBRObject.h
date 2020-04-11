#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_PBR_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_PBR_OBJECT_H

#include "Objects/VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct Material {
				struct PushBlock {
					float roughness;
					float metallic;
					float r, g, b;
				} params;
				std::string name;
				Material() {}
				Material(std::string n, glm::vec3 c, float r, float m) : name(n) {
					params.roughness = r;
					params.metallic = m;
					params.r = c.r;
					params.g = c.g;
					params.b = c.b;
				}
				~Material() {}
			};

			class VulkanPBRObject : public VulkanRenderableObject {
			public:
				VulkanPBRObject(ADGRVulkanRenderableObjectInitInfo initInfo);
				virtual ~VulkanPBRObject() {}

				Material myMaterial;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_PBR_OBJECT_H
