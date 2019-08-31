#pragma once

namespace Dynamik {
	namespace ADGR {
		namespace core {

			enum DMKVulkanRendererCreateBufferType {
				VERTEX_BUFFER,
				INDEX_BUFFER
			};

			struct DMKVulkanRendererLoadObjectInfo {
				std::string path;
				std::vector<float> offsets = { 0.0f, 0.0f, 0.0f };

				std::vector<core::Vertex>* vertexBufferObject;
				std::vector<uint32_t>* indexBufferObject;
			};

			struct DMKVulkanRendererCreateVertexBufferInfo {
				VkBuffer* buffer;
				VkDeviceMemory* bufferMemory;

				std::vector<Vertex> vertexBufferObject;
			};

			struct DMKVulkanRendererCreateIndexBufferInfo {
				VkBuffer* buffer;
				VkDeviceMemory* bufferMemory;

				std::vector<uint32_t> indexBufferObject;
			};

			struct DMKVulkanRendererCreateUniformBufferInfo {
				std::vector<VkBuffer>* buffer;
				std::vector<VkDeviceMemory>* bufferMemory;
			};

			struct DMKVulkanRendereCreateDescriptorSetsInfo {
				std::vector<VkBuffer>* uniformBuffers;
				VkImageView textureImageView;
				VkSampler textureSampler;

				std::vector<VkDescriptorSet>* descriptorSets;
				VkDescriptorPool descriptorPool;
				VkDescriptorSetLayout* layout;
				std::vector<uint8_t> bindIndexes = { 0, 1 };
			};
		}
	}
}