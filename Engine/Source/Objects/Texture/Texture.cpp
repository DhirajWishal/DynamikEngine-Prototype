#include "dmkafx.h"
#include "Texture.h"

#include <stb_image.h>
#include "load_ktx.hpp"

namespace Dynamik {
	void Texture::loadTexture(std::string path, DMKTextureType type, DMKTextureInputType inputType)
	{
		if (type == DMKTextureType::DMK_TEXTURE_TYPE_2D)
		{
			if (path.find(".ktx") != std::string::npos)
			{

				gli::texture ktxFile = gli::load_ktx(path);

				if (ktxFile.empty())
					DMK_CORE_FATAL("Failed to load the *.ktx file!");

				textureData = StaticAllocator<UCHR>::allocate(ktxFile.size());
				memcpy(textureData.get(), ktxFile.data(), ktxFile.size());

				width = ktxFile.extent().x;
				height = ktxFile.extent().y;

				switch (ktxFile.format())
				{
				case gli::texture::format_type::FORMAT_RGBA8_UNORM_PACK8:
					fileChannels = 4;
					break;

				case gli::texture::format_type::FORMAT_RGB8_UNORM_PACK8:
					fileChannels = 3;
					break;
				case gli::texture::format_type::FORMAT_RGBA_ASTC_8X8_UNORM_BLOCK16:
					fileChannels = 4;
					break;
				default:
					break;
				}
				inputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_KTX;
			}
			else
			{
				stbi_set_flip_vertically_on_load(true);
				auto data = stbi_load(path.c_str(), &width, &height, &fileChannels, NULL);
				textureData = data;
			}
		}
		else if (type == DMKTextureType::DMK_TEXTURE_TYPE_3D)
		{
		}
		else if (type == DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP);

		_getFormatFromChannels();
	}

	void Texture::loadCubemap(std::vector<std::string> paths, DMKTextureInputType inputType)
	{
		type = DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP;

		if (inputType == DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_AUTO)
		{
			if ((paths[0].find(".ktx") != std::string::npos) && (paths.size() < 6))
			{

				gli::texture ktxFile = gli::load_ktx(paths[0]);

				if (ktxFile.empty())
					DMK_CORE_FATAL("Failed to load the *.ktx file!");

				textureData = StaticAllocator<UCHR>::allocate(ktxFile.size());
				memcpy(textureData.get(), ktxFile.data(), ktxFile.size());

				width = ktxFile.extent().x;
				height = ktxFile.extent().y;

				switch (ktxFile.format())
				{
				case gli::texture::format_type::FORMAT_RGBA8_UNORM_PACK8:
					fileChannels = 4;
					break;

				case gli::texture::format_type::FORMAT_RGB8_UNORM_PACK8:
					fileChannels = 3;
					break;
				case gli::texture::format_type::FORMAT_RGBA_ASTC_8X8_UNORM_BLOCK16:
					fileChannels = 4;
					break;
				default:
					break;
				}
				inputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_KTX;
			}
			else if ((paths[0].find(".hdri")) && (paths.size() < 6))
				inputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_HDRI;
			else
				inputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_IMAGE;
		}

		if (inputType == DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_KTX)
		{
			gli::texture ktxFile = gli::load_ktx(paths[0]);

			if (ktxFile.empty())
				DMK_CORE_FATAL("Failed to load the *.ktx file!");

			textureData = StaticAllocator<UCHR>::allocate(ktxFile.size());
			memcpy(textureData.get(), ktxFile.data(), ktxFile.size());

			width = ktxFile.extent().x;
			height = ktxFile.extent().y;

			switch (ktxFile.format())
			{
			case gli::texture::format_type::FORMAT_RGBA8_UNORM_PACK8:
				fileChannels = 4;
				break;

			case gli::texture::format_type::FORMAT_RGB8_UNORM_PACK8:
				fileChannels = 3;
				break;
			case gli::texture::format_type::FORMAT_RGBA_ASTC_8X8_UNORM_BLOCK16:
				fileChannels = 4;
				break;
			default:
				break;
			}
		}
		else if (inputType == DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_HDRI)
		{
			textureData = stbi_load(paths[0].c_str(), &width, &height, &fileChannels, NULL);
		}
		else if (inputType == DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_IMAGE && paths.size() == 6)
		{
			imageCount = 6;
			POINTER<UCHR> _pool[6] = { };
			for (UI32 index = 0; index < paths.size(); index++)
				_pool[index] = stbi_load(paths[index].c_str(), &width, &height, &fileChannels, STBI_rgb_alpha);

			fileChannels = 4;
			textureData = StaticAllocator<UCHR>::allocate(size());
			UI32 layerSize = size() / 6;

			for (UI32 itr = 0; itr < paths.size(); itr++)
				memcpy((void*)(UI64(textureData.get()) + (layerSize * itr)), _pool[itr], layerSize);
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