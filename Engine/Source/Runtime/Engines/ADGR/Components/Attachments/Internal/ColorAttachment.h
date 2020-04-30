#pragma once
#ifndef _DYNAMIK_ADGR_COLOR_ATTACHMENT_H
#define _DYNAMIK_ADGR_COLOR_ATTACHMENT_H

#include "../RenderAttachment.h"

namespace Dynamik {
	namespace ADGR {
		class ColorAttachment : public RenderAttachment {
		public:
			ColorAttachment() {}
			~ColorAttachment() {}

			TextureData colorImage;
		};
	}
}

#endif // !_DYNAMIK_ADGR_COLOR_ATTACHMENT_H
