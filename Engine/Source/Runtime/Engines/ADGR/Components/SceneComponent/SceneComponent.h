#pragma once
#ifndef _DYNAMIK_ADGR_SCENE_COMPONENT_H
#define _DYNAMIK_ADGR_SCENE_COMPONENT_H

#include "Objects/InternalFormat/InternalFormat.h"

namespace Dynamik {
	namespace ADGR {
		class SceneComponent {
		public:
			SceneComponent() {}
			virtual ~SceneComponent() {}

		protected:
			POINTER<InternalFormat> myInternalFormat;
		};
	}
}

#endif // !_DYNAMIK_ADGR_SCENE_COMPONENT_H
