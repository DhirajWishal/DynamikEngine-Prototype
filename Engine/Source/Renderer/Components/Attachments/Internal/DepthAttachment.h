#pragma once
#ifndef _DYNAMIK_RENDERER_DEPTH_ATTACHMENT_H
#define _DYNAMIK_RENDERER_DEPTH_ATTACHMENT_H

#include "InternalAttachment.h"

namespace Dynamik {
	namespace Renderer {
		class DepthAttachment : public InternalAttachment {
		public:
			DepthAttachment() {}
			~DepthAttachment() {}
		};
	}
}

#endif // !_DYNAMIK_RENDERER_DEPTH_ATTACHMENT_H
