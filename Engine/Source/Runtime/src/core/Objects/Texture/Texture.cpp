#include "dmkafx.h"
#include "Texture.h"

#include <stb_image.h>

namespace Dynamik {
	void Texture::loadTexture(std::string path)
	{
		I32 fileChennels = 0;
		textureData = stbi_load(path.c_str(), &width, &height, &fileChennels, STBI_rgb_alpha);
	}
	
	void Texture::unloadTexture()
	{
		stbi_image_free(textureData.get());
	}
}