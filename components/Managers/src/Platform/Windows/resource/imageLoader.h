#pragma once

namespace Dynamik {
	namespace resource {

		//typedef stbi_uc image;


		enum TextureType {
			TEXTURE_TYPE_RGB,
			TEXTURE_TYPE_RGBA
		};

		struct TextureData {
			uint64_t size = 0;

			int texWidth = 0, texHeight = 0, texChannels = 0;

			unsigned char* loadTexture(std::string path, TextureType type);

			void freeData(unsigned char* image);
		};
	}
}
