#pragma once

#ifndef _DYNAMIK_ADGR_OPENGL_RENDERER_H
#define _DYNAMIK_ADGR_OPENGL_RENDERER_H

#include "backend/base/RendererBackendBase.h"

#include "core/window.h"

namespace Dynamik {
	namespace ADGR {
		class openglRenderer : public RendererBackend {
		public:
			openglRenderer();
			openglRenderer(DMKRendererSettings settings) : RendererBackend(settings) {}
			~openglRenderer();

			void init() override;

		private:
			core::windowManagerGL myWindowManager;

			uint32_t openGLDataContainerIndex = 0;
			std::vector<core::ADGROpenGLDataContainer> myOpenGLDataContainers = {};
		};
	}
}

#endif // !_DYNAMIK_ADGR_OPENGL_RENDERER_H

