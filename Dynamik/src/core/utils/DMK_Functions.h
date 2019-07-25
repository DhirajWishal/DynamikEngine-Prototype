#pragma once

/*
  Stand alone function library for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 DMK_Functions.cpp file.
*/

namespace Dynamik {
	namespace utils {
		// var = utils::max(first, secons);
		template<typename T>
		T max(T first, T second);

		// var = utils::min(first, secons);
		template<typename T>
		T min(T first, T second);

		// var = utils::swap(&first, &secons);
		template<typename T>
		void swap(T* first, T* second);
	}
}