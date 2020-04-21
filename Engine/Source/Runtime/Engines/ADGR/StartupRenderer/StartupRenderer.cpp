#include "dmkafx.h"
#include "StartupRenderer.h"

#include <stb_image.h>

namespace Dynamik {
	namespace ADGR {
		inline void StartupRenderer::initialize()
		{
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(DMK_DEBUG)
			myInstance.window = glfwCreateWindow(myInstance.windowWidth, myInstance.windowHeight, "Dynamik Engine", nullptr, nullptr);

#else
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			myInstance.window = glfwCreateWindow(mode->width, mode->height, info.title.c_str(), monitor, NULL);
			myInstance.windowWidth = mode->width;
			myInstance.windowHeight = mode->height;

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

#endif

			if (myInstance.window == NULL)
			{
				DMK_CORE_FATAL("Failed to create window!");
				glfwTerminate();
				return;
			}

			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
			glfwMakeContextCurrent(myInstance.window);

			if (glewInit() != GLEW_OK)
				DMK_CORE_FATAL("Failed to initialize GLEW");
		}
		
		void StartupRenderer::initializeShaders()
		{
			CCPTR vertShaderPath = "E:/Projects/Dynamik Engine/Versions/Dynamik (Prototype)/Engine/Application/Runtime/Shaders/Startup/shader.vert";
			CCPTR fragShaderPath = "E:/Projects/Dynamik Engine/Versions/Dynamik (Prototype)/Engine/Application/Runtime/Shaders/Startup/shader.frag";

			myInstance.myShaderManager = Shader(vertShaderPath, fragShaderPath);
		}

		void StartupRenderer::initializeVertexBuffers()
		{
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
			glGenVertexArrays(1, &myInstance.VAO);
			glGenBuffers(1, &myInstance.VBO);
			glGenBuffers(1, &myInstance.EBO);

			glBindVertexArray(myInstance.VAO);

			glBindBuffer(GL_ARRAY_BUFFER, myInstance.VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myInstance.EBO);
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
			glGenTextures(1, &myInstance.textureIndex);
			glBindTexture(GL_TEXTURE_2D, myInstance.textureIndex);
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// load image, create texture and generate mipmaps
			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true);

			I32 width = 0, height = 0, nrChannels = 0;
			UCHR* data = stbi_load(path, &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				DMK_CORE_FATAL("Failed to load Texture!");
			}
			stbi_image_free(data);

			myInstance.myShaderManager.use(); // don't forget to activate/use the shader before setting uniforms!
			// either set it manually like so:
			glUniform1i(glGetUniformLocation(myInstance.myShaderManager.ID, "texture1"), 0);
		}
		
		void StartupRenderer::draw()
		{
			// input
		// -----
			processInput(myInstance.window);

			// render
			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// bind textures on corresponding texture units
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, myInstance.textureIndex);

			// render container
			myInstance.myShaderManager.use();
			glBindVertexArray(myInstance.VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(myInstance.window);
			glfwPollEvents();

		}

		void StartupRenderer::terminate()
		{
			glDeleteVertexArrays(1, &myInstance.VAO);
			glDeleteBuffers(1, &myInstance.VBO);
			glDeleteBuffers(1, &myInstance.EBO);

			// glfw: terminate, clearing all previously allocated GLFW resources.
			// ------------------------------------------------------------------
			glfwTerminate();

		}

		void processInput(GLFWwindow* window)
		{
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
		}
	}
}