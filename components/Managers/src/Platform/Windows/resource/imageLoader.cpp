#include "mngafx.h"
#include "imageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Dynamik {
	namespace resource {

		unsigned char* TextureData::loadTexture(std::string path) {
			unsigned char*  myTexture = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

			size = uint64_t(texWidth) * uint64_t(texHeight) * 4;

			return myTexture;
		}

		void TextureData::freeData(unsigned char* image) {
			stbi_image_free(image);
		}
	}
}
