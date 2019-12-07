#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_GL_TEXTURE_H
#define _DYNAMIK_ADGR_VULKAN_GL_TEXTURE_H

#include "core/Texture.h"

#include <string>

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class glTexture : public Core {
			public:
				glTexture(std::string& path);
				~glTexture();

				void bind(unsigned int slot = 0);
				void unBind();

				inline unsigned int getWidth() const { return myWidth; }
				inline unsigned int getHeight() const { return myHeight; };

			private:
				unsigned int rendererID = 0;
				std::string filePath = "";
				unsigned char* localBuffer = nullptr;
				int myWidth = 0;
				int myHeight = 0;
				int myBufferPerPixel = 0;
			};
		}
	}
}

#endif
