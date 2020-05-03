#pragma once
#ifndef _DYNAMIK_RENDERER_STARTUP_RENDERER_H
#define _DYNAMIK_RENDERER_STARTUP_RENDERER_H

#include "Public/datatypes.h"
#include "Backend/Shader.h"

namespace Dynamik {
	namespace Renderer {
		class StartupRenderer {
			StartupRenderer() {}

			static StartupRenderer instance;
		public:
			StartupRenderer(const StartupRenderer&) = delete;
			StartupRenderer(StartupRenderer&&) = delete;

			static void initialize();
			static void initializeShaders();
			static void initializeVertexBuffers();
			static void loadTexure(CCPTR path);
			static void initializeTextureImage();

			static void draw();

			static void terminate();

			static void makeContextCurrent();

		private:
			static void _errorCallback(I32 id, CCPTR description);

			GLFWwindow* window = nullptr;
			I32 windowWidth = 1280;
			I32 windowHeight = 720;

			Shader_S myShaderManager;
			UI32 textureIndex = 0;
			UI32 VBO = 0, VAO = 0, EBO = 0;
			POINTER<UCHR> textureData;
		};
	}
}

#endif // !_DYNAMIK_RENDERER_STARTUP_RENDERER_H
