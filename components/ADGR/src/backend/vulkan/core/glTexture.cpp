#include "adgrafx.h"
#ifdef DMK_USE_VULKAN
#include "glTexture.h"
#include "Platform/windows/resource/imageLoader.h"
#include "Platform.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			glTexture::glTexture(std::string& path) : filePath(path) {
				resource::TextureData texData;

				localBuffer = texData.loadTexture(path, resource::TEXTURE_TYPE_RGB, true);

				glGenTextures(1, &rendererID);
				glBindTexture(GL_TEXTURE_2D, rendererID);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, myWidth, myHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
				glBindTexture(GL_TEXTURE_2D, 0);

				if (localBuffer)
					texData.freeData(localBuffer);
			}

			glTexture::~glTexture() {
				glDeleteTextures(1, &rendererID);
			}

			void glTexture::bind(unsigned int slot) {
				//glActiveTexture()
				glBindTexture(GL_TEXTURE_2D, rendererID);
			}

			void glTexture::unBind() {
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}
}
#endif // DMK_USE_VULKAN
