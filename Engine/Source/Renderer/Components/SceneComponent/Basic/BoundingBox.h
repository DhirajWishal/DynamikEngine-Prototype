#pragma once
#ifndef _DYNAMIK_BOUNDING_BOX_H
#define _DYNAMIK_BOUNDING_BOX_H

#include "Objects/InternalFormat/InternalFormat.h"

namespace Dynamik {
	class BoundingBox : public InternalFormat {
	public:
		BoundingBox() {}
		~BoundingBox() {}

		virtual void initialize() override;
		virtual DMKUniformBufferData onUpdate(DMKCameraData data) override;
	};
}

#endif // !_DYNAMIK_BOUNDING_BOX_H
