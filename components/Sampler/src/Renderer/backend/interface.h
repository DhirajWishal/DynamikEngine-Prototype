#pragma once

#ifdef DMK_PLATFORM_WINDOWS
//#define SMP_API 
#ifdef DMK_BUILD_DLL
#define SMP_API __declspec(dllexport)

#else
#define SMP_API __declspec(dllimport)

#endif
#else
#error Dynamik Engine only supports MS Windows!

#endif
