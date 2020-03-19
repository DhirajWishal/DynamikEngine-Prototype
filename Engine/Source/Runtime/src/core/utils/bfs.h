#pragma once
#ifndef _DYNAMIK_BFS_H
#define _DYNAMIK_BFS_H

#include <vector>
#include <list>
#include "CentralDataHub.h"

namespace Dynamik {
	template<typename T>
	class BFS {
	public:
		BFS() {}
		BFS(UI32 size) : mySize(size) {}
		BFS(ARRAY<std::list<T>> adj) : myList(adj), mySize(adj.size()) {}
		~BFS() {}

		void add(T vertex, T edge) {
			if (myList.size() != mySize)
				myList.resize(mySize);

			myList[vertex].push_back(edge);
		}

	private:
		ARRAY<std::list<T>> myList = {};
		UI32 mySize = 0;
	};
}

#endif // !_DYNAMIK_BFS_H
