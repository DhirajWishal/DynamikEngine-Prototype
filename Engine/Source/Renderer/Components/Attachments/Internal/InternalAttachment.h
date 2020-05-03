#pragma once
#ifndef _DYNAMIK_RENDERER_INTERNAL_ATTACHMENT_H
#define _DYNAMIK_RENDERER_INTERNAL_ATTACHMENT_H

#include "../RenderAttachment.h"

namespace Dynamik {
	namespace Renderer {
		class InternalAttachment : public RenderAttachment {
		public:
			InternalAttachment() {}
			virtual ~InternalAttachment() {}

			POINTER<TextureData> image;
		};
	}
}

#endif // !_DYNAMIK_RENDERER_INTERNAL_ATTACHMENT_H
