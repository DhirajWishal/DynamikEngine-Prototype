#include "adgrafx.h"
#ifdef DMK_USE_OPENGL
#include "shader.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			void shaderManager::init(unsigned I32* ID, const std::string& path, DMKOpengGLShaderType shaderType) {
				std::string shaderCode;
				std::ifstream shaderFile;
				// ensure ifstream objects can throw exceptions:
				shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

				try {
					// open files
					shaderFile.open(path);

					std::stringstream shaderStream;
					shaderStream << shaderFile.rdbuf();

					shaderFile.close();
					shaderCode = shaderStream.str();
				}
				catch (std::ifstream::failure e) {
					DMK_CORE_FATAL("Shader file coulf not be opened!")
				}

				const char* shaderCodeCCP = shaderCode.c_str();
				unsigned I32 shader = 0;
				I32 success;
				char infoLog[512];

				if (shaderType == DMKOpengGLShaderType::DMK_ADGR_OPENGL_SHADER_TYPE_VERTEX)
					shader = glCreateShader(GL_VERTEX_SHADER);
				if (shaderType == DMKOpengGLShaderType::DMK_ADGR_OPENGL_SHADER_TYPE_FRAGMENT)
					shader = glCreateShader(GL_FRAGMENT_SHADER);

				glShaderSource(shader, 1, &shaderCodeCCP, NULL);
				glCompileShader(shader);
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success)
					DMK_CORE_ERROR("Error initializing shader!");

				if (*ID)
					*ID = glCreateProgram();
				glAttachShader(*ID, shader);
				glLinkProgram(*ID);
				glGetProgramiv(*ID, GL_LINK_STATUS, &success);
				if (!success)
					DMK_CORE_ERROR("Error linking shader!");

				glDeleteShader(shader);
			}

			void shaderManager::bind(const openglFormat* format) {
				glUseProgram(format->myShaderID);
			}

			void shaderManager::unbind() {
			}

			void shaderManager::updateBool(unsigned I32 id, const std::string& name, B1 value) const {
				glUniform1i(glGetUniformLocation(id, name.c_str()), (I32)value);
			}

			void shaderManager::updateInt(unsigned I32 id, const std::string& name, I32 value) const {
				glUniform1i(glGetUniformLocation(id, name.c_str()), value);
			}

			void shaderManager::updateFloat(unsigned I32 id, const std::string& name, F32 value) const {
				glUniform1f(glGetUniformLocation(id, name.c_str()), value);
			}

			void shaderManager::setTransform(openglFormat* format, std::deque<DMKEventContainer>& eventContainers) const {
				DMKUpdateInfo updateInfo = {};
				updateInfo = format->myRendererFormat->myInternalFormat->myGameObject->draw(eventContainers);

				glm::mat4 model = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(updateInfo.leftRight, updateInfo.frontBack, updateInfo.upDown)),
					glm::radians(updateInfo.rotationX), glm::vec3(0.0f, 0.0f, 1.0f))
					* glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
						glm::radians(updateInfo.rotationY), glm::vec3(0.0f, 1.0f, 0.0f))
					* glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
						glm::radians(updateInfo.rotationZ), glm::vec3(1.0f, 0.0f, 0.0f));
				glm::mat4 view = glm::lookAt(glm::vec3(0.5f, 3.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 0.0f, 1.0f));
				glm::mat4 proj = glm::perspective(glm::radians(45.0f), (F32)1280 / (F32)720, 0.001f, 10.0f);	// width, height
				proj[1][1] *= -1;

				glUniformMatrix4fv(glGetUniformLocation(format->myShaderID, "model"), 1, GL_FALSE, &model[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(format->myShaderID, "view"), 1, GL_FALSE, &view[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(format->myShaderID, "projection"), 1, GL_FALSE, &proj[0][0]);
			}
		}
	}
}
#endif // DMK_USE_OPENGL