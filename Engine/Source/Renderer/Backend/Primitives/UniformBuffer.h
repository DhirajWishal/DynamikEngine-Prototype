#pragma once
#ifndef _DYNAMIK_RENDERER_UNIFORM_BUFFER_H
#define _DYNAMIK_RENDERER_UNIFORM_BUFFER_H

#include "RenderPrimitive.h"

namespace Dynamik {
	namespace Renderer {
		class UniformBuffer : public RenderPrimitive {
		public:
			UniformBuffer() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_UNIFORM_BUFFER) {}
			virtual ~UniformBuffer() {}
		};
	}
}

#endif // !_DYNAMIK_RENDERER_UNIFORM_BUFFER_H
