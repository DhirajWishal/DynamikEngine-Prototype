#pragma once
#ifndef _DYNAMIK_EXPERIMENTAL_SINGLETON_H
#define _DYNAMIK_EXPERIMENTAL_SINGLETON_H

#include "Platform.h"

namespace Dynamik {
	namespace Experimental {
		class DMK_API Singleton {
			Singleton() {}
			Singleton(const Singleton&) = delete;
		public:
			~Singleton() {}
			static void printHello();

		private:
			void _hello();
			static Singleton& get();
		};
	}
}

#endif // !_DYNAMIK_EXPERIMENTAL_SINGLETON_H
