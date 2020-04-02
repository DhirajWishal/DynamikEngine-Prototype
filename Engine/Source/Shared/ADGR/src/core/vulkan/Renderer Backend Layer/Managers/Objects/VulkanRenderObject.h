#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RENDER_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_RENDER_OBJECT_H

#include "vulkan/vulkan.h"
#include "CentralDataHub.h"

#include "core/vulkan/Renderer Backend Layer/Managers/VulkanBufferFunctions.h"
#include "core/vulkan/Renderer Backend Layer/Managers/VulkanCoreFunctions.h"
#include "core/vulkan/Renderer Backend Layer/Managers/VulkanTextureFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanVertexBufferContainer;
			struct ADGRVulkanIndexBufferContainer;
			struct ADGRVulkanTextureContainer;

			class VulkanRenderObject {
			public:
				VulkanRenderObject() {}
				~VulkanRenderObject() {}

				DMK_ADGR_RENDERING_TECHNOLOGY renderTechnology = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;

				VkRenderPass* renderPass = nullptr;

				VkPipeline pipeline = VK_NULL_HANDLE;
				VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

				ARRAY<ADGRVulkanVertexBufferContainer> vertexBuffers;
				ARRAY<ADGRVulkanIndexBufferContainer> indexBuffers;
				ARRAY<ADGRVulkanTextureContainer> textures;
				ARRAY<VkPushConstantRange> pushConstants;

				ARRAY<VkBuffer> uniformBuffers;
				ARRAY<VkDeviceMemory> uniformBufferMemories;
				
				VkDescriptorSetLayout descriptorLayout;
				ARRAY<VkDescriptorPool> descriptorPools;
				ARRAY<ARRAY<VkDescriptorSet>> descriptorSets;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDER_OBJECTS_CONTAINER_H
