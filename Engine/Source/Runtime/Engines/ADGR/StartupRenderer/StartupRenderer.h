#pragma once
#ifndef _DYNAMIK_ADGR_STARTUP_RENDERER_H
#define _DYNAMIK_ADGR_STARTUP_RENDERER_H

#include "Public/datatypes.h"
#include "Backend/Shader.h"

namespace Dynamik {
	namespace ADGR {
		class StartupRenderer {
			StartupRenderer() {}

			static StartupRenderer& instance();

		public:
			StartupRenderer(const StartupRenderer&) = delete;
			StartupRenderer(StartupRenderer&&) = delete;

			static void initialize();
			static void initializeShaders();
			static void initializeVertexBuffers();
			static void loadTexure(CCPTR path);
			static void draw();
			static void terminate();

		private:
			GLFWwindow* window = nullptr;
			UI32 windowWidth = 1280;
			UI32 windowHeight = 720;

			Shader myShaderManager;
			UI32 textureIndex = 0;
			UI32 VBO = 0, VAO = 0, EBO = 0;
		};
	}
}

#endif // !_DYNAMIK_ADGR_STARTUP_RENDERER_H
