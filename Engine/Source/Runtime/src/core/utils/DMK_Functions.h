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
		DMK_API int min(int first, int second);
		DMK_API UI32 min(UI32 first, UI32 second);
		DMK_API float min(float first, float second);

		// var = utils::max(first, secons);
		template<class Alloc>
		DMK_API Alloc max(Alloc first, Alloc second);
		DMK_API int max(int first, int second);
		DMK_API UI32 max(UI32 first, UI32 second);
		DMK_API float max(float first, float second);

		// var = utils::swap(&first, &secons);
		template<class Alloc>
		DMK_API void swap(Alloc* first, Alloc* second);
		DMK_API void swap(int* first, int* second);
		DMK_API void swap(UI32* first, UI32* second);
		DMK_API void swap(float* first, float* second);
		DMK_API void swap(char* first, char* second);
		DMK_API void swap(CCPTR* first, CCPTR* second);
		DMK_API void swap(std::string* first, std::string* second);

		// var = utils::factorial(value);
		DMK_API long double factorial(long double value);

		// var = utils::power(value);
		DMK_API long double power(double base, double power);

		// var = utils::square(value);
		DMK_API long double square(double base);

		// var = utils::splitLine(line, character);
		DMK_API std::vector<std::string> splitLine(std::string_view line, char splitBy);

		// var = rand();
		DMK_API uint32_t rand(uint32_t from = 0, uint32_t to = 100);

		DMK_API std::vector<UI32> bubbleSort(std::vector<UI32> data);
	}
}

#endif	// !_DYNAMIK_FUNCTIONS_H
