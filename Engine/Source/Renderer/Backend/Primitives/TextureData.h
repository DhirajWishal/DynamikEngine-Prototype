#pragma once
#ifndef _DYNAMIK_RENDERER_TEXTURE_DATA_H
#define _DYNAMIK_RENDERER_TEXTURE_DATA_H

#include "RenderPrimitive.h"
#include "Objects/Mesh/Mesh.h"

namespace Dynamik {
	namespace Renderer {
		class TextureData : public RenderPrimitive {
		public:
			TextureData() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_TEXTURE_DATA) {}
			virtual ~TextureData() {}

			DMKFormat format = DMKFormat::DMK_FORMAT_RGB_8_UNIFORM;
		};
	}
}

#endif // !_DYNAMIK_RENDERER_TEXTURE_DATA_H
