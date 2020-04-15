#include "cdhafx.h"
#include "vertex.h"

namespace Dynamik {
	ARRAY<VkVertexInputBindingDescription> Vertex::getBindingDescription(int bindCount) {
		ARRAY<VkVertexInputBindingDescription> bindingDescription(bindCount);

		for (int i = 0; i < bindCount; i++) {
			bindingDescription[i].binding = i;
			bindingDescription[i].stride = sizeof(Vertex);
			bindingDescription[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}

		return bindingDescription;
	}

	ARRAY<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions() {
		ARRAY<VkVertexInputAttributeDescription> attributeDescriptions;

		VkVertexInputAttributeDescription _description = {};
		_description.binding = 0;
		_description.location = 0;
		_description.format = VK_FORMAT_R32G32B32_SFLOAT;
		_description.offset = offsetof(Vertex, Position);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 1;
		_description.format = VK_FORMAT_R32G32B32_SFLOAT;
		_description.offset = offsetof(Vertex, Color);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 2;
		_description.format = VK_FORMAT_R32G32_SFLOAT;
		_description.offset = offsetof(Vertex, TexCoordinates);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 3;
		_description.format = VK_FORMAT_R32G32_SFLOAT;
		_description.offset = offsetof(Vertex, Normals);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 4;
		_description.format = VK_FORMAT_R32_SFLOAT;
		_description.offset = offsetof(Vertex, Integrity);
		attributeDescriptions.pushBack(_description);

		return attributeDescriptions;
	}

	ARRAY<VkVertexInputBindingDescription> Dynamik::vertex2D::getBindingDescription(int bindCount)
	{
		ARRAY<VkVertexInputBindingDescription> bindingDescription(bindCount);

		for (int i = 0; i < bindCount; i++) {
			bindingDescription[i].binding = i;
			bindingDescription[i].stride = sizeof(vertex2D);
			bindingDescription[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}

		return bindingDescription;
	}

	ARRAY<VkVertexInputAttributeDescription> vertex2D::getAttributeDescriptions()
	{
		ARRAY<VkVertexInputAttributeDescription> attributeDescriptions;

		VkVertexInputAttributeDescription _description = {};
		_description.binding = 0;
		_description.location = 0;
		_description.format = VK_FORMAT_R32G32_SFLOAT;
		_description.offset = offsetof(vertex2D, position);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 1;
		_description.format = VK_FORMAT_R32G32B32_SFLOAT;
		_description.offset = offsetof(vertex2D, color);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 2;
		_description.format = VK_FORMAT_R32G32_SFLOAT;
		_description.offset = offsetof(vertex2D, texCoordinates);
		attributeDescriptions.pushBack(_description);

		return attributeDescriptions;
	}

	ARRAY<VkVertexInputBindingDescription> VertexP::getBindingDescription(int bindCount)
	{
		ARRAY<VkVertexInputBindingDescription> bindingDescription(bindCount);

		for (int i = 0; i < bindCount; i++) {
			bindingDescription[i].binding = i;
			bindingDescription[i].stride = sizeof(VertexP);
			bindingDescription[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}

		return bindingDescription;
	}

	ARRAY<VkVertexInputAttributeDescription> VertexP::getAttributeDescriptions()
	{
		ARRAY<VkVertexInputAttributeDescription> attributeDescriptions;

		VkVertexInputAttributeDescription _description = {};
		_description.binding = 0;
		_description.location = 0;
		_description.format = VK_FORMAT_R32G32B32_SFLOAT;
		_description.offset = offsetof(VertexP, position);
		attributeDescriptions.pushBack(_description);

		return attributeDescriptions;
	}

	ARRAY<VkVertexInputBindingDescription> VertexPN::getBindingDescription(int bindCount)
	{
		ARRAY<VkVertexInputBindingDescription> bindingDescription(bindCount);

		for (int i = 0; i < bindCount; i++) {
			bindingDescription[i].binding = i;
			bindingDescription[i].stride = sizeof(VertexPN);
			bindingDescription[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}

		return bindingDescription;
	}

	ARRAY<VkVertexInputAttributeDescription> VertexPN::getAttributeDescriptions()
	{
		ARRAY<VkVertexInputAttributeDescription> attributeDescriptions;

		VkVertexInputAttributeDescription _description = {};
		_description.binding = 0;
		_description.location = 0;
		_description.format = VK_FORMAT_R32G32B32_SFLOAT;
		_description.offset = offsetof(VertexPN, position);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 1;
		_description.format = VK_FORMAT_R32G32B32_SFLOAT;
		_description.offset = offsetof(VertexPN, normal);
		attributeDescriptions.pushBack(_description);

		return attributeDescriptions;
	}

	ARRAY<VkVertexInputBindingDescription> VertexP2N2::getBindingDescription(int bindCount)
	{
		ARRAY<VkVertexInputBindingDescription> bindingDescription(bindCount);

		for (int i = 0; i < bindCount; i++) {
			bindingDescription[i].binding = i;
			bindingDescription[i].stride = sizeof(VertexP2N2);
			bindingDescription[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}

		return bindingDescription;
	}

	ARRAY<VkVertexInputAttributeDescription> VertexP2N2::getAttributeDescriptions()
	{
		ARRAY<VkVertexInputAttributeDescription> attributeDescriptions;

		VkVertexInputAttributeDescription _description = {};
		_description.binding = 0;
		_description.location = 0;
		_description.format = VK_FORMAT_R32G32_SFLOAT;
		_description.offset = offsetof(VertexP2N2, position);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 1;
		_description.format = VK_FORMAT_R32G32_SFLOAT;
		_description.offset = offsetof(VertexP2N2, normal);
		attributeDescriptions.pushBack(_description);

		return attributeDescriptions;
	}

	ARRAY<VkVertexInputBindingDescription> SkeletalVertex::getBindingDescription(int bindCount)
	{
		ARRAY<VkVertexInputBindingDescription> bindingDescription(bindCount);

		for (int i = 0; i < bindCount; i++) {
			bindingDescription[i].binding = i;
			bindingDescription[i].stride = sizeof(SkeletalVertex);
			bindingDescription[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}

		return bindingDescription;
	}

	ARRAY<VkVertexInputAttributeDescription> SkeletalVertex::getAttributeDescriptions()
	{
		ARRAY<VkVertexInputAttributeDescription> attributeDescriptions;

		VkVertexInputAttributeDescription _description = {};
		_description.binding = 0;
		_description.location = 0;
		_description.format = VK_FORMAT_R32G32B32_SFLOAT;
		_description.offset = offsetof(SkeletalVertex, Position);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 1;
		_description.format = VK_FORMAT_R32G32B32_SFLOAT;
		_description.offset = offsetof(SkeletalVertex, Normal);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 2;
		_description.format = VK_FORMAT_R32G32_SFLOAT;
		_description.offset = offsetof(SkeletalVertex, UV);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 3;
		_description.format = VK_FORMAT_R32G32_SFLOAT;
		_description.offset = offsetof(SkeletalVertex, Color);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 4;
		_description.format = VK_FORMAT_R32G32B32A32_SFLOAT;
		_description.offset = offsetof(SkeletalVertex, boneWeights);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 5;
		_description.format = VK_FORMAT_R32G32B32A32_SINT;
		_description.offset = offsetof(SkeletalVertex, boneIDs);
		attributeDescriptions.pushBack(_description);

		return attributeDescriptions;
	}

	ARRAY<VkVertexInputBindingDescription> VertexPNU::getBindingDescription(int bindCount)
	{
		ARRAY<VkVertexInputBindingDescription> bindingDescription(bindCount);

		for (int i = 0; i < bindCount; i++) {
			bindingDescription[i].binding = i;
			bindingDescription[i].stride = sizeof(VertexPNU);
			bindingDescription[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}

		return bindingDescription;
	}

	ARRAY<VkVertexInputAttributeDescription> VertexPNU::getAttributeDescriptions()
	{
		ARRAY<VkVertexInputAttributeDescription> attributeDescriptions;

		VkVertexInputAttributeDescription _description = {};
		_description.binding = 0;
		_description.location = 0;
		_description.format = VK_FORMAT_R32G32B32_SFLOAT;
		_description.offset = offsetof(VertexPNU, Position);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 1;
		_description.format = VK_FORMAT_R32G32B32_SFLOAT;
		_description.offset = offsetof(VertexPNU, Normal);
		attributeDescriptions.pushBack(_description);

		_description.binding = 0;
		_description.location = 2;
		_description.format = VK_FORMAT_R32G32_SFLOAT;
		_description.offset = offsetof(VertexPNU, UV);
		attributeDescriptions.pushBack(_description);

		return attributeDescriptions;
	}
	
	ARRAY<VkVertexInputBindingDescription> VertexL::getBindingDescription(int bindCount)
	{
		ARRAY<VkVertexInputBindingDescription> bindingDescription(bindCount);

		for (int i = 0; i < bindCount; i++) {
			bindingDescription[i].binding = i;
			bindingDescription[i].stride = sizeof(VertexPNU);
			bindingDescription[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}

		return bindingDescription;
	}
	
	ARRAY<VkVertexInputAttributeDescription> VertexL::getAttributeDescriptions()
	{
		ARRAY<VkVertexInputAttributeDescription> attributeDescriptions;

		VkVertexInputAttributeDescription _description = {};
		_description.binding = 0;
		_description.location = 0;
		_description.format = VK_FORMAT_R32G32B32_SFLOAT;
		_description.offset = offsetof(VertexPNU, Position);
		attributeDescriptions.pushBack(_description);

		return attributeDescriptions;
	}
}