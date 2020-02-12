#pragma once

#ifndef _DYNAMIK_ADGR_OPENGL_RENDERER_H
#define _DYNAMIK_ADGR_OPENGL_RENDERER_H
#ifdef DMK_USE_OPENGL

#include "backend/base/RendererBackendBase.h"

#include "core/openglFormat.h"
#include "core/window.h"
#include "core/shader.h"
#include "core/texture.h"
#include "core/vertexAndIndexBuffer.h"

namespace Dynamik {
	namespace ADGR {
		class openglRenderer : public RendererBackend {
		public:
			openglRenderer();
			openglRenderer(DMKRendererSettings settings) : RendererBackend(settings) {}
			~openglRenderer();

			void init() override;
			void initStageOne() override;
			void initStageTwo() override;
			void initStageThree() override;

			void drawFrame() override;
			void shutDown() override;


			std::deque<DMKEventContainer>* events() override;
			inline B1 closeEvent() override { return myWindowManager.closeEvent(&myOpenGLDataContainers[openGLDataContainerIndex]); }

			void setFormats(std::vector<RendererFormat>& rendererFormats) override;
			void updateFormats(std::vector<RendererFormat>& rendererFormats) override;

		private:
			void _addFormats(std::vector<RendererFormat>& rendererFormats);

			core::windowManagerGL myWindowManager;
			core::shaderManager myShaderManager;
			core::textureManager myTextureManager;
			core::vertexAndIndexBufferManager myVertexAndIndexBufferManager;

			UI32 openGLDataContainerIndex = 0;
			std::vector<core::ADGROpenGLDataContainer> myOpenGLDataContainers = {};
			std::vector<core::openglFormat> myOpenglFormats = {};

			unsigned I32 VBO = 0, VAO = 0, EBO = 0;
			I32 shaderProgram = 0;
			unsigned I32 textureProg = 0;
		};
	}
}

#endif // DMK_USE_OPENGL

#endif // !_DYNAMIK_ADGR_OPENGL_RENDERER_H

