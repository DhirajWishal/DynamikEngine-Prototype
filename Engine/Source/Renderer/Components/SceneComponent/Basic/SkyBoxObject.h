#pragma once
#ifndef _DYNAMIK_RENDERER_SKYBOX_H
#define _DYNAMIK_RENDERER_SKYBOX_H

#include "../SceneComponent.h"

namespace Dynamik {
	namespace Renderer {
		class SkyBoxObject : public SceneComponent {
		public:
			SkyBoxObject() {}
			~SkyBoxObject() {}

			void initializeResources() override final;
			void initializeContext() override final;
			DMKUniformBufferData update(DMKCameraData cameraData) override final;
			void terminate() override final;
		};
	}
}

#endif // !_DYNAMIK_RENDERER_SKYBOX_H
