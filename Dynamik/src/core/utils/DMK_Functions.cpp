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
		/* ---------- MAX FUNCTIONS ---------- */
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

		/* ---------- MIN FUNCTIONS ---------- */
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

		/* ---------- SWAP FUNCTIONS ---------- */
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

	}
}
