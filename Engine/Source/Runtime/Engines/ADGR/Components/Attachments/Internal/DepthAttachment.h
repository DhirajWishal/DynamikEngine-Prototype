#pragma once
#ifndef _DYNAMIK_ADGR_DEPTH_ATTACHMENT_H
#define _DYNAMIK_ADGR_DEPTH_ATTACHMENT_H

#include "../RenderAttachment.h"

namespace Dynamik {
	namespace ADGR {
		class DepthAttachment : public RenderAttachment {
		public:
			DepthAttachment() {}
			~DepthAttachment() {}
		};
	}
}

#endif // !_DYNAMIK_ADGR_DEPTH_ATTACHMENT_H
