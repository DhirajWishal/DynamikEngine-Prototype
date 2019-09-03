#pragma once

namespace Dynamik {
	namespace ADGR {
		namespace core {

			/* FUNCTION DESCRIPTOR STRUCTS */
			// naming convention -> (DMK + <class name> + <function name> + <type>)

			enum DMKVulkanRendererCreateBufferType {
				VERTEX_BUFFER,
				INDEX_BUFFER
			};

			struct DMKVulkanRendererLoadObjectInfo {
				std::string path = "";
				std::vector<float> offsets = { 0.0f, 0.0f, 0.0f };

				std::vector<core::Vertex>* vertexBufferObject = {};
				std::vector<uint32_t>* indexBufferObject = {};
			};

			struct DMKVulkanRendererCreateVertexBufferInfo {
				VkBuffer* buffer = VK_NULL_HANDLE;
				VkDeviceMemory* bufferMemory = VK_NULL_HANDLE;

				std::vector<Vertex> vertexBufferObject = {};
			};

			struct DMKVulkanRendererCreateIndexBufferInfo {
				VkBuffer* buffer = VK_NULL_HANDLE;
				VkDeviceMemory* bufferMemory = VK_NULL_HANDLE;

				std::vector<uint32_t> indexBufferObject = {};
			};

			struct DMKVulkanRendererCreateUniformBufferInfo {
				std::vector<VkBuffer>* buffer = {};
				std::vector<VkDeviceMemory>* bufferMemory = {};
			};

			struct DMKVulkanRendereCreateDescriptorSetsInfo {
				std::vector<VkBuffer>* uniformBuffers = {};
				VkImageView textureImageView = VK_NULL_HANDLE;
				VkSampler textureSampler = VK_NULL_HANDLE;

				std::vector<VkDescriptorSet>* descriptorSets = {};
				VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
				VkDescriptorSetLayout* layout = VK_NULL_HANDLE;
				std::vector<uint8_t> bindIndexes = { 0, 1 };
			};


			/* COMMON VULKAN DATA STRUCTURES */
			// naming convention -> (DMK + <API> + <structure>)

			struct DMKVulkanBuffer {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			struct DMKVulkanTexture {
				VkImage texture = VK_NULL_HANDLE;
				VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
			};

			struct DMKVulkanVertexBuffer {
				std::vector<Vertex> vertexBuffers = {};

				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			struct DMKVulkanIndexBuffer {
				std::vector<uint32_t> indexBuffers = {};

				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			struct DMKVulkanSkyboxCommandBufferBindData {
				VkBuffer vertexBuffer = VK_NULL_HANDLE;
				VkBuffer indexBuffer = VK_NULL_HANDLE;
				std::vector<uint32_t> indexes = {};

				VkPipeline pipeline = VK_NULL_HANDLE;
				VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
			};

			struct DMKVulkanSkyboxContainer {
				std::string path = "";

				VkImage skyboxImage;
				VkImageLayout skyboxLayout;
				VkSampler skyboxSampler;

				VkFormat skyboxFormat;
				VkDeviceMemory skyboxMemory;

				uint32_t mipLevels;

				VkBuffer vertexBuffer;
				VkBuffer indexBuffer;
				std::vector<uint32_t> indexes;

				VkDescriptorSet descriptorSet;
				VkDescriptorPool descriptorPool;
				VkPipeline pipeline;
			};
		}
	}
}