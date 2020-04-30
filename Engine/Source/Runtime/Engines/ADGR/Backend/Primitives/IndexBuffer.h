#pragma once
#ifndef _DYNAMIK_ADGR_INDEX_BUFFER_H
#define _DYNAMIK_ADGR_INDEX_BUFFER_H

#include "RenderPrimitive.h"
#include "Dynamik/DMKDataType.h"

namespace Dynamik {
	namespace ADGR {
		class IndexBuffer : public RenderPrimitive {
		public:
			IndexBuffer() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_INDEX_BUFFER) {}
			IndexBuffer(DMKDataType type) :
				RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_INDEX_BUFFER),
				indexType(type) {}
			virtual ~IndexBuffer() {}

			DMKDataType indexType = DMKDataType::DMK_DATA_TYPE_UI32;
			UI32 indexCount = 0;
		};
	}
}

#endif // !_DYNAMIK_ADGR_INDEX_BUFFER_H
