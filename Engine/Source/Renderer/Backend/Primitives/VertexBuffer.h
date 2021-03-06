#pragma once
#ifndef _DYNAMIK_RENDERER_VERTEX_BUFFER_H
#define _DYNAMIK_RENDERER_VERTEX_BUFFER_H

#include "RenderPrimitive.h"

namespace Dynamik {
	namespace Renderer {
		class VertexBuffer : public RenderPrimitive {
		public:
			VertexBuffer() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_VERTEX_BUFFER) {}
			virtual ~VertexBuffer() {}

			UI32 vertexCount = 0;
		};
	}
}

#endif // !_DYNAMIK_RENDERER_VERTEX_BUFFER_H
