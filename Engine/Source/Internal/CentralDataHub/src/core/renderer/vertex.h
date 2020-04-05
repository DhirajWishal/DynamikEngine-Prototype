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
#include "DataTypesLib/Public/Array.h"

namespace Dynamik {
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec2 TexCoordinates;
		glm::vec3 Normals = glm::vec3(0.0f, 0.0f, 0.0f);
		float Integrity = 1.0f;

		static ARRAY<VkVertexInputBindingDescription> getBindingDescription(int bindCount);
		static ARRAY<VkVertexInputAttributeDescription> getAttributeDescriptions();

		bool operator==(const Vertex& other) const {
			return Position == other.Position
				&& Color == other.Color
				&& TexCoordinates == other.TexCoordinates
				&& Normals == other.Normals
				&& Integrity == other.Integrity;
		}
	};

	struct vertex2D {
		glm::vec2 position = glm::vec2(0.0f, 0.0f);
		glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec2 texCoordinates = glm::vec2(0.0f, 0.0f);

		static ARRAY<VkVertexInputBindingDescription> getBindingDescription(int bindCount);
		static ARRAY<VkVertexInputAttributeDescription> getAttributeDescriptions();

		bool operator==(const vertex2D& other) const {
			return position == other.position
				&& color == other.color
				&& texCoordinates == other.texCoordinates;
		}
	};

	class DMKVertex {
	public:
		DMKVertex() {}
		virtual ~DMKVertex() {}

		glm::vec3 Position = { 0, 0, 0 };

		virtual ARRAY<VkVertexInputBindingDescription> getBindingDescription(int bindCount)
		{
			return ARRAY<VkVertexInputBindingDescription>();
		}
		virtual ARRAY<VkVertexInputAttributeDescription> getAttributeDescriptions()
		{
			return ARRAY<VkVertexInputAttributeDescription>();
		}

		virtual bool operator==(const DMKVertex* other) const { return false; }
	};

	class VertexL3C3T2N3I : public DMKVertex {
	public:
		VertexL3C3T2N3I() {}
		~VertexL3C3T2N3I() {}

		glm::vec3 Color = { 0, 0, 0 };
		glm::vec3 Normals = { 0, 0, 0 };
		float Integrity = 1.0f;

		ARRAY<VkVertexInputBindingDescription> getBindingDescription(int bindCount) override;
		ARRAY<VkVertexInputAttributeDescription> getAttributeDescriptions() override;

		bool operator==(const DMKVertex* other) const override;
	};
}

#endif // !_DYNAMIK_ADGR_VULKAN_VERTEX_H
