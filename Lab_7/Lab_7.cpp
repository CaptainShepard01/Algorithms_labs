﻿#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <cmath>

struct WorldMap {
	std::string country;
	std::string city;

	WorldMap(std::string country, std::string city)
		:country(country),
		city(city) {};

	WorldMap(std::string city)
		:city(city) {};

	WorldMap() {};

	bool operator <(const WorldMap& rhs) const {
		return this->city < rhs.city;
	}

	bool operator >(const WorldMap& rhs)const {
		return this->city > rhs.city;
	}

	bool operator ==(const WorldMap& rhs)const {
		return this->city == rhs.city;
	}

	bool operator <=(const WorldMap& rhs)const {
		return this->city <= rhs.city;
	}
};

std::ostream& operator<<(std::ostream& os, const WorldMap& v) {
	os << '(';

	if (v.city != "") {
		os << " Country: " << v.country << ", City: " << v.city;
		//os << v.city;
	}

	os << " )";
	return os;
}

#define MIN_INFINITY WorldMap("");
#define NUMERIC_MIN_INFINITY std::numeric_limits<int>::min();

template<typename T>
struct Node {
	T key;
	Node* parent = nullptr;
	Node* child = nullptr;
	Node* sibling = nullptr;
	int degree = 0;

	Node(T key) :key(key) {}
	Node() {};
};

template <typename T>
class BinomialHeap {
private:
	Node<T>* head = nullptr;

	void getGraphInfo(Node<T>* x, std::string& text) {
		if (x != nullptr) {
			if (x->child != nullptr)
				text += "\"" + std::to_string(x->key) + "\"" + " -> " + "\"" + std::to_string(x->child->key) + "\";\n";
			if (x->sibling != nullptr)
				text += "\"" + std::to_string(x->key) + "\"" + " -> " + "\"" + std::to_string(x->sibling->key) + "\";\n";
			if (x->parent != nullptr)
				text += "\"" + std::to_string(x->key) + "\"" + " -> " + "\"" + std::to_string(x->parent->key) + "\";\n";

			getGraphInfo(x->child, text);
			getGraphInfo(x->sibling, text);
		}
	}

	Node<T>* search(std::string k, Node<T>* H) {
		Node<T>* x = H;
		Node<T>* p = nullptr;

		if (x->key.city == k) {
			p = x;
			return p;
		}

		if (x->child != nullptr && p == nullptr)
			p = search(k, x->child);

		if (x->sibling != nullptr && p == nullptr)
			p = search(k, x->sibling);

		return p;
	}

public:
	BinomialHeap() {};
	BinomialHeap(Node<T>* head) :head(head) {};

	void createEmpty() {
		head = new Node<T>;
	}

	Node<T>* minimum() {
		Node<T>* y = nullptr;
		Node<T>* x = head;

		T min = NUMERIC_MIN_INFINITY;

		while (x != nullptr) {
			if (x->key < min) {
				min = x->key;
				y = x;
			}
			x = x->sibling;
		}

		return y;
	}

	void binominalLink(Node<T>* y, Node<T>* z) {
		y->parent = z;
		y->sibling = z->child;
		z->child = y;
		z->degree++;
	}

	Node<T>* merge(BinomialHeap<T>* h1, BinomialHeap<T>* h2) {
		Node<T>* h1Node = h1->head;
		Node<T>* h2Node = h2->head;
		Node<T>* newHead = new Node<T>;
		Node<T>* iterator = new Node<T>;
		if (h1Node != nullptr && h2Node != nullptr) {
			if (h1Node->degree <= h2Node->degree) {
				newHead = h1Node;
				h1Node = h1Node->sibling;
			}
			else {
				newHead = h2Node;
				h2Node = h2Node->sibling;
			}
		}
		else if (h1Node != nullptr) {
			return h1Node;
		}
		else if (h2Node != nullptr) {
			return h2Node;
		}
		else {
			throw std::exception("A zachem?");
		}

		iterator = newHead;

		while (h1Node != nullptr && h2Node != nullptr) {
			if (h1Node->degree <= h2Node->degree) {
				iterator->sibling = h1Node;
				iterator = h1Node;
				h1Node = h1Node->sibling;
			}
			else {
				iterator->sibling = h2Node;
				iterator = h2Node;
				h2Node = h2Node->sibling;
			}
		}

		if (h2Node != nullptr) {
			iterator->sibling = h2Node;
		}
		else if (h1Node != nullptr) {
			iterator->sibling = h1Node;
		}

		return newHead;
	}

	Node<T>* heapUnion(BinomialHeap<T>* h1, BinomialHeap<T>* h2) {
		BinomialHeap<T>* h = new BinomialHeap<T>();
		h->createEmpty();
		h->head = merge(h1, h2);

		if (h->head == nullptr) {
			return nullptr;
		}

		Node<T>* prev_x = nullptr;
		Node<T>* x = h->head;
		Node<T>* next_x = x->sibling;

		while (next_x != nullptr) {
			if ((x->degree != next_x->degree) || (next_x->sibling != nullptr && next_x->sibling->degree == x->degree)) {
				prev_x = x;
				x = next_x;
			}
			else if (x->key <= next_x->key) {
				x->sibling = next_x->sibling;
				binominalLink(next_x, x);
			}
			else {
				if (prev_x == nullptr) {
					h->head = next_x;
				}
				else {
					prev_x->sibling = next_x;
				}
				binominalLink(x, next_x);
				x = next_x;
			}
			next_x = x->sibling;
		}
		return h->head;
	}

	void insert(T x) {
		BinomialHeap<T>* insertion = new BinomialHeap<T>();
		insertion->createEmpty();
		insertion->head->key = x;

		head = heapUnion(this, insertion);
	}

	Node<T>* extractMin() {
		Node<T>* min = head;
		Node<T>* current = head;
		Node<T>* preNode = nullptr;

		while (current != nullptr) {
			if (current->sibling != nullptr && current->sibling->key < min->key) {
				preNode = current;
			}
			if (current->key < min->key) {
				min = current;
			}
			current = current->sibling;
		}

		if (preNode != nullptr) {
			preNode->sibling = min->sibling;
		}

		current = min->child;
		std::vector<Node<T>*> reverse;

		while (current != nullptr) {
			current->parent = nullptr;
			reverse.push_back(current);
			current = current->sibling;
		}

		std::reverse(reverse.begin(), reverse.end());

		reverse[reverse.size() - 1]->sibling = nullptr;

		for (int i = 0; i < reverse.size() - 1; ++i) {
			reverse[i]->sibling = reverse[i + 1];
		}

		BinomialHeap<T>* extractor = new BinomialHeap<T>();
		extractor->createEmpty();
		extractor->head = reverse[0];

		head = heapUnion(this, extractor);
		return min;
	}

	void decreaseKey(std::string toChange, std::string k) {
		Node<T>* x = find(toChange);

		if (k > x->key.city) {
			return;
		}

		x->key.city = k;
		Node<T>* y = x;
		Node<T>* z = y->parent;

		while (z != nullptr && y->key < z->key) {
			T temp = y->key;
			y->key = z->key;
			z->key = temp;
			y = z;
			z = y->parent;
		}
	}

	std::string getWebGraphviz(std::string graphName = "G") {
		std::string graphText = "digraph " + graphName + " {\n";

		getGraphInfo(head, graphText);

		return graphText += "}";
	}

	Node<T>* find(std::string k) {
		return search(k, head);
	}
};

template<>
Node<WorldMap>* BinomialHeap<WorldMap> ::minimum() {
	Node<WorldMap>* y = nullptr;
	Node<WorldMap>* x = head;

	WorldMap min = MIN_INFINITY;

	while (x != nullptr) {
		if (x->key < min) {
			min = x->key;
			y = x;
		}
		x = x->sibling;
	}

	return y;
}

template<>
void BinomialHeap<WorldMap> ::getGraphInfo(Node<WorldMap>* x, std::string& text) {
	if (x != nullptr) {
		if (x->child != nullptr)
			text += "\"" + x->key.city + "\"" + " -> " + "\"" + x->child->key.city + "\";\n";
		if (x->sibling != nullptr)
			text += "\"" + x->key.city + "\"" + " -> " + "\"" + x->sibling->key.city + "\";\n";
		if (x->parent != nullptr)
			text += "\"" + x->key.city + "\"" + " -> " + "\"" + x->parent->key.city + "\";\n";

		getGraphInfo(x->child, text);
		getGraphInfo(x->sibling, text);
	}
}

int main() {
	BinomialHeap<WorldMap> anotherOne;

	anotherOne.insert({ "Spain", "Barcelona" });

	anotherOne.insert({ "Ukraine","Varash" });

	anotherOne.insert({ "Spain","Madrid" });
	anotherOne.insert({ "Russia","Mahachkala" });
	anotherOne.insert({ "China","Beijing" });
	anotherOne.insert({ "Madagaskar","Antananarivo" });
	anotherOne.insert({ "Columbia","Pictures" });
	anotherOne.insert({ "USA","Omerica" });
	anotherOne.insert({ "Canada","Coldcity" });
	anotherOne.insert({ "Tajikistan","Dushanbe" });
	anotherOne.insert({ "India","New Delhi" });


	std::cout << anotherOne.getWebGraphviz() << std::endl;

	std::cout << "\nMinimum: " << anotherOne.extractMin()->key << std::endl << std::endl;

	std::cout << anotherOne.getWebGraphviz() << std::endl;

	std::cout << "\nDecreasing key:\n\n";
	anotherOne.decreaseKey("Coldcity", "Aaaaaa ColdCity");

	std::cout << anotherOne.getWebGraphviz() << std::endl;

	system("pause >> void");

	return 0;
}