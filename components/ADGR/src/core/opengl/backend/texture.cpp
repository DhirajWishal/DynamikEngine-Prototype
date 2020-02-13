#include "adgrafx.h"
#include "texture.h"

#include "Platform/windows/resource/imageLoader.h"
#include "Platform.h"

#ifdef DMK_USE_OPENGL
namespace Dynamik {
	namespace ADGR {
		namespace core {
			void textureManager::init(openglFormat* format) {
				resource::TextureData texData;

				for (std::string_view path : format->myRendererFormat->myInternalFormat->myGameObject->myProperties.texturePaths) {
					unsigned char* localBuffer = texData.loadTexture(path.data(), resource::TEXTURE_TYPE_RGBA, true);

					unsigned I32 ID = 0;
					glGenTextures(1, &ID);
					glBindTexture(GL_TEXTURE_2D, ID);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texData.texWidth, texData.texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
					glGenerateMipmap(GL_TEXTURE_2D);

					if (localBuffer)
						texData.freeData(localBuffer);

					format->myTextureIDs.push_back(ID);
				}
			}

			void textureManager::bind(openglFormat* format, unsigned I32 slot) {
				for (UI32 i = 0; i < format->myTextureIDs.size(); i++) {
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, format->myTextureIDs[i]);
				}
			}

			void textureManager::unBind() {
			}

			void textureManager::destroy(unsigned I32 id) {
				glDeleteTextures(1, &id);
			}
		}
	}
}
#endif // DMK_USE_OPENGL
