#pragma once

#include "core/Buffer.h"

#include "data structures/DMK_ADGR_DataStructures.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKBindCommandBufferInfo {
				std::vector<VkFramebuffer> frameBuffers = {};
				std::vector<std::vector<VkDescriptorSet>*> descriptorSets = {};

				std::vector<VkBuffer> indexBuffers = {};

				std::vector<VkBuffer> vertexBuffers = {};
				std::vector<std::vector<uint32_t>> indices = {};

				DMKVulkanSkyboxCommandBufferBindData skybox;
			};

			class commandBufferManager : public Buffer {
			public:
				commandBufferManager() {}
				~commandBufferManager() {}

				void createBuffer(VkBuffer* buffer, size_t size,
					VkBufferUsageFlags flags, VkBufferUsageFlagBits memoryFlags) override;
				void initCommandPool(ADGRVulkanDataContainer* container);

				void bindCommands(ADGRVulkanDataContainer* container, DMKBindCommandBufferInfo info);
			};
		}
	}
}
