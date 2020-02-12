#include "adgrafx.h"
#include "vertexAndIndexBuffer.h"
#ifdef DMK_USE_OPENGL
namespace Dynamik {
	namespace ADGR {
		namespace core {
			void vertexAndIndexBufferManager::init(openglFormat* format) {
				//glGenVertexArrays(1, &format->vertexArrayObject);
				glGenBuffers(1, &format->vertexBufferObject);
				glGenBuffers(1, &format->indexBufferObject);

				//glBindVertexArray(format->vertexArrayObject);

				glBindBuffer(GL_ARRAY_BUFFER, format->vertexBufferObject);
				glBufferData(GL_ARRAY_BUFFER,
					sizeof(format->myRendererFormat->myInternalFormat->myVertexBufferObjects[0]),
					format->myRendererFormat->myInternalFormat->myVertexBufferObjects[0].data(), GL_STATIC_DRAW);

				// position attribute
				glVertexAttribPointer(0, 3, GL_FLOAT_VEC3, GL_FALSE, 8 * sizeof(F32), (void*)0);
				glEnableVertexAttribArray(0);
				// color attribute
				glVertexAttribPointer(1, 3, GL_FLOAT_VEC3, GL_FALSE, 8 * sizeof(F32), (void*)(3 * sizeof(F32)));
				glEnableVertexAttribArray(1);
				// texture coord attribute
				glVertexAttribPointer(2, 2, GL_FLOAT_VEC2, GL_FALSE, 8 * sizeof(F32), (void*)(6 * sizeof(F32)));
				glEnableVertexAttribArray(2);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, format->indexBufferObject);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					sizeof(format->myRendererFormat->myInternalFormat->myIndexBufferObjects[0]),
					format->myRendererFormat->myInternalFormat->myIndexBufferObjects[0].data(), GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, NULL);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
			}

			void vertexAndIndexBufferManager::bind(const openglFormat* format) {
				//glBindVertexArray(format->vertexArrayObject);
				glBindBuffer(GL_ARRAY_BUFFER, format->vertexBufferObject);
				// position attribute
				glVertexAttribPointer(0, 3, GL_FLOAT_VEC3, GL_FALSE, 8 * sizeof(F32), (void*)0);
				glEnableVertexAttribArray(0);
				// color attribute
				glVertexAttribPointer(1, 3, GL_FLOAT_VEC3, GL_FALSE, 8 * sizeof(F32), (void*)(3 * sizeof(F32)));
				glEnableVertexAttribArray(1);
				// texture coord attribute
				glVertexAttribPointer(2, 2, GL_FLOAT_VEC2, GL_FALSE, 8 * sizeof(F32), (void*)(6 * sizeof(F32)));
				glEnableVertexAttribArray(2);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, format->indexBufferObject);
			}

			void vertexAndIndexBufferManager::draw(const openglFormat* format) {
				glDrawElements(GL_TRIANGLES, format->myRendererFormat->myInternalFormat->myIndexCounts[0], GL_UNSIGNED_INT, nullptr);
			}
			
			void vertexAndIndexBufferManager::clear(const openglFormat* format) {
				glDeleteVertexArrays(1, &format->vertexArrayObject);
				glDeleteBuffers(1, &format->vertexBufferObject);
				glDeleteBuffers(1, &format->indexBufferObject);
			}
		}
	}
}
#endif // DMK_USE_OPENGL

