#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_DATA_STRUCTURES_H
#define _DYNAMIK_ADGR_VULKAN_DATA_STRUCTURES_H

#include "CentralDataHub.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			/* FUNCTION DESCRIPTOR STRUCTS */
			// naming convention -> (DMK + <class name> + <function name> + <type>)

			/* COMMON VULKAN DATA STRUCTURES */
			// naming convention -> (DMK + <API> + <structure>)

			struct DMKObjectData {
				std::string path = "";
				std::vector<float> offsets = { 0.0f, 0.0f, 0.0f };

				std::vector<Vertex>* vertexBufferObject = {};
				std::vector<VkBuffer>* vertexBuffers = {};
				std::vector<VkDeviceMemory>* vertexBufferMemories = {};

				std::vector<uint32_t>* indexBufferObject = {};
				VkBuffer* indexBuffer = VK_NULL_HANDLE;
				VkDeviceMemory* indexBufferMemory = VK_NULL_HANDLE;

				uint32_t* vertexCount = 0;
				uint32_t* indexCount = 0;
			};

			enum DMKVulkanRendererCreateBufferType {
				VERTEX_BUFFER,
				INDEX_BUFFER
			};

			struct DMKVulkanRendererLoadObjectInfo {
				DMKObjectData objectDataContainer = {};
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

			struct DMKVulkanBuffer {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			struct DMKVulkanTexture {
				VkImage texture = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
				VkSampler sampler = VK_NULL_HANDLE;

				uint32_t mipLevels = 1;
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

				VkImage skyboxImage = VK_NULL_HANDLE;
				VkImageLayout skyboxLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				VkSampler skyboxSampler = VK_NULL_HANDLE;

				VkFormat skyboxFormat = VK_FORMAT_UNDEFINED;
				VkDeviceMemory skyboxMemory = VK_NULL_HANDLE;

				uint32_t mipLevels = NULL;

				DMKVulkanSkyboxCommandBufferBindData commandBufferBindData = {};

				VkPipeline pipeline = VK_NULL_HANDLE;
			};
		}
	}
}

#endif	// !_DYNAMIK_ADGR_VULKAN_DATA_STRUCTURES_H
