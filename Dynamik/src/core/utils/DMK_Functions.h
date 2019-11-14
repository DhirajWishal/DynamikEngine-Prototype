#pragma once

/*
  Stand alone function library for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 DMK_Functions.cpp file.
*/

#ifndef _DYNAMIK_FUNCTIONS_H
#define _DYNAMIK_FUNCTIONS_H

#include "core/core.h"
#include "CentralDataHub.h"

namespace Dynamik {
	namespace utils {
		// var = utils::min(first, secons);
		template<class Alloc>
		DMK_API Alloc min(Alloc first, Alloc second);
		DMK_API int32 min(int32 first, int32 second);
		DMK_API uint32 min(uint32 first, uint32 second);
		DMK_API float min(float first, float second);

		// var = utils::max(first, secons);
		template<class Alloc>
		DMK_API Alloc max(Alloc first, Alloc second);
		DMK_API int32 max(int32 first, int32 second);
		DMK_API uint32 max(uint32 first, uint32 second);
		DMK_API float max(float first, float second);

		// var = utils::swap(&first, &secons);
		template<class Alloc>
		DMK_API void swap(Alloc* first, Alloc* second);
		DMK_API void swap(int32* first, int32* second);
		DMK_API void swap(uint32* first, uint32* second);
		DMK_API void swap(float* first, float* second);
		DMK_API void swap(char* first, char* second);
		DMK_API void swap(ccp* first, ccp* second);
		DMK_API void swap(std::string* first, std::string* second);

		// var = utils::factorial(value);
		DMK_API long double factorial(long double value);
	}
}

#endif	// !_DYNAMIK_FUNCTIONS_H
