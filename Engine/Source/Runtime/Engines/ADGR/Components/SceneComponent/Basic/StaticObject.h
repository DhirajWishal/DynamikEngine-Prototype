#pragma once
#ifndef _DYNAMIK_ADGR_STATIC_OBJECT_H
#define _DYNAMIK_ADGR_STATIC_OBJECT_H

#include "../SceneComponent.h"

namespace Dynamik {
	namespace ADGR {
		class StaticObject : public SceneComponent {
		public:
			StaticObject() {}
			~StaticObject() {}

			void initializeResources() override final;
			void initializeContext() override final;
			DMKUniformBufferData update(DMKCameraData cameraData) override final;
			void terminate() override final;
		};
	}
}

#endif // !_DYNAMIK_ADGR_STATIC_OBJECT_H3
