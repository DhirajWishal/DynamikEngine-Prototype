#pragma once
#ifndef _DYNAMIK_ADGR_RENDER_PRIMITIVES_H
#define _DYNAMIK_ADGR_RENDER_PRIMITIVES_H

namespace Dynamik {
	namespace ADGR {
		enum class DMKRenderPrimitiveType {
			DMK_RENDER_PRIMITIVE_TYPE_UNKNOWN,
			DMK_RENDER_PRIMITIVE_TYPE_VERTEX_BUFFER,
			DMK_RENDER_PRIMITIVE_TYPE_INDEX_BUFFER,
			DMK_RENDER_PRIMITIVE_TYPE_TEXTURE_DATA,
			DMK_RENDER_PRIMITIVE_TYPE_UNIFORM_BUFFER,

			DMK_RENDER_PRIMITIVE_TYPE_FRAME_BUFFER,
			DMK_RENDER_PRIMITIVE_TYPE_RENDER_PASS,
			DMK_RENDER_PRIMITIVE_TYPE_SHADER,
			DMK_RENDER_PRIMITIVE_TYPE_PIPELINE,
			DMK_RENDER_PRIMITIVE_TYPE_PIPELINE_CACHE,
			DMK_RENDER_PRIMITIVE_TYPE_SWAPCHAIN,

			DMK_RENDER_PRIMITIVE_TYPE_OTHER,
		};

		/* BASE CLASS 
		 */
		class RenderPrimitive {
		public:
			RenderPrimitive(DMKRenderPrimitiveType type) : primitiveType(type) {}
			virtual ~RenderPrimitive() {}

			DMKRenderPrimitiveType primitiveType = DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_UNKNOWN;
		};
	}
}

#endif // !_DYNAMIK_ADGR_PRIMITIVES_H
