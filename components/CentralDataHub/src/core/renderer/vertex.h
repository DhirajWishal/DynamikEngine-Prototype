#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_VERTEX_H
#define _DYNAMIK_ADGR_VULKAN_VERTEX_H

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
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec2 TexCoordinates;

		static std::vector<VkVertexInputBindingDescription> getBindingDescription(int bindCount);
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(int bindCount);

		bool operator==(const Vertex& other) const {
			return Position == other.Position && Color == other.Color && TexCoordinates == other.TexCoordinates;
		}
	};

	struct PointVertex {
		glm::vec3 Position;
		glm::vec3 Color;

		static std::vector<VkVertexInputBindingDescription> getBindingDescription(int bindCount);
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(int bindCount);

		bool operator==(const Vertex& other) const {
			return Position == other.Position && Color == other.Color;
		}
	};
}

#endif // !_DYNAMIK_ADGR_VULKAN_VERTEX_H