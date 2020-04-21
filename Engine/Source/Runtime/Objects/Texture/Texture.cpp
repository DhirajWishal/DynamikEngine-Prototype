#include "dmkafx.h"
#include "Texture.h"

#include <stb_image.h>

namespace Dynamik {
	void Texture::loadTexture(std::string path)
	{
		textureData = stbi_load(path.c_str(), &width, &height, &fileChannels, NULL);

		switch (fileChannels)
		{
		case 1:
			format = DMKFormat::DMK_FORMAT_R_8_UNIFORM;
			break;

		case 2:
			format = DMKFormat::DMK_FORMAT_RG_8_UNIFORM;
			break;

		case 3:
			format = DMKFormat::DMK_FORMAT_RGB_8_UNIFORM;
			break;

		case 4:
			format = DMKFormat::DMK_FORMAT_RGBA_8_UNIFORM;
			break;

		default:
			format = DMKFormat::DMK_FORMAT_UNDEFINED;
			break;
		}
	}
	
	void Texture::unloadTexture()
	{
		stbi_image_free(textureData.get());
	}

	UI32 Texture::size() 
	{ 
		return width * height * fileChannels; 
	}
}