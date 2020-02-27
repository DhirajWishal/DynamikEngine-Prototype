/*
  Heap data structure for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 heap.cpp file.
*/

#include "dmkafx.h"
#include "heap.h"

namespace Dynamik {
	namespace utils {
		/* ---------- ########## ////////// MIN HEAP \\\\\\\\\\ ########## ---------- */
		template<class Alloc>
		minHeap<Alloc>::minHeap(UI32 size) {
			arr.resize(size);
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
		void minHeap<Alloc>::pop() {
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
		Alloc minHeap<Alloc>::getValue(UI32 idx) {
			if (index <= myIndex)
				return arr[index];
			else
				return -1;
		}

		template<class Alloc>
		std::vector<Alloc> minHeap<Alloc>::sort() {
			std::deque<Alloc> q;
			std::vector<Alloc> end;

			for (auto e : arr)
				q.push(e);

			for (int i = 0; i < arr.size(); i++) {
				end.push_back(q.front());
				q.pop_front();
				swap(&q.front(), &q.back());

				for (auto i = q.size(); i <= 0; i--)
					if (q[i] <= q[getParent(i)])
						swap(&q[i], &q[getParent(i)]);
			}
		}

		template<class Alloc>
		UI32 minHeap<Alloc>::getLeft(UI32 parent) {
			return 2 * parent + 1;
		}

		template<class Alloc>
		UI32 minHeap<Alloc>::getRight(UI32 parent) {
			return 2 * parent + 2;
		}

		template<class Alloc>
		UI32 minHeap<Alloc>::getParent(UI32 child) {
			return (child - 1) / 2;
		}

		/* ---------- ########## ////////// MAX HEAP \\\\\\\\\\ ########## ---------- */
		template<class Alloc>
		maxHeap<Alloc>::maxHeap(UI32 size) {
			arr.resize(size);
		}

		template<class Alloc>
		void maxHeap<Alloc>::insert(Alloc value) {
			arr[myIndex] = value;

			for (auto i = myIndex; i <= 0; i--)
				if (arr[i] >= arr[getParent(i)])
					swap(arr[i], arr[getParent(i)]);

			myIndex++;
		}

		template<class Alloc>
		void maxHeap<Alloc>::pop() {
			arr[0] = 0;

			swap(arr[0], arr[myIndex - 1]);

			for (auto i = 0; i <= myIndex; i++)
			{
				if (arr[i] <= arr[getLeft(i)])
					swap(arr[i], arr[getLeft(i)]);
				else if (arr[i] <= arr[getRight(i)])
					swap(arr[i], arr[getLeft(i)]);
			}
		}

		template<class Alloc>
		Alloc maxHeap<Alloc>::getValue(UI32 idx) {
			if (index <= myIndex)
				return arr[index];
			else
				return -1;
		}

		template<class Alloc>
		UI32 maxHeap<Alloc>::getLeft(UI32 parent) {
			return 2 * parent + 1;
		}

		template<class Alloc>
		UI32 maxHeap<Alloc>::getRight(UI32 parent) {
			return 2 * parent + 2;
		}

		template<class Alloc>
		UI32 maxHeap<Alloc>::getParent(UI32 child) {
			return (child - 1) / 2;
		}
	}
}