#pragma once

#ifndef _DYNAMIK_CENTRAL_DATA_HUB_CONSTANTS_H
#define _DYNAMIK_CENTRAL_DATA_HUB_CONSTANTS_H

#define DMK_RESULT_SUCCESSFUL		1
#define DMK_RESULT_UNSUCCESSFUL		0

#ifdef DMK_FULL_MEMORY_ALIGN
#define DMK_MEMORY_ALIGN			8	// 8 bytes (64 bit)

#else
#define DMK_MEMORY_ALIGN			4	// 4 bytes (32 bit)

#endif

#define DMK_MEMORY_USE_MALLOC
//#define DMK_MEMORY_USE_NEW

#endif
