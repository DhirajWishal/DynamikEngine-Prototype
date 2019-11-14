#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H

#include "core/Buffer.h"

#include "data structures/DMK_ADGR_DataStructures.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct ADGRObjectRenderData {
				std::vector<VkBuffer> vertexBuffers = {};
				VkBuffer indexBuffer = VK_NULL_HANDLE;

				uint32_t indexCount = 0;

				std::vector<VkDescriptorSet> descriptorSets = {};

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
