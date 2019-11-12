#pragma once

#ifndef _DYNAMIK_CENTRAL_DATA_HUB_DATATYPES_H
#define _DYNAMIK_CENTRAL_DATA_HUB_DATATYPES_H

#include <any>

namespace Dynamik {
	typedef unsigned int		uint32;
	typedef int					int32;

	typedef signed char			int8;
	typedef short				int16;
	typedef long long			int64;
	typedef unsigned char		uint8;
	typedef unsigned short		uint16;
	typedef unsigned long long	uint64;

	typedef const char* ccp;
	typedef void* pointer;

	typedef std::any			var;
}

#endif
