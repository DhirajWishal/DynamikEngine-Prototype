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

				void init() override;
				void createFrameBuffers(DMKFrameBuffersCreateInfo info);

			private:
				VkDevice* m_device = &device;

				std::vector<VkFramebuffer>* m_frameBuffers = &frameBuffers;
				uint32_t mySize = 0;
			};
		}
	}
}
