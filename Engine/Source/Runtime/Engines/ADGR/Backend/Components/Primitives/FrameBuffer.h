#pragma once
#ifndef _DYNAMIK_ADGR_FRAME_BUFFER_H
#define _DYNAMIK_ADGR_FRAME_BUFFER_H

#include "RenderPrimitive.h"

namespace Dynamik {
	namespace ADGR {
		class FrameBuffer : public RenderPrimitive {
		public:
			FrameBuffer() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_FRAME_BUFFER) {}
			virtual ~FrameBuffer() {}
		};
	}
}

#endif // !_DYNAMIK_ADGR_FRAME_BUFFER_H
