#pragma once

/*
  Heap data structure for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 heap.h file.
*/

#ifndef _DYNAMIK_HEAP_H
#define _DYNAMIK_HEAP_H

#include "core/core.h"
#include "CentralDataHub.h"

namespace Dynamik {
	namespace utils {
		/* Min Heap Data Structure */
		template<class Alloc>
		class minHeap {
		public:
			minHeap(UI32 size);
			~minHeap() {}

			void insert(Alloc value);
			void pop();

			Alloc getValue(UI32 index);
			ARRAY<Alloc> sort();

		private:
			inline UI32 getLeft(UI32 parent);
			inline UI32 getRight(UI32 parent);
			inline UI32 getParent(UI32 child);

			ARRAY<Alloc> arr;
			UI32 myIndex = 0;
		};

		/* Max Heap Data Structure */
		template<class Alloc>
		class maxHeap {
		public:
			maxHeap(UI32 size);
			~maxHeap() {}

			void insert(Alloc value);
			void pop();

			Alloc getValue(UI32 index);

		private:
			inline UI32 getLeft(UI32 parent);
			inline UI32 getRight(UI32 parent);
			inline UI32 getParent(UI32 child);

			ARRAY<Alloc> arr;
			UI32 myIndex;
		};

		// Exporting/ Importing minHeap class
		minHeap<int>;
		minHeap<UI32>;
		minHeap<float>;
		minHeap<double>;

		// Exporting/ Importing maxHeap class
		maxHeap<int>;
		maxHeap<UI32>;
		maxHeap<float>;
		maxHeap<double>;

		template<class TYPE>
		class Heap {
		public:
			Heap() {}
			Heap(UI32 size) {}
			~Heap() {}

			virtual void insert(TYPE value) {}
			virtual void pop() {}

			virtual TYPE getValue(UI32 index) { return TYPE(); }
		};
	}
}

#endif // !_DYNAMIK_HEAP_H
