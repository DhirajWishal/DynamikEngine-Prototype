#pragma once
#ifndef _DYNAMIK_TEXTURE_H
#define _DYNAMIK_TEXTURE_H

#include "Public/Pointer.h"
#include "Public/datatypes.h"
#include "../DMKObject/DMKObject.h"
#include "Dynamik/objectSpecifications.h"
#include "GameObjectDescriptors.h"

namespace Dynamik {
	class Texture : public DMKObject {
	public:
		Texture() {}
		virtual ~Texture() {}

		virtual void loadTexture(std::string path, DMKTextureType type, DMKTextureInputType inputType);
		virtual void loadCubemap(std::vector<std::string> paths, DMKTextureInputType inputType);
		virtual void unloadTexture();
		virtual UI32 size();

		POINTER<UCHR> textureData;
		I32 width = 0;
		I32 height = 0;
		UI32 mipLevel = 0;
		DMKFormat format = DMKFormat::DMK_FORMAT_RGBA_8_UNIFORM;
		DMKTextureType type = DMKTextureType::DMK_TEXTURE_TYPE_2D;
		I32 fileChannels = 0;
		UI32 imageCount = 1;

	private:
		void _getFormatFromChannels();
	};
}

#endif // !_DYNAMIK_TEXTURE_H
