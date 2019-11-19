#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H

#include "core/Buffer.h"

#include "data structures/DMK_ADGR_DataStructures.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct ADGRObjectRenderData {
				DMK_ADGR_RENDERING_TECHNOLOGY renderingType = DMK_ADGR_VULKAN_RENDERER_INDEXED;

				std::vector<Vertex> vertexBufferObject = {};
				std::vector<VkBuffer> vertexBuffers = {};
				VkDeviceMemory vertexBufferMemory = {};

				std::vector<uint32_t> indexBufferObject = {};
				VkBuffer indexBuffer = {};
				VkDeviceMemory indexBufferMemory = {};

				uint32_t indexCount = 0;
				uint32_t vertexCount = 0;

				std::vector<std::string> texturePaths = {};
				std::vector<VkImage> textureImages = {};
				std::vector<VkDeviceMemory> textureImageMemories = {};
				std::vector<VkSampler> textureSamplers = {};
				std::vector<VkImageView> textureImageViews = {};
				std::vector<VkDescriptorSet> descriptorSets = {};
				uint32_t mipLevel = 1;

				VkPipeline pipeline = {};
				VkPipelineLayout pipelineLayout = {};
			};

			struct DMKBindCommandBufferInfo {
				std::vector<VkFramebuffer> frameBuffers = {};

				std::vector<ADGRObjectRenderData> objectBindDatas = {};

				DMKVulkanSkyboxCommandBufferBindData skybox;
			};

			class commandBufferManager : public Buffer {
			public:
				commandBufferManager() {}
				~commandBufferManager() {}

				void initCommandPool(ADGRVulkanDataContainer* container);

				void bindCommands(ADGRVulkanDataContainer* container, DMKBindCommandBufferInfo info);
			};
		}
	}
}

#endif	// !_DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
