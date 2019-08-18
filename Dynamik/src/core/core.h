#pragma once

#ifdef DMK_PLATFORM_WINDOWS
#ifdef DMK_BUILD_DLL
#define DMK_API __declspec(dllexport)

#define DMK_CORE

#else
#define DMK_API __declspec(dllimport)

#endif
#else
#error Dynamik Engine only supports MS Windows!

#endif

#define BIT(x) (1 << x)

#define DMK_SUCCESS		0
#define DMK_FAIL		-1
