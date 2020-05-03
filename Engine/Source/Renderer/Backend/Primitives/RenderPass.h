#pragma once
#ifndef _DYNAMIK_RENDERER_RENDER_PASS_H
#define _DYNAMIK_RENDERER_RENDER_PASS_H

#include "RenderPrimitive.h"

namespace Dynamik {
	namespace Renderer {
		class RenderPass : public RenderPrimitive {
		public:
			RenderPass() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_RENDER_PASS) {}
			virtual ~RenderPass() {}
		};
	}
}

#endif // !_DYNAMIK_RENDERER_RENDER_PASS_H
