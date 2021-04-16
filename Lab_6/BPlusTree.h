#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <algorithm>
#include "E:/Visual studio/Algorithms_labs/Lab_5/WorldMap.h"

template <typename T>
class BPlusTreeNode {
	static size_t N;

	const size_t ID;
	bool isLeaf = false;
	BPlusTreeNode<T>* sibling = nullptr;

	std::vector<T> keys;
	std::vector<BPlusTreeNode<T>*> children;

	template <typename T, const size_t>
	friend class BPlusTree;


public:
	std::string getText();

public:
	BPlusTreeNode();
};

template <typename T>
size_t BPlusTreeNode<T>::N = 0;

template<typename T>
BPlusTreeNode<T>::BPlusTreeNode<T>()
	: ID(N++) {}

template<typename T>
std::string BPlusTreeNode<T>::getText() {
	std::stringstream result;

	for (const auto& key : keys) {
		result << key << " ";
	}

	result << "ID : " << ID;

	return result.str();
}

template <typename T, const size_t degree>
class BPlusTree {
	BPlusTreeNode<T>* root;

	void getGraphInfo(BPlusTreeNode<T>* x, std::string& text);

	void splitChild(BPlusTreeNode<T>* parent, size_t childIndex);

	void insertNonfull(BPlusTreeNode<T>* current, const T& key);

	std::pair<BPlusTreeNode<T>*, size_t> search(BPlusTreeNode<T>* current, const T& key);

	std::pair<BPlusTreeNode<T>*, size_t>  findParent(BPlusTreeNode<T>* current, BPlusTreeNode<T>* child);

	T findMinElement(BPlusTreeNode<T>* root);

	BPlusTreeNode<T>* findLeftSibling(BPlusTreeNode<T>* node);

	BPlusTreeNode<T>* findGlobalLeftSibling(BPlusTreeNode<T>* current, BPlusTreeNode<T>* node);

	BPlusTreeNode<T>* findGlobalLeftSibling(BPlusTreeNode<T>* node);

	BPlusTreeNode<T>* findRightSibling(BPlusTreeNode<T>* node);

	void recursiveSplit(BPlusTreeNode<T>* current);

	BPlusTreeNode<T>* mergeLeafNodes(BPlusTreeNode<T>* left, BPlusTreeNode<T>* right);

	BPlusTreeNode<T>* mergeInternalNodes(BPlusTreeNode<T>* left, BPlusTreeNode<T>* right);

	void removeFixup(BPlusTreeNode<T>* current, const T& key);
public:
	BPlusTree();
	BPlusTree(const std::initializer_list<T>& list);

	void insert(const T& key);

	std::pair<BPlusTreeNode<T>*, size_t> search(const T& key);

	void remove(const T& key);

	std::string getWebGraphviz(std::string graphName = "G");
};

#include "BPlusTree.hxx"