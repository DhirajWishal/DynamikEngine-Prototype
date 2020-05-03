#pragma once
#ifndef _DYNAMIK_RENDERER_PIPELINE_CACHE_H
#define _DYNAMIK_RENDERER_PIPELINE_CACHE_H

#include "RenderPrimitive.h"

namespace Dynamik {
	namespace Renderer {
		class PipelineCache : public RenderPrimitive {
		public:
			PipelineCache() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_PIPELINE_CACHE) {}
			virtual ~PipelineCache() {}
		};
	}
}

#endif // !_DYNAMIK_RENDERER_PIPELINE_CACHE_H
