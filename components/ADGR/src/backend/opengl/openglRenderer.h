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
			inline bool closeEvent() override { return myWindowManager.closeEvent(&myOpenGLDataContainers[openGLDataContainerIndex]); }

			void setFormats(std::vector<RendererFormat>& rendererFormats) override;
			void updateFormats(std::vector<RendererFormat>& rendererFormats) override;

		private:
			void _addFormats(std::vector<RendererFormat>& rendererFormats);

			core::windowManagerGL myWindowManager;
			core::shaderManager myShaderManager;
			core::textureManager myTextureManager;
			core::vertexAndIndexBufferManager myVertexAndIndexBufferManager;

			uint32_t openGLDataContainerIndex = 0;
			std::vector<core::ADGROpenGLDataContainer> myOpenGLDataContainers = {};
			std::vector<core::openglFormat> myOpenglFormats = {};

			unsigned int VBO = 0, VAO = 0, EBO = 0;
			int shaderProgram = 0;
			unsigned int textureProg = 0;
		};
	}
}

#endif // DMK_USE_OPENGL

#endif // !_DYNAMIK_ADGR_OPENGL_RENDERER_H

