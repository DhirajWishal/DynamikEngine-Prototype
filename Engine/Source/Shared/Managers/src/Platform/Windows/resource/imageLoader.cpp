#include "mngafx.h"
#include "imageLoader.h"

#include "debugger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Dynamik {
	namespace resource {
		unsigned char* TextureData::loadTexture(std::wstring path, TextureType type, bool flip) {
			unsigned char* myTexture = nullptr;

			stbi_set_flip_vertically_on_load(flip);

			if (type == TEXTURE_TYPE_RGB)
				myTexture = stbi_load(WSTRtoCSTR(path).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb);
			else if (type == TEXTURE_TYPE_RGBA)
				myTexture = stbi_load(WSTRtoCSTR(path).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
			else
				DMK_CORE_ERROR("Invalid Texture type!");

			size = uint64_t(texWidth) * uint64_t(texHeight) * 4;

			if (!myTexture)
				return nullptr;

			return myTexture;
		}

		void TextureData::freeData(unsigned char* image) {
			stbi_image_free(image);
		}
	}
}