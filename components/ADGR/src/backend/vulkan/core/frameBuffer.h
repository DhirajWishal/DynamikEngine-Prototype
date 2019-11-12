#pragma once

#include "core/Buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct DMKFrameBuffersCreateInfo {
				std::vector<VkFramebuffer>* frameBuffers;
				std::vector<VkImageView> swapChainImageViews;
				VkImageView colorImageView;
				VkImageView depthImageView;
			};

			class frameBufferManager : public Buffer {
			public:
				frameBufferManager() {}
				~frameBufferManager() {}

				void init(ADGRVulkanDataContainer* container) override;
				void createFrameBuffers(ADGRVulkanDataContainer* container, DMKFrameBuffersCreateInfo info);

				void clear(ADGRVulkanDataContainer* container);

			private:
				int mySize = 0;
			};
		}
	}
}
