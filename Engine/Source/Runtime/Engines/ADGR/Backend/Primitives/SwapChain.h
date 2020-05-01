#pragma once
#ifndef _DYNAMIK_ADGR_SWAP_CHAIN_H
#define _DYNAMIK_ADGR_SWAP_CHAIN_H

#include "RenderPrimitive.h"

namespace Dynamik {
	namespace ADGR {
		class SwapChain : public RenderPrimitive {
		public:
			SwapChain() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_SWAPCHAIN) {}
			virtual ~SwapChain() {}

			virtual void initialize(UI32 width, UI32 height) {}
			virtual void terminate() {}

			struct Extent2D
			{
				UI32 width = 0;
				UI32 height = 0;
			} extent;
		};
	}
}

#endif // !_DYNAMIK_ADGR_SWAP_CHAIN_H
