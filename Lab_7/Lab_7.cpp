﻿#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <cmath>


template<typename T>
struct Node {
	int key = 0;
	Node* parent = nullptr;
	Node* child = nullptr;
	Node* sibling = nullptr;
	int degree = 0;

	Node(int key) :key(key) {}
	Node() {};
};

template <typename T>
class BinomialHeap {
private:
	Node<T>* head = nullptr;

public:
	BinomialHeap() {};
	BinomialHeap(Node<T>* head) :head(head) {};

	void createEmpty() {
		head = new Node<T>;
	}

	Node<T>* minimum() {
		Node<T>* y = nullptr;
		Node<T>* x = head;
		T min = INT_MIN;

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

	void printHeap() {
		Node<T>* currPtr = head;
		while (currPtr != nullptr) {
			std::cout << "B" << currPtr->degree << std::endl;
			std::cout << "There are " << std::pow(2, currPtr->degree) << " nodes in this tree" << std::endl;
			std::cout << "The level order traversal is" << std::endl;
			std::queue<Node<T>*> q;
			q.push(currPtr);
			while (!q.empty()) {
				Node<T>* p = q.front();
				q.pop();
				std::cout << p->key << " ";

				if (p->child != nullptr) {
					Node<T>* tempPtr = p->child;
					while (tempPtr != nullptr) {
						q.push(tempPtr);
						tempPtr = tempPtr->sibling;
					}
				}
			}
			currPtr = currPtr->sibling;
			std::cout << std::endl << std::endl;
		}
	}

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

	std::string getWebGraphviz(std::string graphName = "G") {
		std::string graphText = "digraph " + graphName + " {\n";

		getGraphInfo(head, graphText);

		return graphText += "}";
	}
};

int main() {
	Node<int>* first = new Node<int>(12);
	Node<int>* second = new Node<int>(7);
	Node<int>* third = new Node<int>(25);
	Node<int>* fourth = new Node<int>(15);
	Node<int>* fifth = new Node<int>(28);
	Node<int>* six = new Node<int>(33);
	Node<int>* seventh = new Node<int>(41);

	first->sibling = second;

	second->child = third;
	second->degree = 1;
	third->parent = second;
	second->sibling = fourth;

	fourth->child = fifth;
	fourth->degree = 2;
	fifth->parent = fourth;
	fifth->degree = 1;
	fifth->sibling = six;
	six->parent = fourth;

	fifth->child = seventh;
	seventh->parent = fifth;


	Node<int>* a = new Node<int>(18);
	Node<int>* b = new Node<int>(3);
	Node<int>* c = new Node<int>(37);
	Node<int>* d = new Node<int>(6);
	Node<int>* e = new Node<int>(8);
	Node<int>* f = new Node<int>(29);
	Node<int>* g = new Node<int>(10);
	Node<int>* h = new Node<int>(44);
	Node<int>* i = new Node<int>(30);
	Node<int>* j = new Node<int>(23);
	Node<int>* k = new Node<int>(22);
	Node<int>* l = new Node<int>(48);
	Node<int>* m = new Node<int>(31);
	Node<int>* n = new Node<int>(17);
	Node<int>* o = new Node<int>(45);
	Node<int>* p = new Node<int>(32);
	Node<int>* q = new Node<int>(24);
	Node<int>* r = new Node<int>(50);
	Node<int>* s = new Node<int>(55);

	a->sibling = b;
	b->degree = 1;
	b->child = c;
	c->parent = b;

	d->degree = 4;
	e->degree = 3;
	f->degree = 2;
	g->degree = 1;

	i->degree = 2;
	j->degree = 1;

	l->degree = 1;

	o->degree = 1;

	b->sibling = d;
	d->child = e;
	e->parent = d;
	e->sibling = f;
	f->parent = d;
	f->sibling = g;
	g->parent = d;
	g->sibling = h;
	h->parent = d;

	e->child = i;
	i->parent = e;
	i->sibling = j;
	j->parent = e;
	j->sibling = k;
	k->parent = e;

	f->child = l;
	l->parent = f;
	l->sibling = m;
	m->parent = f;

	g->child = n;
	n->parent = g;

	i->child = o;
	o->sibling = p;
	p->parent = i;
	o->parent = i;
	q->parent = j;
	j->child = q;

	l->child = r;
	r->parent = l;

	o->child = s;
	s->parent = o;

	BinomialHeap<int>* h1 = new BinomialHeap<int>(first);
	BinomialHeap<int>* h2 = new BinomialHeap<int>(a);

	Node<int>* newHead = h1->heapUnion(h1, h2);

	BinomialHeap<int>* result = new BinomialHeap<int>(newHead);

	std::cout << result->getWebGraphviz();

	system("pause");

	return 0;
}