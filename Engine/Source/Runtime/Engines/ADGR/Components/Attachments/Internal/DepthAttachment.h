#pragma once
#ifndef _DYNAMIK_ADGR_DEPTH_ATTACHMENT_H
#define _DYNAMIK_ADGR_DEPTH_ATTACHMENT_H

#include "InternalAttachment.h"

namespace Dynamik {
	namespace ADGR {
		class DepthAttachment : public InternalAttachment {
		public:
			DepthAttachment() {}
			~DepthAttachment() {}
		};
	}
}

#endif // !_DYNAMIK_ADGR_DEPTH_ATTACHMENT_H
