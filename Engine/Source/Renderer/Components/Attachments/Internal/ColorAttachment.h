#pragma once
#ifndef _DYNAMIK_RENDERER_COLOR_ATTACHMENT_H
#define _DYNAMIK_RENDERER_COLOR_ATTACHMENT_H

#include "InternalAttachment.h"

namespace Dynamik {
	namespace Renderer {
		class ColorAttachment : public InternalAttachment {
		public:
			ColorAttachment() {}
			~ColorAttachment() {}
		};
	}
}

#endif // !_DYNAMIK_RENDERER_COLOR_ATTACHMENT_H
