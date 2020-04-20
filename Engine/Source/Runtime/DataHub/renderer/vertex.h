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
#include "Public/Array.h"

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

	struct VertexP {
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

		static ARRAY<VkVertexInputBindingDescription> getBindingDescription(int bindCount);
		static ARRAY<VkVertexInputAttributeDescription> getAttributeDescriptions();

		bool operator==(const VertexP& other) const {
			return position == other.position;
		}
	};

	struct VertexPN {
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);

		static ARRAY<VkVertexInputBindingDescription> getBindingDescription(int bindCount);
		static ARRAY<VkVertexInputAttributeDescription> getAttributeDescriptions();

		bool operator==(const VertexPN& other) const {
			return position == other.position
				&& normal == other.normal;
		}
	};

	struct VertexP2N2 {
		glm::vec2 position;
		glm::vec2 normal;

		static ARRAY<VkVertexInputBindingDescription> getBindingDescription(int bindCount);
		static ARRAY<VkVertexInputAttributeDescription> getAttributeDescriptions();

		bool operator==(const VertexP2N2& other) const {
			return position == other.position
				&& normal == other.normal;
		}
	};

	struct SkeletalVertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;
		glm::vec3 Color;
		float boneWeights[4];
		UI32 boneIDs[4];

		static ARRAY<VkVertexInputBindingDescription> getBindingDescription(int bindCount);
		static ARRAY<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};

	struct VertexPNU {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;

		static ARRAY<VkVertexInputBindingDescription> getBindingDescription(int bindCount);
		static ARRAY<VkVertexInputAttributeDescription> getAttributeDescriptions();

		B1 operator==(const VertexPNU& other)
		{
			return Position == other.Position &&
				Normal == other.Normal &&
				UV == other.UV;
		}
	};

	enum class DMKVertexAttributeType {
		DMK_VERTEX_ATTRIBUTE_TYPE_F32,		// float
		DMK_VERTEX_ATTRIBUTE_TYPE_VEC2,		// glm::vec2
		DMK_VERTEX_ATTRIBUTE_TYPE_VEC3,		// glm::vec3
		DMK_VERTEX_ATTRIBUTE_TYPE_MAT3,		// glm::mat3
		DMK_VERTEX_ATTRIBUTE_TYPE_MAT4,		// glm::mat4
	};

	struct DMKVertexAttribute {
		std::string attributeName = "";
		DMKVertexAttributeType attributeType = DMKVertexAttributeType::DMK_VERTEX_ATTRIBUTE_TYPE_F32;
		UI32 bindingLocation = 0;
	};

	class DMKVertex {
	public:
		DMKVertex() {}
		virtual ~DMKVertex() {}

		void addAttribute(const DMKVertexAttribute& attribute);
		void updateAttribute(const DMKVertexAttribute attribute, UI32 attributeIndex);
		ARRAY<DMKVertexAttribute> getAttributes();

	protected:
		ARRAY<DMKVertexAttribute> attributes;
	};

	class InternalVertexInterpretation : public DMKVertex {
	public:
		InternalVertexInterpretation() {}
		~InternalVertexInterpretation() {}
	};
}

#endif // !_DYNAMIK_ADGR_VULKAN_VERTEX_H
