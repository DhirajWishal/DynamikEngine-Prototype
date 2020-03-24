#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_PUSH_CONSTANT_H
#define _DYNAMIK_ADGR_VULKAN_PUSH_CONSTANT_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class PushConstant {
			public:
				PushConstant() {}
				PushConstant(VkShaderStageFlagBits flag) : myShaderStageFlags(flag) {}
				virtual ~PushConstant() {}

				VkShaderStageFlagBits getMyStageFlag() { return myShaderStageFlags; }
				glm::vec4 myVecData;

			protected:
				VkShaderStageFlagBits myShaderStageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_ALL;
			};

			class vertexShaderPushConstant : public PushConstant {
			public:
				vertexShaderPushConstant() : PushConstant(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT) {}
				~vertexShaderPushConstant() {}

			};

			class fragmentShaderPushConstant : public PushConstant {
			public:
				fragmentShaderPushConstant() : PushConstant(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT) {}
				~fragmentShaderPushConstant() {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_PUSH_CONSTANT_H
