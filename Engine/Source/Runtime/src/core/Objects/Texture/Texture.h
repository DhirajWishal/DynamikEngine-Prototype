#pragma once
#ifndef _DYNAMIK_TEXTURE_H
#define _DYNAMIK_TEXTURE_H

#include "DataTypesLib/Public/Pointer.h"
#include "DataTypesLib/Public/datatypes.h"
#include "../DMKObject.h"

namespace Dynamik {
	enum class DMKTextureFormat {
		DMK_TEXTURE_FORMAT_R_8,
		DMK_TEXTURE_FORMAT_RG_8,
		DMK_TEXTURE_FORMAT_RGB_8,
		DMK_TEXTURE_FORMAT_RGBA_8,

		DMK_TEXTURE_FORMAT_R_16,
		DMK_TEXTURE_FORMAT_RG_16,
		DMK_TEXTURE_FORMAT_RGB_16,
		DMK_TEXTURE_FORMAT_RGBA_16,

		DMK_TEXTURE_FORMAT_R_32,
		DMK_TEXTURE_FORMAT_RG_32,
		DMK_TEXTURE_FORMAT_RGB_32,
		DMK_TEXTURE_FORMAT_RGBA_32,

		DMK_TEXTURE_FORMAT_D_8,
		DMK_TEXTURE_FORMAT_D_16,
		DMK_TEXTURE_FORMAT_D_32,
	};

	class Texture : public DMKObject {
	public:
		Texture() {}
		virtual ~Texture() {}

		virtual void loadTexture(std::string path);
		virtual void unloadTexture();

		POINTER<UCHR> textureData;
		I32 width = 0;
		I32 height = 0;
		UI32 mipLevel = 0;
	};
}

#endif // !_DYNAMIK_TEXTURE_H
