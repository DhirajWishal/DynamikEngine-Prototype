#pragma once
#ifndef _DYNAMIK_TEXTURE_H
#define _DYNAMIK_TEXTURE_H

#include "Public/Pointer.h"
#include "Public/datatypes.h"
#include "../DMKObject/DMKObject.h"
#include "Dynamik/objectSpecifications.h"

namespace Dynamik {
	class Texture : public DMKObject {
	public:
		Texture() {}
		virtual ~Texture() {}

		virtual void loadTexture(std::string path);
		virtual void unloadTexture();
		virtual UI32 size();

		POINTER<UCHR> textureData;
		I32 width = 0;
		I32 height = 0;
		UI32 mipLevel = 0;
		DMKFormat format = DMKFormat::DMK_FORMAT_RGBA_8_UNIFORM;
		I32 fileChannels = 0;
	};
}

#endif // !_DYNAMIK_TEXTURE_H
