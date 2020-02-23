#pragma once

#ifndef _DYNAMIK_ERROR_H
#define _DYNAMIK_ERROR_H

namespace Dynamik {
	namespace utils {
#ifdef DMK_PLATFORM_WINDOWS
#define TRIGGER_ERROR_BREAKPOINT	__debugbreak()

#define ERROR_INFO					std::cout << __FILE__ << __LINE__ << std::endl

#define ERROR_PROMPT(...)			std::cout << __FILE__ << __LINE__ << (__VA_ARGS__) << std::endl

#define ERROR_CHECK(x)				if(!(x)) { ERROR_INFO; TRIGGER_ERROR_BREAKPOINT; }

#define ERROR_DEFINE(...)			std::cout << "(" << __FILE__ << ":" <<__LINE__ << ") ERROR:" <<\
									(__VA_ARGS__) << std::endl; TRIGGER_ERROR_BREAKPOINT

#define ASSERT(x)					if(!(x)) {						\
										ERROR_INFO;					\
										TRIGGER_ERROR_BREAKPOINT;	\
										}

		// ----------
#endif
	}
}

#endif // !_DYNAMIK_ERROR_H
