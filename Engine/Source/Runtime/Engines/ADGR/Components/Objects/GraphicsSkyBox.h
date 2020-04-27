#pragma once
#ifndef _DYNAMIK_ADGR_GRAPHICS_SKYBOX_H
#define _DYNAMIK_ADGR_GRAPHICS_SKYBOX_H

#include "../GraphicsComponent.h"

namespace Dynamik {
	namespace ADGR {
		class GraphcisSkyBox : public GraphicsComponent {
		public:
			GraphcisSkyBox() {}
			~GraphcisSkyBox() {}

			void initialize() override final;
			void terminate() override final;
		};
	}
}

#endif // !_DYNAMIK_ADGR_GRAPHICS_SKYBOX_H
