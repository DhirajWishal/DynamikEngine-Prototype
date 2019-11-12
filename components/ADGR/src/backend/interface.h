#pragma once

#ifdef DMK_PLATFORM_WINDOWS
//#define ADGR_API
#ifdef DMK_BUILD_DLL
#define ADGR_API __declspec(dllexport)

#else
#define ADGR_API __declspec(dllimport)

#endif
#else
#error Dynamik Engine only supports MS Windows!

#endif
