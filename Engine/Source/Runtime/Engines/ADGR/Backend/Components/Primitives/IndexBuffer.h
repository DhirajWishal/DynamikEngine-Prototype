#pragma once
#ifndef _DYNAMIK_ADGR_INDEX_BUFFER_H
#define _DYNAMIK_ADGR_INDEX_BUFFER_H

#include "RenderPrimitive.h"

namespace Dynamik {
	namespace ADGR {
		class IndexBuffer : public RenderPrimitive {
		public:
			IndexBuffer() :RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_INDEX_BUFFER) {}
			virtual ~IndexBuffer() {}
		};
	}
}

#endif // !_DYNAMIK_ADGR_INDEX_BUFFER_H
