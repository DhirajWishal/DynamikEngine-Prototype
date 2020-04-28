#pragma once
#ifndef _DYNAMIK_ADGR_PIPELINE_H
#define _DYNAMIK_ADGR_PIPELINE_H

#include "RenderPrimitive.h"

namespace Dynamik {
	namespace ADGR {
		class Pipeline : public RenderPrimitive {
		public:
			Pipeline() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_PIPELINE) {}
			virtual ~Pipeline() {}
		};
	}
}

#endif // !_DYNAMIK_ADGR_PIPELINE_H
