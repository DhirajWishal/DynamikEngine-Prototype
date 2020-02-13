#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_GL_TEXTURE_H
#define _DYNAMIK_ADGR_VULKAN_GL_TEXTURE_H
#ifdef DMK_USE_VULKAN

#include <string>

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class glTexture {
			public:
				glTexture(std::string& path);
				~glTexture();

				void bind(UI32 slot = 0);
				void unBind();

				inline UI32 getWidth() const { return myWidth; }
				inline UI32 getHeight() const { return myHeight; };

			private:
				UI32 rendererID = 0;
				std::string filePath = "";
				UCPTR localBuffer = nullptr;
				I32 myWidth = 0;
				I32 myHeight = 0;
				I32 myBufferPerPixel = 0;
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif
