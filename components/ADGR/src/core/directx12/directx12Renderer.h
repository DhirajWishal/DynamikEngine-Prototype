#pragma once

#ifndef _DYNAMIK_ADGR_DIRECT_X_12_RENDERER_H
#define _DYNAMIK_ADGR_DIRECT_X_12_RENDERER_H

#include "core/base/RendererBackendBase.h"

namespace Dynamik {
	namespace ADGR {
		class directx12Renderer : public RendererBackend {
		public:
			directx12Renderer();
			directx12Renderer(DMKRendererSettings settings) : RendererBackend(settings) {}
			~directx12Renderer();
		};
	}
}

#endif // !_DYNAMIK_ADGR_DIRECT_X_12_RENDERER_H

