#pragma once

#include "core/core.h"
#include "core/utils/DMK_Error.h"

namespace Dynamik {

	// RAII support for Dynamik Engine
	class DMK_API safeLock {
	public:
		safeLock();
		~safeLock();
	};

}
