#pragma once

#ifndef _DYNAMIK_MANAGERS_WINDOWS_IMAGE_LOADER_H
#define _DYNAMIK_MANAGERS_WINDOWS_IMAGE_LOADER_H

namespace Dynamik {
	namespace resource {
		//typedef stbi_uc image;

		enum TextureType {
			TEXTURE_TYPE_RGB,
			TEXTURE_TYPE_RGBA
		};

		struct DMK_API TextureData {
			uint64_t size = 0;

			int texWidth = 0, texHeight = 0, texChannels = 0;

			unsigned char* loadTexture(std::string path, TextureType type, bool flip = false);

			void freeData(unsigned char* image);
		};
	}
}

#endif // !_DYNAMIK_MANAGERS_IMAGE_LOADER_H
