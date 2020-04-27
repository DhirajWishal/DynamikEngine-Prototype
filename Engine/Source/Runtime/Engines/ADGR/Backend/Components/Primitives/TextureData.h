#pragma once
#ifndef _DYNAMIK_ADGR_TEXTURE_DATA_H
#define _DYNAMIK_ADGR_TEXTURE_DATA_H

#include "RenderPrimitive.h"

namespace Dynamik {
	namespace ADGR {
		class TextureData : public RenderPrimitive {
		public:
			TextureData() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_TEXTURE_DATA) {}
			virtual ~TextureData() {}
		};
	}
}

#endif // !_DYNAMIK_ADGR_TEXTURE_DATA_H
