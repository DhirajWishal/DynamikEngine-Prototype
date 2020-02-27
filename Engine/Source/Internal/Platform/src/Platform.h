#pragma once

#ifndef _DYNAMIK_PLATFORM_H
#define _DYNAMIK_PLATFORM_H

#if defined DMK_PLATFORM_WINDOWS
#include "core/Windows.h"

// ----------
#elif defined DMK_PLATFORM_IOS

// ----------
#elif defined DMK_PLATFORM_LINUX

// ----------
#elif
#error Unsupported Platform!
// ----------
#endif

#endif // !_DYNAMIK_PLATFORM_H
