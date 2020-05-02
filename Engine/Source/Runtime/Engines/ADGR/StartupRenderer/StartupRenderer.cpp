#include "dmkafx.h"
#include "StartupRenderer.h"

#include <stb_image.h>

namespace Dynamik {
	namespace ADGR {
		void processInput(GLFWwindow* window)
		{
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
		}

		StartupRenderer StartupRenderer::instance;

		void StartupRenderer::initialize()
		{
			DMK_BEGIN_PROFILE_TIMER();

			glfwSetErrorCallback(_errorCallback);
			glfwInit();

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(DMK_DEBUG)
			instance.window = glfwCreateWindow(instance.windowWidth, instance.windowHeight, "Dynamik Engine", nullptr, nullptr);

#else
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			instance.window = glfwCreateWindow(mode->width, mode->height, info.title.c_str(), monitor, NULL);
			instance.windowWidth = mode->width;
			instance.windowHeight = mode->height;

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

#endif

			if (instance.window == NULL)
			{
				DMK_CORE_FATAL("Failed to create window!");
				glfwTerminate();
				return;
			}

			glfwMakeContextCurrent(instance.window);

			if (glewInit() != GLEW_OK)
				DMK_CORE_FATAL("Failed to initialize GLEW");

			glfwWindowHint(GLFW_SAMPLES, 4);
			glEnable(GL_MULTISAMPLE);
		}

		void StartupRenderer::initializeShaders()
		{
			DMK_BEGIN_PROFILE_TIMER();

			CCPTR vertShaderPath = "E:/Projects/Dynamik Engine/Versions/Dynamik/Engine/Application/Runtime/Shaders/Startup/shader.vert";
			CCPTR fragShaderPath = "E:/Projects/Dynamik Engine/Versions/Dynamik/Engine/Application/Runtime/Shaders/Startup/shader.frag";

			instance.myShaderManager = Shader_S(vertShaderPath, fragShaderPath);
		}

		void StartupRenderer::initializeVertexBuffers()
		{
			DMK_BEGIN_PROFILE_TIMER();

			float vertices[] = {
				// positions          // colors           // texture coords
				 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
				 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
				-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
				-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
			};
			unsigned int indices[] = {
				0, 1, 3, // first triangle
				1, 2, 3  // second triangle
			};
			glGenVertexArrays(1, &instance.VAO);
			glGenBuffers(1, &instance.VBO);
			glGenBuffers(1, &instance.EBO);

			glBindVertexArray(instance.VAO);

			glBindBuffer(GL_ARRAY_BUFFER, instance.VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance.EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// color attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			// texture coord attribute
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
		}

		void StartupRenderer::loadTexure(CCPTR path)
		{
			DMK_BEGIN_PROFILE_TIMER();

			stbi_set_flip_vertically_on_load(true);

			I32 nrChannels = 0;
			instance.textureData = stbi_load(path, &instance.windowWidth, &instance.windowHeight, &nrChannels, 0);
			if (!instance.textureData.isValid())
				DMK_CORE_FATAL("Failed to load Texture!");
		}

		void StartupRenderer::initializeTextureImage()
		{
			glGenTextures(1, &instance.textureIndex);
			glBindTexture(GL_TEXTURE_2D, instance.textureIndex);
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// load image, create texture and generate mipmaps

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, instance.windowWidth, instance.windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, instance.textureData.get());
			glGenerateMipmap(GL_TEXTURE_2D);

			instance.myShaderManager.use(); // don't forget to activate/use the shader before setting uniforms!
			// either set it manually like so:
			glUniform1i(glGetUniformLocation(instance.myShaderManager.ID, "texture1"), 0);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void StartupRenderer::draw()
		{
			DMK_BEGIN_PROFILE_TIMER();

			// input
			// -----
			processInput(instance.window);

			// render
			// ------
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// bind textures on corresponding texture units
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, instance.textureIndex);

			// render container
			instance.myShaderManager.use();
			glBindVertexArray(instance.VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(instance.window);
			glfwPollEvents();
		}

		void StartupRenderer::terminate()
		{
			DMK_BEGIN_PROFILE_TIMER();

			stbi_image_free(instance.textureData.get());

			glDeleteVertexArrays(1, &instance.VAO);
			glDeleteBuffers(1, &instance.VBO);
			glDeleteBuffers(1, &instance.EBO);

			glfwDestroyWindow(instance.window);
		}

		void StartupRenderer::makeContextCurrent()
		{
			glfwMakeContextCurrent(instance.window);
		}

		void StartupRenderer::_errorCallback(I32 id, CCPTR description)
		{
			std::cout << "GLFW Error: " << description << std::endl;
		}
	}
}