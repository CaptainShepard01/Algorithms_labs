#include <iostream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <stack>
#include <random>
#include <cmath>
#include <iomanip>
#include <exception>
#include <sstream>

#include "../Lab_5/WorldMap.h"
#include"../Lab_8/FibbonacciHeap.hxx"

#define ADDITIONAL_VERTEX_DATA = 0;

template<typename T>
struct vertex {
	static int id;

	T data;
	int number;

	vertex(T data) :
		number(id++),
		data(data)
	{};

	std::string getText();
};

template<typename T>
int vertex<T>::id = 0;

template<typename T>
class Graph {
private:
	std::vector<vertex<T>> vertices;
	std::vector<std::vector<std::pair<int, int>>> adjacencyList;

	std::vector<int> d;
	std::vector<int> pi;

	int minWeight;

	void initializeSingleSource(int source);
	void relax(int u, int v);
	void relax(int from, int to, FibbonacciHeap<std::pair<int, int>>& heap);

	void getGraphInfo(std::string& text);

	bool BellmanFord(int source);
	void Dijkstra(int source);

	void positiveEdges();
	void originEdges();
public:

	Graph();
	void addEdge(int from, int to, int weight);
	void addVertex(T data);

	void generateRandomGraph(const int& numberOfVertices);

	std::vector<std::vector<int>> Johnson();

	std::string getWebGraphviz(std::string graphName = "G");
};

void matrixPrint(std::vector<std::vector<int>> matrix) {
	for (int i = 0; i < matrix.size(); ++i) {
		for (int j = 0; j < matrix[i].size(); ++j) {
			std::cout << std::setw(3);
			if (matrix[i][j] != INT_MAX) {
				std::cout << matrix[i][j];
			}
			else {
				std::cout << "inf";
			}
			std::cout << ' ';
		}
		std::cout << '\n';
	}
}

int main() {
	//Graph<int> testGraph;

	/*for (int i = 0; i < 6; ++i) {
		testGraph.addVertex(i);
	}*/

	/*testGraph.addEdge(0, 1, 5);
	testGraph.addEdge(0, 2, 6);
	testGraph.addEdge(1, 4, 148);
	testGraph.addEdge(2, 3, 14);
	testGraph.addEdge(3, 2, 7);
	testGraph.addEdge(3, 4, 25);*/


	/*testGraph.addVertex(0);
	testGraph.addVertex(-1);
	testGraph.addVertex(-5);
	testGraph.addVertex(0);
	testGraph.addVertex(-4);

	testGraph.addEdge(0, 1, 3);
	testGraph.addEdge(0, 4, -4);
	testGraph.addEdge(0, 2, 8);
	testGraph.addEdge(1, 3, 1);
	testGraph.addEdge(1, 4, 7);
	testGraph.addEdge(2, 1, 4);
	testGraph.addEdge(3, 0, 2);
	testGraph.addEdge(3, 2, -5);
	testGraph.addEdge(4, 3, 6);*/


	for (int i = 0; i < 7; ++i) {
		Graph<int> testGraph;
		testGraph.generateRandomGraph(i);
		std::cout << testGraph.getWebGraphviz() << "\n\n";
		matrixPrint(testGraph.Johnson());
		std::cout << "\n\n";
	}

	/*Graph<int> testGraph;
	for (int i = 0; i < 5; ++i) {
		testGraph.addVertex(i);
	}
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			testGraph.addEdge(i, j, rand() % 20 - 5);
	std::cout << testGraph.getWebGraphviz() << "\n\n";*/

	return 0;
}

template<typename T>
std::string vertex<T>::getText() {
	std::stringstream result;

	result << "Data: " << data << " ID: " << number;

	return result.str();
}

template<typename T>
void Graph<T>::initializeSingleSource(int source) {
	d.assign(vertices.size(), INT_MAX);
	pi.assign(vertices.size(), -1);
	d[source] = 0;
}

template<typename T>
void Graph<T>::relax(int from, int to) {
	int weight = -1;
	for (int i = 0; i < adjacencyList[from].size(); ++i) {
		if (adjacencyList[from][i].first == to) {
			weight = adjacencyList[from][i].second;
			break;
		}
	}

	if (d[from] != INT_MAX && d[to] > d[from] + weight) {
		d[to] = d[from] + weight;
		pi[to] = from;
	}
}

template<typename T>
void Graph<T>::relax(int from, int to, FibbonacciHeap<std::pair<int, int>>& heap) {
	int weight = -1;
	for (int i = 0; i < adjacencyList[from].size(); ++i) {
		if (adjacencyList[from][i].first == to) {
			weight = adjacencyList[from][i].second;
			break;
		}
	}

	if (d[from] != INT_MAX && d[to] > d[from] + weight) {
		heap.fibHeapDecreaseKey({ d[to], to }, { d[from] + weight, to });
		d[to] = d[from] + weight;
		pi[to] = from;
	}
}

template<typename T>
void Graph<T>::getGraphInfo(std::string& text) {
	std::stringstream stream;

	for (int i = 0; i < vertices.size(); ++i) {
		stream << "\"" << vertices[i].getText() << "\";\n";
		for (int j = 0; j < adjacencyList[i].size(); ++j) {
			stream << "\"" << vertices[i].getText() << "\"" << " -> " << "\"" << vertices[adjacencyList[i][j].first].getText() << "\"" << " [ label = " << "\"" << adjacencyList[i][j].second << "\"" << " ];\n";
		}
	}

	text += stream.str();
}

template<typename T>
bool Graph<T>::BellmanFord(int source) {
	initializeSingleSource(source);

	minWeight = INT_MAX;

	for (int i = 0; i < vertices.size() - 1; ++i) {
		for (int from = 0; from < adjacencyList.size(); ++from) {
			for (int to = 0; to < adjacencyList[from].size(); ++to) {
				//Updating minimal weight to adjust weights for Dijkstra algorithm
				if (adjacencyList[from][to].second < minWeight) {
					minWeight = adjacencyList[from][to].second;
				}
				relax(from, adjacencyList[from][to].first);
			}
		}
	}

	for (int from = 0; from < adjacencyList.size(); ++from) {
		for (int to = 0; to < adjacencyList[from].size(); ++to) {
			int weight = INT_MIN;
			for (int i = 0; i < adjacencyList[from].size(); ++i) {
				if (adjacencyList[from][i].first == to) {
					weight = adjacencyList[from][i].second;
					break;
				}
			}
			if (weight != INT_MIN && d[to] > d[from] + weight)
				return false;
		}
	}

	return true;
}

template<typename T>
void Graph<T>::Dijkstra(int source) {
	initializeSingleSource(source);

	std::vector<int> S;

	FibbonacciHeap<std::pair<int, int>> Q;
	for (int i = 0; i < d.size(); ++i) {
		Q.fibbonacciInsert({ d[i], i });
	}

	while (Q.getMin() != nullptr) {
		int u = Q.fibHeapExtractMin()->key.second;

		S.push_back(u);

		for (int to = 0; to < adjacencyList[u].size(); ++to) {
			relax(u, adjacencyList[u][to].first, Q);
		}
	}
}

template<typename T>
void Graph<T>::positiveEdges() {
	if (minWeight >= 0)
		return;

	for (int from = 0; from < adjacencyList.size(); ++from) {
		for (int to = 0; to < adjacencyList[from].size(); ++to) {
			adjacencyList[from][to].second += minWeight;
		}
	}
}

template<typename T>
void Graph<T>::originEdges() {
	if (minWeight >= 0)
		return;

	for (int from = 0; from < adjacencyList.size(); ++from) {
		for (int to = 0; to < adjacencyList[from].size(); ++to) {
			adjacencyList[from][to].second -= minWeight;
		}
	}
}

template<typename T>
Graph<T>::Graph() {

}

template<typename T>
void Graph<T>::addVertex(T data) {
	vertex<T> newVertex(data);

	adjacencyList.push_back({});

	vertices.push_back(newVertex);
}

template<typename T>
void Graph<T>::generateRandomGraph(const int& numberOfVertices) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<> dist(-5, 35);

	for (int i = 0; i < numberOfVertices; ++i) {
		addVertex(i);
	}

	for (int i = 0; i < numberOfVertices; ++i) {
		for (int j = 0; j < numberOfVertices; ++j) {
			if (rand() % 10 < 5) {
				addEdge(i, j, dist(mt));
			}
		}
	}
}

template<typename T>
std::vector<std::vector<int>> Graph<T>::Johnson() {
	std::vector<std::vector<int>> result(vertices.size(), std::vector<int>(vertices.size()));

	Graph<T> expanded = *this;

	expanded.addVertex(0);

	for (int i = 0; i < expanded.vertices.size() - 1; ++i) {
		expanded.addEdge(expanded.vertices.size() - 1, i, 0);
	}

	if (expanded.BellmanFord(expanded.vertices.size() - 1) == false) {
		std::cout << "\nThere is a negative weight cycle in this Graph!\n\n";
		return std::vector<std::vector<int>>();
	}
	else {
		for (int from = 0; from < expanded.adjacencyList.size(); ++from) {
			for (int to = 0; to < expanded.adjacencyList[from].size(); ++to) {
				expanded.adjacencyList[from][to].second =
					expanded.adjacencyList[from][to].second +
					expanded.d[from] -
					expanded.d[expanded.adjacencyList[from][to].first];
			}
		}
		expanded.adjacencyList.pop_back();
		adjacencyList = expanded.adjacencyList;

		for (int i = 0; i < vertices.size(); ++i) {
			Dijkstra(i);
			for (int j = 0; j < vertices.size(); ++j) {
				if (d[j] == INT_MAX || expanded.d[j] == INT_MAX || expanded.d[i] == INT_MAX) {
					result[i][j] = INT_MAX;
				}
				else {
					result[i][j] = d[j] + expanded.d[j] - expanded.d[i];
				}
			}
		}
	}

	return result;
}

template<typename T>
std::string Graph<T>::getWebGraphviz(std::string graphName) {
	std::string graphText = "digraph " + graphName + " {\n";

	getGraphInfo(graphText);

	return graphText += "}";
}

template<typename T>
void Graph<T>::addEdge(int from, int to, int weight) {
	if (adjacencyList.size() <= from || adjacencyList.size() <= to)
		return;

	for (int i = 0; i < adjacencyList[from].size(); ++i) {
		if (adjacencyList[from][i].first == to) {
			if (from == to) {
				adjacencyList[from][i].second = 0;
			}
			else {
				adjacencyList[from][i].second = weight;
			}
			return;
		}
	}

	if (from == to) {
		adjacencyList[from].push_back({ to, 0 });
	}
	else {
		adjacencyList[from].push_back({ to, weight });
	}
}