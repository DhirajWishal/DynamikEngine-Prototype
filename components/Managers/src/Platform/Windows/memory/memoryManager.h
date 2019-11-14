#pragma once

#ifndef _DYNAMIK_MANAGERS_WINDOWS_MEMORY_MANAGER_H
#define _DYNAMIK_MANAGERS_WINDOWS_MEMORY_MANAGER_H

#include "CentralDataHub.h"

namespace Dynamik {
	namespace manager {
		class allocator {
		public:
			allocator(uint32 size);
			~allocator();

			void* allocate(uint32 size, uint32 align, uint32 offset);

		private:
			pointer higherStack;
			pointer lowerStack;
			void* stackPool;

			uint32 totalSize;

			bool isHigh = false;
		};
	}
}

#endif // !_DYNAMIK_MANAGERS_MEMORY_MANAGER_H
