#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <cmath>
#include <exception>
#include <sstream>

#include "../Lab_5/WorldMap.h"

template<typename T>
struct node {
	T key;

	node<T>* left = nullptr;
	node<T>* right = nullptr;

	node<T>* parent = nullptr;
	node<T>* child = nullptr;

	int degree = 0;
	bool mark = false;

	std::string getText();
};

template<typename T>
class FibbonacciHeap {
private:
	node<T>* min;
	int nodeCount;

	void fibbonacciInsert(node<T>* x);
	void rootInsert(node<T>* x);
	void childInsert(node<T>* parent, node<T>* toInsert);
	void rootListConcatenation(node<T>* min2);
	void deleteFromRootlist(node<T>* toDelete);
	void deleteFromChildlist(node<T>* parent, node<T>* child);
	void consolidate();
	void fibHeapLink(node<T>* y, node<T>* x);
	std::vector<node<T>*> allMembers(node<T>* current);
	node<T>* search(T value);
	node<T>* recursiveSearch(T value, node<T>* current, node<T>* result);
	void cut(node<T>* x, node<T>* y);
	void cascadingCut(node<T>* y);
	void getGraphInfo(node<T>* x, std::string& text);

public:
	FibbonacciHeap();
	FibbonacciHeap(const std::initializer_list<T>& list);
	void fibbonacciInsert(T value);
	node<T>* findMin();
	FibbonacciHeap<T>* fibHeapUnion(FibbonacciHeap<T>* anotherHeap);
	node<T>* fibHeapExtractMin();
	void fibHeapDecreaseKey(T current, T decreased);
	void fibHeapDelete(T value);
	std::string getWebGraphviz(std::string graphName);

	node<T>* getMin();
};

template<typename T>
void FibbonacciHeap<T>::getGraphInfo(node<T>* x, std::string& text) {
	if (x == nullptr)
		return;

	for (auto item : allMembers(x)) {
		std::string currentNodeText = item->getText();
		std::stringstream stream;

		if (item == min)
			stream << "\"" << currentNodeText << "\" [color = \"red\"]" << ";\n";
		else {
			stream << "\"" << currentNodeText << "\";\n";
		}

		if (item->right)
			stream << "\"" << currentNodeText << "\"" << " -> " << "\"" << item->right->getText() << "\";\n";

		if (item->left)
			stream << "\"" << currentNodeText << "\"" << " -> " << "\"" << item->left->getText() << "\";\n";

		if (item->parent)
			stream << "\"" << currentNodeText << "\"" << " -> " << "\"" << item->parent->getText() << "\";\n";

		if (item->child) {
			stream << "\"" << currentNodeText << "\"" << " -> " << "\"" << item->child->getText() << "\";\n";

			text += stream.str();
			stream.str(std::string());
			getGraphInfo(item->child, text);
		}

		text += stream.str();
		stream.str(std::string());
	}
}

template<typename T>
std::string FibbonacciHeap<T>::getWebGraphviz(std::string graphName) {
	std::string graphText = "digraph " + graphName + " {\n";

	getGraphInfo(min, graphText);

	return graphText += "}";
}

template<typename T>
inline node<T>* FibbonacciHeap<T>::getMin(){
	return min;
}

template<typename T>
std::string node<T>::getText() {
	std::stringstream result;

	result << key;

	return result.str();
}

template<typename T>
FibbonacciHeap<T>::FibbonacciHeap() {
	min = nullptr;
	nodeCount = 0;
}

template<typename T>
FibbonacciHeap<T>::FibbonacciHeap(const std::initializer_list<T>& list) {
	min = nullptr;
	nodeCount = 0;
	for (const auto& item : list)
		fibbonacciInsert(item);
}

template<typename T>
void FibbonacciHeap<T>::fibbonacciInsert(T value) {
	node<T>* newNode = new node<T>;
	newNode->key = value;

	fibbonacciInsert(newNode);
}

template<typename T>
node<T>* FibbonacciHeap<T>::findMin() {
	return min;
}

template<typename T>
FibbonacciHeap<T>* FibbonacciHeap<T>::fibHeapUnion(FibbonacciHeap<T>* anotherHeap) {
	if (anotherHeap == nullptr)
		return this;

	FibbonacciHeap<T>* newHeap = new FibbonacciHeap<T>();
	newHeap->min = min;
	newHeap->rootListConcatenation(anotherHeap->min);

	if (min == nullptr || (anotherHeap->min != nullptr && anotherHeap->min->key < min->key)) {
		newHeap->min = anotherHeap->min;
	}

	newHeap->nodeCount = nodeCount + anotherHeap->nodeCount;
	return newHeap;
}

template<typename T>
node<T>* FibbonacciHeap<T>::fibHeapExtractMin() {
	node<T>* z = min;

	if (z != nullptr) {
		node<T>* x = z->child;

		if (x != nullptr) {
			node<T>* temp = x->right;

			rootInsert(x);

			for (x = temp; x != z->child; x = temp) {
				temp = x->right;
				rootInsert(x);
			}
		}

		deleteFromRootlist(z);

		if (z != z->left) {
			min = z->right;
			consolidate();
		}

		nodeCount--;
	}

	return z;
}

template<typename T>
void FibbonacciHeap<T>::fibHeapDecreaseKey(T current, T decreased) {
	if (current <= decreased) {
		return;
	}

	node<T>* toChange = search(current);

	if (toChange == nullptr) {
		return;
	}

	toChange->key = decreased;
	node<T>* y = toChange->parent;

	if (y != nullptr && toChange->key < y->key) {
		cut(toChange, y);
		cascadingCut(y);
	}
	if (toChange->key < min->key) {
		min = toChange;
	}
}

template<typename T>
void FibbonacciHeap<T>::fibHeapDelete(T value) {
	if (search(value) != nullptr) {
		fibHeapDecreaseKey(value, { "", "" });
		delete fibHeapExtractMin();
	}
}

template<typename T>
void FibbonacciHeap<T>::rootListConcatenation(node<T>* min2) {
	if (min != nullptr && min2 != nullptr) {
		min->left->right = min2;
		min2->left->right = min;

		node<T>* temp = min->left;

		min->left = min2->left;
		min2->left = temp;
	}
}

template<typename T>
void FibbonacciHeap<T>::deleteFromRootlist(node<T>* toDelete) {
	if (toDelete->left == toDelete) {
		min = nullptr;
		return;
	}

	toDelete->left->right = toDelete->right;
	toDelete->right->left = toDelete->left;
}

template<typename T>
void FibbonacciHeap<T>::deleteFromChildlist(node<T>* parent, node<T>* child) {
	parent->child = child->right;

	if (child->left == child) {
		parent->child = nullptr;
	}
	else {
		child->left->right = child->right;
		child->right->left = child->left;
	}

	parent->degree--;
}

template<typename T>
void FibbonacciHeap<T>::consolidate() {
	int size = (int)ceil(log2(nodeCount)) + 1;
	std::vector<node<T>*> A(size, nullptr);

	for (auto item : allMembers(min)) {
		node<T>* x = item;
		int d = x->degree;

		while (A[d] != nullptr) {
			node<T>* y = A[d];

			if (x->key > y->key) {
				std::swap(x, y);
			}

			fibHeapLink(y, x);
			A[d] = nullptr;
			d++;
		}

		A[d] = x;
	}

	min = nullptr;

	for (auto item : A) {
		if (item != nullptr) {
			if (min == nullptr) {
				min = item;
				min->left = min;
				min->right = min;
			}
			else {
				rootInsert(item);

				if (item->key < min->key) {
					min = item;
				}
			}
		}
	}
}

template<typename T>
void FibbonacciHeap<T>::fibHeapLink(node<T>* y, node<T>* x) {
	deleteFromRootlist(y);
	childInsert(x, y);
	x->degree++;
	y->mark = false;
}

template<typename T>
std::vector<node<T>*> FibbonacciHeap<T>::allMembers(node<T>* current)
{
	std::vector<node<T>*> result;

	node<T>* temp = current;

	if (current == nullptr) {
		return result;
	}

	result.push_back(current);

	temp = current->right;

	while (temp != current) {
		result.push_back(temp);
		temp = temp->right;
	}

	return result;
}

template<typename T>
node<T>* FibbonacciHeap<T>::search(T value) {
	node<T>* temp = min;
	node<T>* result = nullptr;
	result = recursiveSearch(value, temp, result);
	return result;
}

template<typename T>
node<T>* FibbonacciHeap<T>::recursiveSearch(T value, node<T>* current, node<T>* result) {
	for (auto item : allMembers(current)) {
		if (item->key == value) {
			result = item;
			return result;
		}
		if (item->child != nullptr && item->key < value) {
			result = recursiveSearch(value, item->child, result);
		}
		if (result != nullptr) {
			return result;
		}
	}
	return result;
}

template<typename T>
void FibbonacciHeap<T>::cut(node<T>* x, node<T>* y) {
	deleteFromChildlist(y, x);
	rootInsert(x);
	x->mark = false;
}

template<typename T>
void FibbonacciHeap<T>::cascadingCut(node<T>* y) {
	node<T>* z = y->parent;

	if (z != nullptr) {
		if (y->mark == false) {
			y->mark = true;
		}
		else {
			cut(y, z);
			cascadingCut(z);
		}
	}
}

template<typename T>
void FibbonacciHeap<T>::fibbonacciInsert(node<T>* x) {
	if (min == nullptr) {
		min = x;
		x->left = x;
		x->right = x;
	}
	else {
		rootInsert(x);

		if (x->key < min->key) {
			min = x;
		}
	}

	nodeCount++;
}

template<typename T>
void FibbonacciHeap<T>::rootInsert(node<T>* x) {
	if (min->left == min) {
		min->left = x;
		min->right = x;
		x->left = min;
		x->right = min;
	}
	else {
		x->left = min->left;
		min->left->right = x;
		min->left = x;
		x->right = min;
	}

	x->parent = nullptr;
}

template<typename T>
void FibbonacciHeap<T>::childInsert(node<T>* parent, node<T>* toInsert) {
	if (parent->child == nullptr) {
		parent->child = toInsert;
		toInsert->left = toInsert;
		toInsert->right = toInsert;
	}
	else if (parent->child->left == parent->child) {
		parent->child->left = toInsert;
		parent->child->right = toInsert;
		toInsert->left = parent->child;
		toInsert->right = parent->child;
	}
	else {
		toInsert->left = parent->child->left;
		parent->child->left->right = toInsert;
		parent->child->left = toInsert;
		toInsert->right = parent->child;
	}

	toInsert->parent = parent;
}