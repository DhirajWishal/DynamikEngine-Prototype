#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_FORMAT_H
#define _DYNAMIK_ADGR_VULKAN_FORMAT_H
#ifdef DMK_USE_VULKAN

#include "rendererFormat.h"
#include "core/vulkan/backend/data structures/vulkan.h"

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
				ARRAY<VkImage> myTextureImages = {};
				ARRAY<VkDeviceMemory> myTextureImageMemories = {};
				ARRAY<VkSampler> myTextureImageSamplers = {};
				ARRAY<VkImageView> myTextureImageViews = {};
				DMKFormat myTextureFormat = DMKFormat::DMK_FORMAT_R8G8B8A8_UNORM;

				// Vertex buffers
				ARRAY<VkBuffer> myVertexBuffers = {};
				ARRAY<VkDeviceMemory> myVertexBufferMemories = {};

				// Index buffers
				ARRAY<VkBuffer> myIndexBuffers = {};
				ARRAY<VkDeviceMemory> myIndexBufferMemories = {};

				// Descriptor Sets
				VkDescriptorSetLayout myDescriptorSetLayout = VK_NULL_HANDLE;
				ARRAY<VkDescriptorPool> myDescriptorPools = {};
				ARRAY<ARRAY<VkDescriptorSet>> myDescriptorSets = {};

				// Uniform buffers
				ARRAY<VkBuffer> myUniformBuffers = {};
				ARRAY<VkDeviceMemory> myUniformBufferMemories = {};

				B1 isInitialized = false;
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_FORMAT_H
