#include "dmkafx.h"
#include "Texture.h"

#include <stb_image.h>
#include "load_ktx.hpp"

namespace Dynamik {
	void Texture::loadTexture(std::string path, DMKTextureType type, DMKTextureInputType inputType)
	{
		if (type == DMKTextureType::DMK_TEXTURE_TYPE_2D)
		{
			textureData.set(stbi_load(path.c_str(), &width, &height, &fileChannels, NULL));
		}
		else if (type == DMKTextureType::DMK_TEXTURE_TYPE_3D)
		{
		}

		_getFormatFromChannels();
	}

	void Texture::loadCubemap(ARRAY<std::string> paths, DMKTextureInputType inputType)
	{
		if (inputType == DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_AUTO)
		{
			if ((paths[0].find(".ktx") != std::string::npos) && (paths.size() < 6))
				inputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_KTX;
			else if ((paths[0].find(".hdri")) && (paths.size() < 6))
				inputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_HDRI;
			else
				inputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_IMAGES;
		}

		if (inputType == DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_KTX)
		{
			gli::texture ktxFile = gli::load_ktx(paths[0]);
			
			if (ktxFile.empty())
				DMK_CORE_FATAL("Failed to load the *.ktx file!");
			
			textureData.set((UCHR*)ktxFile.data());
			
			switch (ktxFile.format())
			{
			case gli::texture::format_type::FORMAT_RGBA8_UNORM_PACK8:
				fileChannels = 4;
				break;
			
			case gli::texture::format_type::FORMAT_RGB8_UNORM_PACK8:
				fileChannels = 3;
				break;
			default:
				break;
			}
		}
		else if (inputType == DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_HDRI)
		{
			textureData = stbi_load(paths[0].c_str(), &width, &height, &fileChannels, NULL);
		}
		else if (inputType == DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_IMAGES && paths.size() == 6)
		{
			imageCount = 6;
			POINTER<UCHR*> _pool[6] = { };
			for (UI32 index = 0; index < paths.size(); index++)
				_pool[index].set(stbi_load(paths[index].c_str(), &width, &height, &fileChannels, NULL));

			textureData = _pool->get();
		}

		_getFormatFromChannels();
	}

	void Texture::unloadTexture()
	{
		stbi_image_free(textureData.get());
	}

	UI32 Texture::size()
	{
		return width * height * fileChannels * imageCount;
	}

	void Texture::_getFormatFromChannels()
	{
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
}