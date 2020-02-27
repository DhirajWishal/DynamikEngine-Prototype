#pragma once
#ifndef _DYNAMIK_GRAPH_H
#define _DYNAMIK_GRAPH_H

#include <vector>
#include <list>
#include <CentralDataHub.h>

namespace Dynamik {
	template<typename TYPE>
	class Graph {
	public:
		Graph() {}

		//virtual std::list<TYPE> BFS(TYPE start) { return std::list<TYPE>(); }
		virtual std::list<TYPE> DFS(TYPE start) { return std::list<TYPE>(); }
		virtual std::list<TYPE> dijkstra(TYPE start, TYPE end) { return std::list<TYPE>(); }

	protected:
		UI32 mySize = 0;
	};

	template<typename TYPE>
	class graph2D : public Graph<TYPE> {
	public:
		graph2D() {}
		graph2D(std::vector<std::vector<TYPE>> graph) : myGraph(graph) { mySize = myGraph.size(); }
		~graph2D() {}

		std::vector<TYPE> BFS(TYPE start) {
			std::vector<TYPE> finals = {};
			visited.resize(mySize, false);
			visited[start] = true;

			std::list<TYPE> queue = {};
			queue.push_back(start);

			std::vector<TYPE>::iterator itr;

			while (!queue.empty()) {
				start = queue.front();
				finals.push_back(start);
				queue.pop_front();

				for (itr = myGraph[start].begin(); itr != myGraph[start].end(); itr++) {
					if (!visited.at(*itr)) {
						visited[*itr] = true;
						queue.push_back(*itr);
					}
				}
			}

			return finals;
		}
		std::list<TYPE> DFS(TYPE start) override { return std::list<TYPE>(); }

	private:
		std::vector<std::vector<TYPE>> myGraph = { {} };
		std::vector<B1> visited = {};
	};
}

#endif // !_DYNAMIK_GRAPH_H