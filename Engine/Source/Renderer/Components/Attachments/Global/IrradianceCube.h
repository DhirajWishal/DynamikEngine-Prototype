#pragma once
#ifndef _DYNAMIK_RENDERER_IRRADIANCE_CUBE_H
#define _DYNAMIK_RENDERER_IRRADIANCE_CUBE_H

#include "../RenderAttachment.h"

namespace Dynamik {
	namespace Renderer {
		class IrradianceCube : public RenderAttachment {
		public:
			IrradianceCube() {}
			~IrradianceCube() {}
		};
	}
}

#endif // !_DYNAMIK_RENDERER_IRRADIANCE_CUBE_H
