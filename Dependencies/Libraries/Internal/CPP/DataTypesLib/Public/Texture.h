#pragma once
#ifndef _DYNAMIK_DATA_TYPES_TEXTURE_H
#define _DYNAMIK_DATA_TYPES_TEXTURE_H

#include "FastString.h"

namespace Dynamik {
	class DMKTexture {
	public:
		DMKTexture();
		virtual ~DMKTexture();

		virtual void load(FSTR path);
		virtual void convertToCubemap();
		virtual void convertToHDRI();
	};
}

#endif // !_DYNAMIK_DATA_TYPES_TEXTURE_H
