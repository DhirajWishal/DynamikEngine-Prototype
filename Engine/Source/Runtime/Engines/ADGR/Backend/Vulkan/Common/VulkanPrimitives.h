#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_PRIMITIVES_H
#define _DYNAMIK_ADGR_VULKAN_PRIMITIVES_H

#include "../../Primitives/FrameBuffer.h"
#include "../../Primitives/IndexBuffer.h"
#include "../../Primitives/Pipeline.h"
#include "../../Primitives/PipelineCache.h"
#include "../../Primitives/RenderPass.h"
#include "../../Primitives/Shader.h"
#include "../../Primitives/SwapChain.h"
#include "../../Primitives/TextureData.h"
#include "../../Primitives/UniformBuffer.h"
#include "../../Primitives/VertexBuffer.h"

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanFrameBuffer : public FrameBuffer {
			public:
				VulkanFrameBuffer() {}
				~VulkanFrameBuffer() {}

				VkFramebuffer buffer = VK_NULL_HANDLE;
			};

			class VulkanIndexBuffer : public IndexBuffer {
			public:
				VulkanIndexBuffer() {}
				VulkanIndexBuffer(DMKDataType type) : IndexBuffer(type) {}
				~VulkanIndexBuffer() {}

				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			class VulkanPipeline : public Pipeline {
			public:
				VulkanPipeline() {}
				~VulkanPipeline() {}

				VkPipelineLayout layout = VK_NULL_HANDLE;
				VkPipeline pipeline = VK_NULL_HANDLE;
			};

			class VulkanPipelineCache : public PipelineCache {
			public:
				VulkanPipelineCache() {}
				~VulkanPipelineCache() {}

				VkPipelineCache cache = VK_NULL_HANDLE;
			};

			class VulkanRenderPass : public RenderPass {
			public:
				VulkanRenderPass() {}
				~VulkanRenderPass() {}

				VkRenderPass renderPass = VK_NULL_HANDLE;
			};

			class VulkanShader : public Shader {
			public:
				VulkanShader() {}
				VulkanShader(DMKShaderLocation loc) : Shader(loc) {}
				~VulkanShader() {}

				VkShaderModule shaderModule = VK_NULL_HANDLE;
				VkPipelineShaderStageCreateInfo shaderStage = {};
			};

			class VulkanSwapChain : public SwapChain {
			public:
				VulkanSwapChain() {}
				~VulkanSwapChain() {}

				VkSwapchainKHR swapChain = VK_NULL_HANDLE;
				ARRAY<VkImage> swapChainImages;
				ARRAY<VkImageView> swapChainImageViews;
				VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
				VkExtent2D swapChainExtent = { 0, 0 };
			};

			class VulkanTextureData : public TextureData {
			public:
				VulkanTextureData() {}
				~VulkanTextureData() {}

				VkImage image = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkSampler imageSampler = VK_NULL_HANDLE;
				VkFormat format = VK_FORMAT_UNDEFINED;
				UI32 mipLevels = 1;

				UI32 width = 0, height = 0;
			};

			class VulkanUniformBuffer : public UniformBuffer {
			public:
				VulkanUniformBuffer() {}
				~VulkanUniformBuffer() {}

				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			class VulkanVertexBuffer : public VertexBuffer {
			public:
				VulkanVertexBuffer() {}
				~VulkanVertexBuffer() {}

				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_PRIMITIVES_H
