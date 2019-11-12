#pragma once

/*
  Heap data structure for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 heap.h file.
*/

#include "core/core.h"
#include "CentralDataHub.h"

namespace Dynamik {
	namespace utils {
		/* Min Heap Data Structure */
		template<class Alloc>
		class minHeap {
		public:
			minHeap(uint32 size);
			~minHeap() {}

			void insert(Alloc value);
			void pop();

			Alloc getValue(uint32 index);
			std::vector<Alloc> sort();

		private:
			inline uint32 getLeft(uint32 parent);
			inline uint32 getRight(uint32 parent);
			inline uint32 getParent(uint32 child);

			std::vector<Alloc> arr;
			uint32 myIndex = 0;
		};

		/* Max Heap Data Structure */
		template<class Alloc>
		class maxHeap {
		public:
			maxHeap(uint32 size);
			~maxHeap() {}

			void insert(Alloc value);
			void pop();

			Alloc getValue(uint32 index);

		private:
			inline uint32 getLeft(uint32 parent);
			inline uint32 getRight(uint32 parent);
			inline uint32 getParent(uint32 child);

			std::vector<Alloc> arr;
			uint32 myIndex;
		};

		// Exporting/ Importing minHeap class
		minHeap<int>;
		minHeap<uint32>;
		minHeap<float>;
		minHeap<double>;

		// Exporting/ Importing maxHeap class
		maxHeap<int>;
		maxHeap<uint32>;
		maxHeap<float>;
		maxHeap<double>;
	}
}
