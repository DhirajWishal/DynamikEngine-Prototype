#pragma once
#ifndef _DYNAMIK_ADGR_GRAPHICS_OBJECT_H
#define _DYNAMIK_ADGR_GRAPHICS_OBJECT_H

#include "../GraphicsComponent.h"

namespace Dynamik {
	namespace ADGR {
		class GraphicsObject : public GraphicsComponent {
		public:
			GraphicsObject() {}
			~GraphicsObject() {}

			void initialize() override final;
			void terminate() override final;
		};
	}
}

#endif // !_DYNAMIK_ADGR_GRAPHICS_OBJECT_H
