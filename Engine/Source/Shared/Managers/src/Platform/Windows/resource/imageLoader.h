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

		struct  TextureData {
			UI64 size = 0;

			I32 texWidth = 0, texHeight = 0, texChannels = 0;

			unsigned char* loadTexture(std::string path, TextureType type, B1 flip = false);

			void freeData(UCPTR image);
		};
	}
}

#endif // !_DYNAMIK_MANAGERS_IMAGE_LOADER_H
