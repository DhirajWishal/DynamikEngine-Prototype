#pragma once

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
