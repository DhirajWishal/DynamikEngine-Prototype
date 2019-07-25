#pragma once

/*
  Heap data structure for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 heap.h file.
*/

#include "core/utils/DMK_DataTypes.h"

namespace Dynamik {
	namespace utils {
		/* Min Heap Data Structure */
		template<class Alloc>
		class minHeap {
		public:
			minHeap(uint32 size);
			~minHeap() {}

			void insert(Alloc value);
			void pop(Alloc value);

		private:
			uint32 getLeft(uint32 parent);
			uint32 getRight(uint32 parent);
			uint32 getParent(uint32 child);

			Alloc arr[];

			uint32 myIndex = 0;
		};

		/* Max Heap Data Structure */
		template<class Alloc>
		class maxHeap {
		public:
			maxHeap(uint32 size);
			~maxHeap() {}

			void insert(Alloc value);
			void pop(Alloc value);

		private:
			Alloc arr[];

			uint32 myIndex;
		};

	}
}
