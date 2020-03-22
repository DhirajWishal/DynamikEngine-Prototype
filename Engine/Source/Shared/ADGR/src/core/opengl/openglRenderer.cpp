#include "adgrafx.h"
#ifdef DMK_USE_OPENGL

#include "Platform/windows.h"
#include "openglRenderer.h"

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, I32 width, I32 height) {
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

namespace Dynamik {
	namespace ADGR {
		using namespace core;

		openglRenderer::openglRenderer() {
		}

		openglRenderer::~openglRenderer() {
		}

		void openglRenderer::init() {
		}

		void openglRenderer::initStageOne() {
			myOpenGLDataContainers.resize(openGLDataContainerIndex + 1);

			myWindowManager.init(&myOpenGLDataContainers[openGLDataContainerIndex]);

			glEnable(GL_DEPTH_TEST);
		}

		void openglRenderer::initStageTwo() {
			for (UI32 itr = 0; itr < myFormatsCount; itr++) {
				openglFormat* _localFormat = &myOpenglFormats[itr];

				// vertex shader
				if (_localFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.vertexShaderPath != "NONE")
					myShaderManager.init(&_localFormat->myShaderID, _localFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.vertexShaderPath, DMKOpengGLShaderType::DMK_ADGR_OPENGL_SHADER_TYPE_VERTEX);
				// tessellation shader
				if (_localFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.tessellationShaderPath != "NONE")
					myShaderManager.init(&_localFormat->myShaderID, _localFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.tessellationShaderPath, DMKOpengGLShaderType::DMK_ADGR_OPENGL_SHADER_TYPE_VERTEX);
				// geometry shader
				if (_localFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.geometryShaderPath != "NONE")
					myShaderManager.init(&_localFormat->myShaderID, _localFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.geometryShaderPath, DMKOpengGLShaderType::DMK_ADGR_OPENGL_SHADER_TYPE_VERTEX);
				// fragment shader
				if (_localFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.fragmentShaderPath != "NONE")
					myShaderManager.init(&_localFormat->myShaderID, _localFormat->myRendererFormat->myInternalFormat->myGameObject->myProperties.renderableObjectProperties.fragmentShaderPath, DMKOpengGLShaderType::DMK_ADGR_OPENGL_SHADER_TYPE_FRAGMENT);

				myVertexAndIndexBufferManager.init(_localFormat);

				myTextureManager.init(_localFormat);
				myShaderManager.bind(_localFormat);
				//glUniform1i(glGetUniformLocation(_localFormat->myShaderID, "texSampler"), 0);
				glUseProgram(NULL);
			}
		}

		void openglRenderer::initStageThree() {
		}

		void openglRenderer::drawFrame() {
			glfwPollEvents();
			// input
// -----
			processInput(myOpenGLDataContainers[openGLDataContainerIndex].window);

			// render
			// ------
			glClearColor(
				myRendererSettings.clearScreenValues[0],
				myRendererSettings.clearScreenValues[1],
				myRendererSettings.clearScreenValues[2],
				myRendererSettings.clearScreenValues[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (UI32 itr = 0; itr < myFormatsCount; itr++) {
				openglFormat* _localFormat = &myOpenglFormats[itr];
				//myTextureManager.bind(_localFormat);
				myShaderManager.bind(_localFormat);

				// draw our first triangle
				myShaderManager.setTransform(_localFormat, myEventContainers);
				myVertexAndIndexBufferManager.bind(_localFormat);
				myVertexAndIndexBufferManager.draw(_localFormat);
			}
			// glBindVertexstd::vector(0); // no need to unbind it every time

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(myOpenGLDataContainers[openGLDataContainerIndex].window);
		}

		void openglRenderer::shutDown() {
			for (auto format : myOpenglFormats)
				myVertexAndIndexBufferManager.clear(&format);

			// glfw: terminate, clearing all previously allocated GLFW resources.
			// ------------------------------------------------------------------
			glfwTerminate();
		}

		std::deque<DMKEventContainer>* openglRenderer::events() {
			myWindowManager.pollEvents();
			myEventContainers = myWindowManager.getEventContainer();
			return &myEventContainers;
		}
		void openglRenderer::setFormats(ARRAY<RendererFormat>& rendererFormats) {
			_addFormats(rendererFormats);
		}

		void openglRenderer::updateFormats(ARRAY<RendererFormat>& rendererFormats) {
			_addFormats(rendererFormats);
		}

		void openglRenderer::_addFormats(ARRAY<RendererFormat>& rendererFormats) {
			myFormatsCount = rendererFormats.size();
			for (I32 i = 0; i < myFormatsCount; i++)
				myOpenglFormats.push_back(openglFormat(&rendererFormats[i]));
		}
	}
}
#endif // DMK_USE_OPENGL