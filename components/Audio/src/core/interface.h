#pragma once

#ifdef DMK_PLATFORM_WINDOWS
#define AUDIO_API
#ifdef DMK_BUILD_DLL_CANCEL
#define AUDIO_API __declspec(dllexport)

#else 
//#define AUDIO_API __declspec(dllimport)

#endif
#endif