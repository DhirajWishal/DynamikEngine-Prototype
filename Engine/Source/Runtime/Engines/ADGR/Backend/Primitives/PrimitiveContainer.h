#pragma once
#ifndef _DYNAMIK_ADGR_PRIMITIVE_CONTAINER_H
#define _DYNAMIK_ADGR_PRIMITIVE_CONTAINER_H

#include "FrameBuffer.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "SwapChain.h"
#include "TextureData.h"
#include "UniformBuffer.h"
#include "VertexBuffer.h"

namespace Dynamik {
	namespace ADGR {
		struct RenderContext {
			SwapChain swapChain;
			RenderPass renderPass;
			ARRAY<FrameBuffer> frameBuffers;
		};
	}
}

#endif // !_DYNAMIK_ADGR_PRIMITIVE_CONTAINER_H
