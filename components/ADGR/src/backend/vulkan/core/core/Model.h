#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_MODEL_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_MODEL_H

#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class Model : public Core {
			public:
				Model() {}
				virtual ~Model() {}

				virtual void loadModel(DMKModelLoadInfo info) {}
			};
		}
	}
}

#endif	// !_DYNAMIK_ADGR_VULKAN_RENDERER_MODEL_H
