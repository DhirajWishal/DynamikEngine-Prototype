#pragma once

#ifndef _DYNAMIK_DEBUGGER_LOG_H
#define _DYNAMIK_DEBUGGER_LOG_H

#pragma warning(disable:4996)

#if defined(DMK_PLATFORM_WINDOWS)

#include <string>

namespace Dynamik {
	enum Calls {
		INFO_L,
		WARN_L,
		ERROR_L,
		FATAL_L,
		CORE_INFO_L
	};

	void LOG(int, std::string);
	void INFO_LOG(std::string);
	void WARN_LOG(std::string);
	void ERROR_LOG(std::string);
	void FATAL_LOG(std::string, std::string, int);
	void CORE_LOG(std::string);
}

#ifdef DMK_DEBUG
/* GLOBAL INIT LOG MACRO */

/* CLIENT LOG MACROS */
#define DMK_FATAL_FORMAT()

#define DMK_INFO(...) ::Dynamik::INFO_LOG(##__VA_ARGS__)
#define DMK_WARN(...) ::Dynamik::WARN_LOG(##__VA_ARGS__)
#define DMK_ERROR(...) ::Dynamik::ERROR_LOG(##__VA_ARGS__)
#define DMK_FATAL(...) {															\
							::Dynamik::FATAL_LOG(##__VA_ARGS__, __FILE__, __LINE__); \
							__debugbreak();											\
						}

#define DMK_LOGGER(...)

/* CORE LOG MACROS */
#define DMK_CORE_INFO(...) ::Dynamik::INFO_LOG(##__VA_ARGS__)
#define DMK_CORE_WARN(...) ::Dynamik::WARN_LOG(##__VA_ARGS__)
#define DMK_CORE_ERROR(...) ::Dynamik::ERROR_LOG(##__VA_ARGS__)
#define DMK_CORE_FATAL(...) {															\
								::Dynamik::FATAL_LOG(##__VA_ARGS__, __FILE__, __LINE__); \
								__debugbreak();											\
							}

#define DMK_CORE_MSG(...) ::Dynamik::CORE_LOG(##__VA_ARGS__)

// assertions
#define DMK_CORE_ASSERT(x, ...)		{										\
										if(!(x)) {							\
											 DMK_CORE_ERROR(__VA_ARGS__);	\
											 __debugbreak();				\
										}									\
									}

#else
#define DMK_INFO(...)
#define DMK_WARN(...)
#define DMK_ERROR(...)
#define DMK_FATAL(...)

#define DMK_LOGGER(...)

/* CORE LOG MACROS */
#define DMK_CORE_INFO(...)
#define DMK_CORE_WARN(...)
#define DMK_CORE_ERROR(...)
#define DMK_CORE_FATAL(...)
#define DMK_CORE_MSG(...)

#define DMK_CORE_ASSERT(x, ...)

// ----------
#endif

// ----------
#endif

#endif // !_DYNAMIK_DEBUGGER_LOG_H
