#pragma once
#ifndef _DYNAMIK_RENDERER_BRDF_H
#define _DYNAMIK_RENDERER_BRDF_H

#include "../RenderAttachment.h"

namespace Dynamik {
	namespace Renderer {
		class BRDF : public RenderAttachment {
		public:
			BRDF() {}
			~BRDF() {}
		};
	}
}

#endif // !_DYNAMIK_RENDERER_BRDF_H
