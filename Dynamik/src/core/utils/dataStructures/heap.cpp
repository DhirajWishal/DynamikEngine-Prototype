/*
  Heap data structure for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 heap.cpp file.
*/

#include "dmkafx.h"
#include "heap.h"
#include "core/utils/DMK_Functions.h"

namespace Dynamik {
	namespace utils {

		/* ---------- ########## ////////// MIN HEAP \\\\\\\\\\ ########## ---------- */
		template<class Alloc>
		minHeap<Alloc>::minHeap(uint32 size) {
			memset(&arr, nullptr, size);
		}

		template<class Alloc>
		void minHeap<Alloc>::insert(Alloc value) {
			arr[myIndex] = value;

			for (auto i = myIndex; i <= 0; i--)
				if (arr[i] <= arr[getParent(i)])
					swap(arr[i], arr[getParent(i)]);

			myIndex++;
		}

		template<class Alloc>
		void minHeap<Alloc>::pop(Alloc value) {
			arr[0] = 0;

			swap(arr[0], arr[myIndex - 1]);

			for (auto i = 0; i <= myIndex; i++)
			{
				if (arr[i] >= arr[getLeft(i)])
					swap(arr[i], arr[getLeft(i)]);
				else if (arr[i] >= arr[getRight(i)])
					swap(arr[i], arr[getLeft(i)]);
			}
		}

		template<class Alloc>
		uint32 minHeap<Alloc>::getLeft(uint32 parent) {
			return 2 * parent + 1;
		}

		template<class Alloc>
		uint32 minHeap<Alloc>::getRight(uint32 parent) {
			return 2 * parent + 2;
		}

		template<class Alloc>
		uint32 minHeap<Alloc>::getParent(uint32 child) {
			return (child - 1) / 2;
		}

		/* ---------- ########## ////////// MAX HEAP \\\\\\\\\\ ########## ---------- */
		template<class Alloc>
		maxHeap<Alloc>::maxHeap(uint32 size) {
			memset(&arr, nullptr, size);
		}

		template<class Alloc>
		void maxHeap<Alloc>::insert(Alloc value) {

		}

		template<class Alloc>
		void minHeap<Alloc>::pop(Alloc value) {

		}
	}
}