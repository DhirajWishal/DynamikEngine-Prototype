#pragma once

/*
  Linked List data structure for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 linkedList.h file.
*/

#ifndef _DYNAMIK_LINKED_LISTS_H
#define _DYNAMIK_LINKED_LISTS_H

#include "core/core.h"

namespace Dynamik {
	namespace utils {
		/* Singly Linked List */
		template<class T>
		class DMK_API sLnkLst {
		public:
			T data = nullptr;

			sLnkLst* next;
		};

		/* Doubly Linked List */
		template<class T>
		class DMK_API dLnkLst {
		public:
			T data = nullptr;

			dLnkLst* next;
			dLnkLst* previous;
		};
	}
}
#endif // !_DYNAMIK_LINKED_LISTS_H
