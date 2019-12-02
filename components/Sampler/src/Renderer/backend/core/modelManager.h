#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_MODEL_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_MODEL_MANAGER_H

#include "core/Model.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class modelManager : public Model {
			public:
				modelManager() {}
				~modelManager() {}

				void loadModel(DMKModelLoadInfo info) override;
			};

			void loadModel(DMKModelLoadInfo info);
		}
	}
}

#endif	// !_DYNAMIK_ADGR_VULKAN_MODEL_MANAGER_H