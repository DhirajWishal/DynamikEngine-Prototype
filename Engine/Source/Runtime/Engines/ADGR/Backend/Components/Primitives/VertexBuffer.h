#pragma once
#ifndef _DYNAMIK_ADGR_VERTEX_BUFFER_H
#define _DYNAMIK_ADGR_VERTEX_BUFFER_H

#include "RenderPrimitive.h"

namespace Dynamik {
	namespace ADGR {
		class VertexBuffer : public RenderPrimitive {
		public:
			VertexBuffer() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_VERTEX_BUFFER) {}
			virtual ~VertexBuffer() {}
		};
	}
}

#endif // !_DYNAMIK_ADGR_VERTEX_BUFFER_H
