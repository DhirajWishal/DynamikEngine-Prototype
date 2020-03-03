#pragma once
#ifndef _DYNAMIK_PLATFORM_WINDOWS_DLL_INTERFACE_H
#define _DYNAMIK_PLATFORM_WINDOWS_DLL_INTERFACE_H

#ifdef DMK_PLATFORM_WINDOWS
#ifdef DMK_BUILD_DLL
#define DMK_API __declspec(dllexport)

#define DMK_CORE

#elif defined(DMK_BUILD_LIB)
#define DMK_API

#else
#define DMK_API __declspec(dllimport)

#endif
#else
#error Dynamik Engine only supports MS Windows!

#endif

#endif // !_DYNAMIK_PLATFORM_WINDOWS_DLL_INTERFACE_H
