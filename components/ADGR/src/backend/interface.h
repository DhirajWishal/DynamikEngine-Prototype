#pragma once

#ifndef _DYNAMIK_ADGR_INTERFACE_H
#define _DYNAMIK_ADGR_INTERFACE_H

#ifdef DMK_PLATFORM_WINDOWS
#ifdef DMK_BUILD_DLL
#define ADGR_API __declspec(dllexport)

#else
#define ADGR_API __declspec(dllimport)

#endif
#else
#error Dynamik Engine only supports MS Windows!

#endif

#endif	// !_DYNAMIK_ADGR_INTERFACE_H
