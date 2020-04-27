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
		};
	}
}

#endif // !_DYNAMIK_ADGR_SWAP_CHAIN_H
