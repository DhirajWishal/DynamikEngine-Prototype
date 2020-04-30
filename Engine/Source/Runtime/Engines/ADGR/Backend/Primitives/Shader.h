#pragma once
#ifndef _DYNAMIK_ADGR_SHADER_H
#define _DYNAMIK_ADGR_SHADER_H

#include "RenderPrimitive.h"
#include "Dynamik/objectSpecifications.h"

namespace Dynamik {
	namespace ADGR {
		class Shader : public RenderPrimitive {
		public:
			Shader() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_SHADER) {}
			Shader(DMKShaderLocation loc) :
				RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_SHADER),
				location(loc) {}
			virtual ~Shader() {}

			DMKShaderLocation location = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX;
		};
	}
}

#endif // !_DYNAMIK_ADGR_SHADER_H
