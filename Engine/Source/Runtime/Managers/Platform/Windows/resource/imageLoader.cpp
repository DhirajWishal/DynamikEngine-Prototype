#include "dmkafx.h"
#include "imageLoader.h"

#include "debugger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Dynamik {
	namespace resource {
		unsigned char* TextureData::loadTexture(std::string path, TextureType type, B1 flip) {
			UCPTR myTexture = nullptr;

			stbi_set_flip_vertically_on_load(flip);

			if (type == TEXTURE_TYPE_RGB)
				myTexture = stbi_load((path).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb);
			else if (type == TEXTURE_TYPE_RGBA)
				myTexture = stbi_load((path).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
			else
				DMK_CORE_ERROR("Invalid Texture type!");

			size = UI64(texWidth) * UI64(texHeight) * 4;

			if (!myTexture)
				return nullptr;

			return myTexture;
		}

		void TextureData::freeData(UCPTR image) {
			stbi_image_free(image);
		}
	}
}