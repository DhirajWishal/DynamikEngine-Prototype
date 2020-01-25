#pragma once

#ifndef _DYNAMIK_NETWORKING_INTERFACE_H
#define _DYNAMIK_NETWORKING_INTERFACE_H

#ifdef DMK_PLATFORM_WINDOWS
#ifdef DMK_BUILD_DLL
#define NETW_API __declspec(dllexport)

#else
#define NETW_API __declspec(dllimport)

#endif
#else
#error Dynamik Engine only supports MS windows!

#endif

#endif // !_DYNAMIK_NETWORKING_INTERFACE_H

