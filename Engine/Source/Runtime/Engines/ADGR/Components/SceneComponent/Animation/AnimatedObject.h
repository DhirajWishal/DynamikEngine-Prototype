#pragma once
#ifndef _DYNAMIK_ADGR_ANIMATED_OBJECT_H
#define _DYNAMIK_ADGR_ANIMATED_OBJECT_H

#include "Objects/InternalFormat/InternalFormat.h"

namespace Dynamik {
	namespace ADGR {
		class AnimatedObject {
		public:
			AnimatedObject() {}
			virtual ~AnimatedObject() {}

			virtual void setInternalFormat(POINTER<InternalFormat> format) { myInternalFormat = format; }

			POINTER<InternalFormat> myInternalFormat;
		};
	}
}

#endif // !_DYNAMIK_ADGR_ANIMATED_OBJECT_H
