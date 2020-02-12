#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_FORMAT_H
#define _DYNAMIK_ADGR_VULKAN_FORMAT_H
#ifdef DMK_USE_VULKAN

#include "rendererFormat.h"
#include "backend/vulkan/core/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class vulkanFormat {
			public:
				vulkanFormat(RendererFormat* format) : myRendererFormat(format) {}
				~vulkanFormat() {}

				// local renderer format
				RendererFormat* myRendererFormat = nullptr;

				void initVertexAndIndexBuffers(I32 size) {
					myVertexBuffers.resize(size);
					myVertexBufferMemories.resize(size);
					myRendererFormat->myInternalFormat->myVertexCounts.resize(size);
					myIndexBuffers.resize(size);
					myIndexBufferMemories.resize(size);
					myRendererFormat->myInternalFormat->myIndexCounts.resize(size);
				}

				void initTexturesAndDescriptors(I32 size) {
					myTextureImages.resize(size);
					myTextureImageMemories.resize(size);
					myTextureImageViews.resize(size);
					myTextureImageSamplers.resize(size);
					myDescriptorPools.resize(size);
					myDescriptorSets.resize(size);
				}

				// Shaders and Pipelines
				ADGRVulkanShaderDataContainer myShaders;
				VkRenderPass myRenderPass = VK_NULL_HANDLE;
				VkPipeline myPipeline = VK_NULL_HANDLE;
				VkPipelineLayout myPipelineLayout = VK_NULL_HANDLE;

				// Textures
				UI32 myMipLevel = 1.0f;
				std::vector<VkImage> myTextureImages = {};
				std::vector<VkDeviceMemory> myTextureImageMemories = {};
				std::vector<VkSampler> myTextureImageSamplers = {};
				std::vector<VkImageView> myTextureImageViews = {};
				DMKFormat myTextureFormat = DMKFormat::DMK_FORMAT_R8G8B8A8_UNORM;

				// Vertex buffers
				std::vector<VkBuffer> myVertexBuffers = {};
				std::vector<VkDeviceMemory> myVertexBufferMemories = {};

				// Index buffers
				std::vector<VkBuffer> myIndexBuffers = {};
				std::vector<VkDeviceMemory> myIndexBufferMemories = {};

				// Descriptor Sets
				VkDescriptorSetLayout myDescriptorSetLayout = VK_NULL_HANDLE;
				std::vector<VkDescriptorPool> myDescriptorPools = {};
				std::vector<std::vector<VkDescriptorSet>> myDescriptorSets = {};

				// Uniform buffers
				std::vector<VkBuffer> myUniformBuffers = {};
				std::vector<VkDeviceMemory> myUniformBufferMemories = {};

				B1 isInitialized = false;
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_FORMAT_H
