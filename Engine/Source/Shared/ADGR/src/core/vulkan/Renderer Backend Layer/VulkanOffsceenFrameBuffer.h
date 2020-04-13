#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_OFFSCREEN_FRAME_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_OFFSCREEN_FRAME_BUFFER_H

#include "Objects/VulkanFrameBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanOffscreenFrameBuffer : public VulkanFrameBuffer {
			public:
				VulkanOffscreenFrameBuffer() {}
				~VulkanOffscreenFrameBuffer() {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_OFFSCREEN_FRAME_BUFFER_H
