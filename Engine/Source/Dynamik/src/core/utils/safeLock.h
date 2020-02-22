#pragma once

#ifndef _DYNAMIK_SAFE_LOCK_H
#define _DYNAMIK_SAFE_LOCK_H

#include "core/core.h"
#include "core/utils/DMK_Error.h"

namespace Dynamik {
	namespace utils {
		// RAII support for Dynamik Engine
		class DMK_API safeLock {
		public:
			safeLock();
			~safeLock();
		};
	}
}

#endif // !_DYNAMIK_SAFE_LOCK_H
