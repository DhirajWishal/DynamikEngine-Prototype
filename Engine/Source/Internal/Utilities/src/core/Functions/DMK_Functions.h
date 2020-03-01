#pragma once

/*
  Stand alone function library for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 DMK_Functions.cpp file.
*/

#ifndef _DYNAMIK_FUNCTIONS_H
#define _DYNAMIK_FUNCTIONS_H

#include "CentralDataHub.h"
#include "Platform.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace Dynamik {
	namespace utils {
		// var = utils::min(first, secons);
		template<class Alloc>
		Alloc min(Alloc first, Alloc second);
		int min(int first, int second);
		UI32 min(UI32 first, UI32 second);
		float min(float first, float second);

		// var = utils::max(first, secons);
		template<class Alloc>
		Alloc max(Alloc first, Alloc second);
		int max(int first, int second);
		UI32 max(UI32 first, UI32 second);
		float max(float first, float second);

		// var = utils::swap(&first, &secons);
		template<class Alloc>
		void swap(Alloc* first, Alloc* second);
		void swap(int* first, int* second);
		void swap(UI32* first, UI32* second);
		void swap(float* first, float* second);
		void swap(char* first, char* second);
		void swap(CCPTR* first, CCPTR* second);
		void swap(std::wstring* first, std::wstring* second);

		// var = utils::factorial(value);
		long double factorial(long double value);

		// var = utils::power(value);
		long double power(double base, double power);

		// var = utils::square(value);
		long double square(double base);

		// var = utils::splitLine(line, character);
		std::vector<std::wstring> splitLine(std::wstring_view line, char splitBy);

		// var = rand();
		uint32_t rand(uint32_t from = 0, uint32_t to = 100);

		std::vector<UI32> bubbleSort(std::vector<UI32> data);
	}

	std::string WSTRtoCSTR(std::wstring string);
	std::wstring CSTRtoWSTR(std::string string);
}

#endif	// !_DYNAMIK_FUNCTIONS_H
