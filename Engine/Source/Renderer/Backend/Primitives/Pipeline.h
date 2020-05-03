#pragma once
#ifndef _DYNAMIK_RENDERER_PIPELINE_H
#define _DYNAMIK_RENDERER_PIPELINE_H

#include "RenderPrimitive.h"

namespace Dynamik {
	namespace Renderer {
		class Pipeline : public RenderPrimitive {
		public:
			Pipeline() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_PIPELINE) {}
			virtual ~Pipeline() {}
		};
	}
}

#endif // !_DYNAMIK_RENDERER_PIPELINE_H
