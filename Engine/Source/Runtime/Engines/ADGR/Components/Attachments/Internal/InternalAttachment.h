#pragma once
#ifndef _DYNAMIK_ADGR_INTERNAL_ATTACHMENT_H
#define _DYNAMIK_ADGR_INTERNAL_ATTACHMENT_H

#include "../RenderAttachment.h"

namespace Dynamik {
	namespace ADGR {
		class InternalAttachment : public RenderAttachment {
		public:
			InternalAttachment() {}
			virtual ~InternalAttachment() {}

			POINTER<TextureData> image;
		};
	}
}

#endif // !_DYNAMIK_ADGR_INTERNAL_ATTACHMENT_H
