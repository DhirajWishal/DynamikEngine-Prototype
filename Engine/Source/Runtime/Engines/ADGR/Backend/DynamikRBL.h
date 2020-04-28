#pragma once
#ifndef _DYNAMIK_ADGR_RBL_H
#define _DYNAMIK_ADGR_RBL_H

#include "Vulkan/VulkanRBL.h"

namespace Dynamik {
	namespace ADGR {
		class DynamikRBL {
			DynamikRBL() {}
			~DynamikRBL() {}

			static DynamikRBL instance;
		public:
			static void initializeStageOne();
		};
	}
}

#endif // !_DYNAMIK_ADGR_RBL_H
