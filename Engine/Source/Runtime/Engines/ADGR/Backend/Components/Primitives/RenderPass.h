#pragma once
#ifndef _DYNAMIK_ADGR_RENDER_PASS_H
#define _DYNAMIK_ADGR_RENDER_PASS_H

#include "RenderPrimitive.h"

namespace Dynamik {
	namespace ADGR {
		class RenderPass : public RenderPrimitive {
		public:
			RenderPass() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_RENDER_PASS) {}
			virtual ~RenderPass() {}
		};
	}
}

#endif // !_DYNAMIK_ADGR_RENDER_PASS_H
