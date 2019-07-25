#pragma once

/*
  Linked List data structure for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 linkedList.h file.
*/

namespace Dynamik {
	namespace utils {

		/* Singly Linked List */
		template<class T>
		class sLnkLst {
		public:
			T data = nullptr;

			sLnkLst* next;
		};

		/* Doubly Linked List */
		template<class T>
		class dLnkLst {
		public:
			T data = nullptr;

			dLnkLst* next;
			dLnkLst* previous;
		};
	}
}