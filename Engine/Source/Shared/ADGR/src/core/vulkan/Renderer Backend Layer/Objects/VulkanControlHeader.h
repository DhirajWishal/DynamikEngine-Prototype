#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_CONTROL_HEADER_H
#define _DYNAMIK_ADGR_VULKAN_CONTROL_HEADER_H

#include "DataTypesLib/Public/datatypes.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			static B1 pushDescriptorsSupported = false;
			static B1 checkpointsSupported = false;
			static B1 meshShadingSupported = false;
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_CONTROL_HEADER_H
