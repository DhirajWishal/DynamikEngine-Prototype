/*
 Stand alone function library for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 DMK_Functions.cpp file.
*/

#include "dmkafx.h"
#include "DMK_Functions.h"

namespace Dynamik {
	namespace utils {
		/* ---------- MIN FUNCTIONS ---------- */
		// var = utils::min(first, secons);
		template<class Alloc>
		Alloc min(Alloc first, Alloc second)
		{
			if (first < second)
				return (Alloc)first;
			else
				return (Alloc)second;
		}

		// var = utils::min(first, secons);
		int32 min(int32 first, int32 second)
		{
			if (first < second)
				return (first);
			else
				return (second);
		}

		// var = utils::min(first, secons);
		uint32 min(uint32 first, uint32 second)
		{
			if (first < second)
				return (first);
			else
				return (second);
		}

		// var = utils::min(first, secons);
		float min(float first, float second)
		{
			if (first < second)
				return (first);
			else
				return (second);
		}

		/* ---------- MAX FUNCTIONS ---------- */
		// var = utils::max(first, secons);
		template<class Alloc>
		Alloc max(Alloc first, Alloc second)
		{
			if (first > second)
				return (Alloc)first;
			else
				return (Alloc)second;
		}

		// var = utils::max(first, secons);
		int32 max(int32 first, int32 second)
		{
			if (first > second)
				return first;
			else
				return second;
		}

		// var = utils::max(first, secons);
		uint32 max(uint32 first, uint32 second)
		{
			if (first > second)
				return (first);
			else
				return (second);
		}

		// var = utils::max(first, secons);
		float max(float first, float second)
		{
			if (first > second)
				return (first);
			else
				return (second);
		}

		/* ---------- SWAP FUNCTIONS ---------- */
		// var = utils::swap(&first, &secons);
		template<class Alloc>
		void swap(Alloc* first, Alloc* second)
		{
			Alloc* temp = first;
			first = second;
			second = temp;
		}

		// var = utils::swap(&first, &secons);
		void swap(int32* first, int32* second)
		{
			int32* temp = first;
			first = second;
			second = temp;
		}

		// var = utils::swap(&first, &secons);
		void swap(uint32* first, uint32* second)
		{
			uint32* temp = first;
			first = second;
			second = temp;
		}

		// var = utils::swap(&first, &secons);
		void swap(float* first, float* second)
		{
			float* temp = first;
			first = second;
			second = temp;
		}

		// var = utils::swap(&first, &secons);
		void swap(char* first, char* second)
		{
			char* temp = first;
			first = second;
			second = temp;
		}

		// var = utils::swap(&first, &secons);
		void swap(ccp* first, ccp* second)
		{
			ccp* temp = first;
			first = second;
			second = temp;
		}

		// var = utils::swap(&first, &secons);
		void swap(std::string* first, std::string* second)
		{
			std::string* temp = first;
			first = second;
			second = temp;
		}

		/* ---------- FACTORIAL FUNCTIONS ---------- */
		// var = utils::factorial(value);
		long double factorial(long double value)
		{
			if (value == 0 || value == 1) return 1;
			if (value == 2) return value;

			long double _result = 1;
			for (long double _counter = 1; _counter <= value; _counter++)
				_result *= _counter;

			return _result;
		}

		// var = utils::power(value);
		long double power(double base, double power)
		{
			if (power == 0) return 1;
			if (power == 1) return base;
			if (power == 2) return base * base;

			long double result = base;
			for (long int itr = 1; itr < power; itr++)
				result *= base;

			return result;
		}

		// var = utils::square(value);
		long double square(double base)
		{
			return power(base, 2);
		}
	}
}