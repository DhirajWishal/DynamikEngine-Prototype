#pragma once

#ifdef DMK_PLATFORM_WINDOWS
#ifdef DMK_BUILD_DLL
#define DMK_API __declspec(dllexport)

#else
#define DMK_API __declspec(dllimport)

#endif
#else
#error Dynamik Engine only supports MS Windows!

#endif

#define BIT(x) (1 << x)
