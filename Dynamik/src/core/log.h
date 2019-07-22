#pragma once
#pragma warning(disable:4996)

#include "dmkafx.h"
#include "core.h"

#if defined(DMK_PLATFORM_WINDOWS)

typedef const char* ccp;

namespace Dynamik {

	enum Calls {
		INFO_L,
		WARN_L,
		ERROR_L,
		FATAL_L,
		CORE_INFO_L
	};

	class DMK_API Log {
	public:
		inline static void LOG(int, ccp);
		inline static void INFO_LOG(ccp);
		inline static void WARN_LOG(ccp);
		inline static void ERROR_LOG(ccp);
		inline static void FATAL_LOG(ccp, ccp, int);
		inline static void CORE_LOG(ccp);

	private:
		inline static void changeToColor(int);
	};

}

/* GLOBAL INIT LOG MACRO */
//#define DMK_INIT_LOGGER ::Dynamik::Log::init()

/* CLIENT LOG MACROS */
#define DMK_INFO(...) ::Dynamik::Log::INFO_LOG(__VA_ARGS__)
#define DMK_WARN(...) ::Dynamik::Log::WARN_LOG(__VA_ARGS__)
#define DMK_ERROR(...) ::Dynamik::Log::ERROR_LOG(__VA_ARGS__)
#define DMK_FATAL(...) ::Dynamik::Log::FATAL_LOG(__VA_ARGS__, __FILE__, __LINE__)

#define DMK_LOGGER(...) 

/* CORE LOG MACROS */
#define DMK_CORE_INFO(...) ::Dynamik::Log::INFO_LOG(__VA_ARGS__)
#define DMK_CORE_WARN(...) ::Dynamik::Log::WARN_LOG(__VA_ARGS__)
#define DMK_CORE_ERROR(...) ::Dynamik::Log::ERROR_LOG(__VA_ARGS__)
#define DMK_CORE_FATAL(...) ::Dynamik::Log::FATAL_LOG(__VA_ARGS__, __FILE__, __LINE__)
#define DMK_CORE_MSG(...) ::Dynamik::Log::CORE_LOG(__VA_ARGS__)

// ----------
#endif